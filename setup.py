#!/usr/bin/python
# -*- encoding: iso-8859-15 -*-

from distutils.core import setup, Extension
import glob, os

_PyTango = Extension('PyTango._PyTango',
		include_dirs = ['src'],
		libraries = ['boost_python', 'tango', 'log4tango'],
		library_dirs = [''],
		sources = glob.glob(os.path.join('src', '*.cpp')))

setup(name='PyTango',
	version='3.0.4',
	description='Python Tango control system binding',
	url='http://www.tango-controls.org',
	packages=['PyTango'],
	package_dir={'PyTango' : 'modules'},
	ext_modules = [_PyTango],
	)
