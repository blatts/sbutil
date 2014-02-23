#!/usr/bin/env python
# -*- mode: Python; coding: latin-1 -*-
# Time-stamp: "2014-02-22 19:34:36 sb"

#  file       SConstruct
#  copyright  (c) Sebastian Blatt 2013, 2014

import os

# Import or create build environment and allow replacing compiler from
# environment variables if set
env = None
try:
  Import('env')
except:
  env = Environment()

for key in ['CC', 'CXX']:
  if os.environ.has_key(key):
    env[key] = os.environ[key]

# Autoconf
conf = Configure(env)

# check CXX
if not conf.CheckCXX():
  print('Need C++ compiler.')
  Exit(0)

env = conf.Finish()


# Export current environment and call SConscript
Export('env')
SConscript('SConscript')

# SConstruct ends here
