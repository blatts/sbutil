#!/usr/bin/env python
# -*- mode: Python; coding: latin-1 -*-
# Time-stamp: "2014-02-21 18:40:14 sb"

#  file       SConstruct
#  copyright  (c) Sebastian Blatt 2013, 2014

# environment variables:
#   LIBPATH, LIBS, ASFLAGS, LINKFLAGS, CPPFLAGS, CPPPATH, CCFLAGS

import os.path

use_clang = True

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

env = Environment()

# switch to clang++
if use_clang:
  cc = 'clang'
  cxx = 'clang++'
  env.Replace(CC = cc, CXX = cxx)

cxxflags = '-g -O3'
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
