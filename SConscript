#!/usr/bin/env python
# -*- mode: Python; coding: latin-1 -*-
# Time-stamp: "2014-02-22 19:21:26 sb"

#  file       SConstruct
#  copyright  (c) Sebastian Blatt 2013, 2014

# environment variables:
#   LIBPATH, LIBS, ASFLAGS, LINKFLAGS, CPPFLAGS, CPPPATH, CCFLAGS

import os.path

# Import environment exported in calling SConstruct file
Import('env')


build_directory = 'build/scons/'

include_directories = [
    '/sw/include',
    ]

library_directories = [
    ]

frameworks = []

warnings = [
    '',
    'all'
    ]


cxxflags = '-g -O3 -std=c++11'
linkflags = ''

cxxflags += " " + " ".join(map(lambda w: '-W%s' % w, warnings))
linkflags += " " + " ".join(map(lambda f: '-framework %s' % f, frameworks))


env.Append(LINKFLAGS = linkflags)
env.Append(LIBPATH = library_directories)
env.Append(CPPPATH = include_directories)
env.Append(CXXFLAGS = cxxflags)

env.SConscript('src/SConscript',
               variant_dir = build_directory + 'sbutil',
               duplicate = 0,
               exports = 'env')

# SConstruct ends here
