# -*- python -*-

import os, stat, sys

from distutils.core import setup, Extension, Command
from distutils.errors import DistutilsExecError
from distutils.command.install import install as base_install

version = '0.0.3'


natspec = [
    'wrapper.c'
    ]

includes = []
libs     = []
libdirs  = []

def error (msg):
    sys.stderr.write ('setup.py: error: %s\n' % msg)
    return


class run_check (Command):

    """ Run all of the tests for the package using uninstalled (local)
    files """
    
    description = "Automatically run the test suite for the package."

    user_options = []

    def initialize_options(self):
        self.build_lib = None
        return


    def finalize_options(self):
        # Obtain the build_lib directory from the build command
        self.set_undefined_options ('build', ('build_lib', 'build_lib'))
        return

    def run(self):
        # Ensure the extension is built
        self.run_command ('build')
        
        # test the uninstalled extensions
        libdir = os.path.join (os.getcwd (), self.build_lib)

        sys.path.insert (0, libdir)

        import testsuite

        try:
            failures = testsuite.run ()

        except RuntimeError, msg:
            sys.stderr.write ('error: %s\n' % msg)
            raise DistutilsExecError ('please consult the "Troubleshooting" section in the README file.')

        if failures > 0:
            raise DistutilsExecError ('check failed.')
        return
    


class run_install (base_install):

    def run(self):
        # The code must pass the tests before being installed
        self.run_command ('check')
        
        base_install.run (self)
        return
    
        
# Actual compilation

setup (name = "python-natspec",
       version = version,

       description = "A Python extension to support natspec",
       author = "Vitaly Lipatov",
       author_email = 'lav@etersoft.ru',
       url = 'http://etersoft.ru/natspec',

       license = 'GPL',
       
       cmdclass = { 'check':   run_check,
                    'install': run_install },
       
       long_description = \
'''
This module contains one extension for natspec :
''',

       ext_modules = [

    Extension("_natspec", natspec,
              include_dirs = includes,
              library_dirs = libdirs,
              define_macros = [('G_LOG_DOMAIN', '"NATSPEC"')],
              libraries = libs)
    ])
