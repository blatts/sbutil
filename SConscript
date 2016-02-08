#!/usr/bin/env python
# -*- mode: Python; coding: latin-1 -*-
# Time-stamp: "2016-02-08 15:47:18 sb"

#  file       SConscript
#  copyright  (c) Sebastian Blatt 2013, 2014, 2015, 2016

# environment variables:
#   LIBPATH, LIBS, ASFLAGS, LINKFLAGS, CPPFLAGS, CPPPATH, CCFLAGS

import os.path

# Import environment exported in calling SConstruct file
Import('env', 'using_cxx11')
# Copy environment so that we do not modify upstairs
localenv = env.Clone()

build_directory = 'build/scons/'
test_directory = 'build/test/'

include_directories = [
    ]

library_directories = [
    ]


frameworks = []

warnings = [
    '',
    'all'
    ]


cxxflags = '-g -O3'
if using_cxx11:
  cxxflags += ' -std=c++11'

linkflags = ''

cxxflags += " " + " ".join(map(lambda w: '-W%s' % w, warnings))
linkflags += " " + " ".join(map(lambda f: '-framework %s' % f, frameworks))


localenv.Append(LINKFLAGS = linkflags)
localenv.Append(LIBPATH = library_directories)
localenv.Append(CPPPATH = include_directories)
localenv.Append(CXXFLAGS = cxxflags)

SConscript('src/SConscript',
           variant_dir = build_directory + 'sbutil',
           duplicate = 0,
           exports = {'env' : localenv})

SConscript('src/SConscript-test',
           variant_dir = test_directory + 'sbutil',
           duplicate = 0,
           exports = {'env' : localenv})

# SConscript ends here
