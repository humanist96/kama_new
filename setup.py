#!/usr/bin/python

from distutils.core import setup, Extension

setup(
	name = "kama",
	py_modules = ["kama"],
	ext_modules = [Extension("_kama",
		["kama_wrap.cxx", "kama.cpp"],
		include_dirs=['.', '/home/dennis/Local/anaconda3/include/python3.6m'],
		library_dirs=['.'],
		libraries=['mecab', 'stdc++']
	)],
	version = '0.0.1',
	fullname = 'kama-0.01',
	author = 'Wonjong Choi',
	author_email = 'wj77.choi@gmail.com',
	url = '',
	description = ''
)
