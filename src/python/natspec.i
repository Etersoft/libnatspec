/***********************************************************
*           Interface file for wrapping libnatspec
*           author: Bryan Cole  email: bryan.cole@teraview.co.uk  
*
*     This file was created with Python wrappers in mind but will
*     probably work for other swig-supported script languages
*
*    to regenerate the wrappers run:
*    swig -python natspec.i
*
***********************************************************/
%module natspec
%{
#include "../../lib/natspec.h"
%}

%include "../../lib/natspec.h"
