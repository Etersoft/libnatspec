from distutils.core import setup, Extension

module1 = Extension('_natspec',
			define_macros = [('MAJOR_VERSION','0'),
					('MINOR_VERSION','1')],
			include_dirs = ['../../include'],
			library_dirs = ['../../lib'],
			libraries = ['natspec'],
			sources = ['natspec_wrap.c'])
			
setup (name='_natspec',
	version = '0.1.9',
	description = 'Python wrapper module for the LIBNATSPEC',
	author = 'Vitaly Lipatov',
	author_email = 'lav@etersoft.ru',
	long_description = ''' Wrapper for the Nation Specifity Library ''',
	ext_modules = [module1])
