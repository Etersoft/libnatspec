/***************************************************************************
                           submountd.c
                             -------------------
    begin                : Wed May 14 12:29:05 EDT 2003
    copyright            : (C) 2003-2004 by Eugene S. Weiss
    email                : eweiss@sbcglobal.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 /* This is the user portion of the submount system.  It's just a lightweight
  * program for mountiong and unmounting filesystems.
  */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/user.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>

#if USE_RESMGR
#include <resmgr.h>
#include <pwd.h>
#endif

#ifdef HAVE_NATSPEC
#include <natspec.h>
#endif

#include "submountd.h"


static struct fs_options fs_opts[] = {
	{"iso9660", 1, 0, 1},
	{"ntfs", 1, 1, 1},
	{"udf", 1, 1, 1},
	{NULL, 0, 0, 0}
};


/* This is the main routine for the unmounting daemon.  It checks that
 * the filesystem it mounted is at the top of the mountpoint, and then
 * attempts a umount if it is.  If EBUSY is returned from the umount call,
 * the loop is continued.  As of version 0.2, it includes a kludge to prevent
 * layer-caking mounts when a program automatically mounts a device without
 * checking if it is already mounted (urpmi).  It xshould be removed once the
 * programs that do this are fixed.
 */
void umountd(struct mount_params *p)
{

	int retval, match;
	FILE *mntlist;
	char line[4096], fs[16], top[16], mntpt[4096 + 1];

	sleep(SPINUP_DELAY);
	for (;;) {
		sleep(INTERVAL);
		mntlist = fopen(MOUNT_LIST, "r");
		if (!mntlist)
			continue;
		match = 0;
		while (fgets(line, 4096, mntlist)) {
			sscanf(line, "%*s %4096s %15s", mntpt, fs);
			if (strcmp(p->mountpoint, mntpt) == 0) {
				strcpy(top, fs);
				match++;
			}
		}
		fclose(mntlist);
		if ((!match) || ((strncmp(fs, "subfs", 6) == 0) &&
					(match < 3)))  {
			exit(EXIT_SUCCESS);
		}
		if ((strcmp(p->fstype, fs) == 0) || (match > 2)) {
			retval = umount(p->mountpoint);
			if ((!retval) && (match < 3))
				exit(EXIT_SUCCESS);
			if (errno != EBUSY)
				exit(EXIT_FAILURE);
		}
	}
}


/* Forks off the unmounting daemon.
 */
void launch_umountd(struct mount_params *p)
{

	pid_t pid;

	pid = fork();
	switch (pid) {
	case -1:
		break;
	case 0:
		umountd(p);
	default:
		break;
	}
	return;
}


/* Removes options relevent only to non-UNIX filesystems so they can be
 * used for mounts with automatic filesystem determination.
 */
char *proc_options(char *fstype, char *options)
{

	char *tmp, *ptr;
	int iocharset = 0, umask = 0, uid = 0, i = 0;

	if (!(tmp = malloc(strlen(options) + 1)))
		exit(EXIT_FAILURE);
	if (strcmp("vfat", fstype) == 0) {
		strcpy(tmp, options);
		return tmp;
	}
	while (fs_opts[i].name) {
		if (strcmp(fs_opts[i].name, fstype) == 0) {
			iocharset = fs_opts[i].iocharset;
			umask = fs_opts[i].umask;
			uid = fs_opts[i].uid;
			break;
		}
		i++;
	}
	tmp[0] = '\0';
	while ((ptr = strsep(&options, ","))) {
		if ((!strstr(ptr, "codepage")) &&
		    ((!strstr(ptr, "iocharset")) || (iocharset)) &&
		    ((!strstr(ptr, "umask")) || (umask)) &&
		    ((!strstr(ptr, "uid=")) || (uid)) &&
		    ((!strstr(ptr, "gid=")) || (uid))) {
			if ( strncmp("uid=0,",ptr,6) && strcmp("uid=0",ptr)
			  && strncmp("gid=0,",ptr,6) && strcmp("gid=0",ptr) ){
				strcat(tmp, ptr);
				strcat(tmp, ",");
			}
		}
	}
	return tmp;
}

/* Actual mounting done here.
 */
int do_mount(char *device, char *mountpoint, char *fstype,
	     unsigned long flags, char *options)
{
	int retval;
#if USE_RESMGR
	struct passwd *pwd;
	int uid = 0;
	char *p, **sessions = rsm_list_sessions();

	if ( options ){
		p = strdup( options );
		do {
			if (!strncmp(p, "uid=", 4))
				uid = atoi( p + 4 );
		} while ((p+1) && (p=strstr(p+1, ",")));
	}

	if ( uid && sessions && *sessions ){
		int okay = 0;
		pwd = getpwuid(uid);
		for ( ; sessions && *sessions; sessions++ ){
			p = strstr( *sessions, " " );
			if ( p && *(p+1) && !strcmp( p+1, pwd->pw_name ) )
			    okay = 1;
		}
		if ( !okay ){
			ERR("device reserved by resmgr session");
			return -1;
		}
	}
#endif

#ifdef HAVE_NATSPEC
	options = natspec_get_enriched_fs_options(fstype, options);
#endif
	retval = mount(device, mountpoint, fstype, flags, options);

	if (retval) {
		if ((errno == EROFS) && (!(flags & MS_RDONLY))) {
			flags |= MS_RDONLY;
			retval = do_mount(device, mountpoint, fstype, flags,
				  options);
		}
	}
#ifdef HAVE_NATSPEC
	free(options);
#endif
	return retval;
}


char *floppyfss[] = {"vfat", "ext2", "minix", NULL };
char *cdfss[] = {"iso9660", "udf", NULL };

/* Mounts a set of fs types from the arrays above, or a list provided by the
 * user.  When it's only a question of two or three fs types, this will be
 * faster than auto, particularly for floppies.
 */
char *do_list_mount(struct mount_params *p, char **types)
{

	char **list, *opts, *copy;
	int retval = 0, i = 0;

	if (types)
		list = types;
	else if (strncmp("floppyfss", p->fstype, 9) == 0)
		list = floppyfss;
	else if (strncmp("cdfss", p->fstype, 5) == 0)
		list = cdfss;
	else
		return NULL;
	if (!(copy = malloc(strlen(p->options) + 1)))
		exit(EXIT_FAILURE);
	while (list[i]) {
		strcpy(copy, p->options);
		opts = proc_options(list[i], copy);
		retval =
		    do_mount(p->device, p->mountpoint, list[i], p->flags,
			     opts);
		free(opts);
		if (!retval) {
			free(copy);
			return list[i];
		}
		i++;
	}
	free(copy);
	return NULL;
}

/* Mounts from a colon seperated list. */
char *user_list_mount(struct mount_params *p)
{

	int len = strlen(p->fstype);
	int i, num_types = 1;
	char **list, *fstype;

	for (i = 0; i < len; i++) {
		if (p->fstype[i] == ':')
			num_types++;
	}
	if (!(list = malloc(sizeof(*list) * (num_types + 1))))
		exit(EXIT_FAILURE);
	i = 0;
	while ((list[i] = strsep(&p->fstype, ":")))
		i++;
	fstype = do_list_mount(p, list);
	free(list);
	return fstype;
}


int main(int argc, char *argv[])
{

	int i, retval = 0;
	struct mount_params p;
	char *ptr, *opts = NULL;

	openlog("submountd", LOG_CONS | LOG_ODELAY, LOG_USER);

	if (argc != 6) {
		ERR("%d arguments received, 6 needed.\n", argc);
		exit(EXIT_FAILURE);
	}

	/* Test option size-- if anything is bigger than PAGE_SIZE, this
	   didn't come from the kernel. */
	for (i = 0; i < argc; i++) {
		ptr = argv[i];
		if ((strnlen(ptr, getpagesize())) > (getpagesize() - 1)) {
			ERR("Argument too large for syscall.\n");
			exit(EXIT_FAILURE);
		}
	}

	p.device = argv[1];
	p.mountpoint = argv[2];
	p.fstype = argv[3];
	p.options = argv[5];

	sscanf(argv[4], "%lx", &p.flags);
	if (strchr(p.fstype, ':')) {
		p.fstype = user_list_mount(&p);
		if (!p.fstype)
			retval = -1;
	} else if (strstr(p.fstype, "fss")) {
		p.fstype = do_list_mount(&p, NULL);
		if (!p.fstype)
			retval = -1;
	} else if (strncmp("auto", p.fstype, 5) == 0) {
		p.fstype = do_guess_fstype(p.device);
		if (!p.fstype) {
			ERR("unable to determine filesystem type\n");
			retval = -1;
		}
		else {
		opts = proc_options(p.fstype, p.options);
		retval =
		    do_mount(p.device, p.mountpoint, p.fstype, p.flags,
			     opts);
		free(opts);
		}
	} else
		retval =
		    do_mount(p.device, p.mountpoint, p.fstype, p.flags,
			     p.options);

	if (retval) {
		ERR("mount failure, %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	launch_umountd(&p);
	exit(EXIT_SUCCESS);
}
