#!/usr/bin/env python
# -*- mode: Python; coding: latin-1 -*-
# Time-stamp: "2014-02-22 21:21:17 sb"

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

def CheckCXX11(context):
  cxx11_compatible_source = '''
int main(int, char**){
  return 0;
}
'''
  context.Message('Checking whether `%s\' accepts -std=c++11... ' % \
                  context.env['CXX'])
  #cache_cxxflags = context.env['CXXFLAGS']
  context.env.Append(CXXFLAGS = '-std=c++11')
  ret = context.TryLink(cxx11_compatible_source, '.cc')
  if ret:
    context.Result('yes')
  else:
    context.Result('no')
  #context.env.Replace(CXXFLAGS = cache_cxxflags)
  return ret

# Autoconf
conf = Configure(env, custom_tests = {'CheckCXX11' : CheckCXX11})

# check CXX
if not conf.CheckCXX():
  print('Need C++ compiler.')
  Exit(0)

using_cxx11 = True
if not conf.CheckCXX11():
  print 'Yo! Your C++ is not at std=c++11 yet, disabling C++11 features.'
  using_cxx11 = False

env = conf.Finish()



# Export current environment and call SConscript
Export('env', 'using_cxx11')
SConscript('SConscript')

# SConstruct ends here
