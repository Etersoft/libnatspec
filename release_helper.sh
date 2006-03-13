#!/bin/sh
. /etc/rpm/etersoft-build-functions

check_key

update_from_cvs

prepare_tarball

rpmbb $SPECNAME || fatal "Can't build"

PUBLICSERVER=etersoft
PUBLICPATH=~/download/$BASENAME

publish_tarball

PUBLICSERVER=cf.sf
PUBLICPATH=~

#publish_tarball

