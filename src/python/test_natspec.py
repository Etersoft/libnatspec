#!/usr/bin/env python
# A test-application to demonstrate using the libnatspec API from Python. 

# set the paths so python can find the natspec module
import sys, os, string
sys.path.append('./build/lib.linux-i686-2.3')

import natspec as n

str = n.natspec_get_filename_encoding("")
print str

print "Infinite cycle:"
while True:
	s = n.natspec_convert(str,str,"UTF8")
	#print s
