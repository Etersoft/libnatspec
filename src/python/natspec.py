# This file was created automatically by SWIG.
# Don't modify this file, modify the SWIG interface instead.
# This file is compatible with both classic and new-style classes.

import _natspec

def _swig_setattr(self,class_type,name,value):
    if (name == "this"):
        if isinstance(value, class_type):
            self.__dict__[name] = value.this
            if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
            del value.thisown
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    self.__dict__[name] = value

def _swig_getattr(self,class_type,name):
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError,name

import types
try:
    _object = types.ObjectType
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0
del types


NATSPEC_UNIXCS = _natspec.NATSPEC_UNIXCS
NATSPEC_WINCS = _natspec.NATSPEC_WINCS
NATSPEC_DOSCS = _natspec.NATSPEC_DOSCS
NATSPEC_MACCS = _natspec.NATSPEC_MACCS

natspec_get_current_locale = _natspec.natspec_get_current_locale

natspec_get_user_locale = _natspec.natspec_get_user_locale

natspec_get_system_locale = _natspec.natspec_get_system_locale

natspec_get_charset = _natspec.natspec_get_charset

natspec_get_filename_encoding = _natspec.natspec_get_filename_encoding

natspec_get_charset_by_locale = _natspec.natspec_get_charset_by_locale

natspec_convert_with_translit = _natspec.natspec_convert_with_translit

natspec_convert = _natspec.natspec_convert

natspec_get_enriched_fs_options = _natspec.natspec_get_enriched_fs_options

natspec_get_codepage_by_charset = _natspec.natspec_get_codepage_by_charset

natspec_get_nls_by_charset = _natspec.natspec_get_nls_by_charset

natspec_extract_charset_from_locale = _natspec.natspec_extract_charset_from_locale

natspec_humble_charset = _natspec.natspec_humble_charset

natspec_get_charset_by_charset = _natspec.natspec_get_charset_by_charset

