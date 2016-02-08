#!/usr/bin/env python
# -*- mode: Python; coding: latin-1 -*-
# Time-stamp: "2016-02-08 16:47:16 sb"

#  file       SConstruct
#  copyright  (c) Sebastian Blatt 2013, 2014, 2015, 2016

import os
import subprocess
import copy

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


# Auto-detect common base directories for library installation
auto_rootdirs = ('/sw', '/usr/local', '/opt')
auto_include_directories = []
auto_library_directories = []
for j in auto_rootdirs:
    if os.path.isdir(j):
        path_include = os.path.join(j, 'include')
        path_library = os.path.join(j, 'lib')
        if os.path.isdir(path_include):
            auto_include_directories.append(path_include)
        if os.path.isdir(path_library):
            auto_library_directories.append(path_library)

class TestExternalCall(object):

    def __init__(self):
        self.command = None
        self.out = None
        self.err = None
        self.returncode = None

    def test(self, command_list):
        p = subprocess.Popen(
            command_list, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        self.out, self.err = p.communicate()
        self.returncode = p.returncode
        if self.returncode != 0:
            raise Exception('Command %s failed with code %d.\n%s' % (command_list,
                                                                     self.returncode,
                                                                     self.err))
        return self.returncode == 0


class TestPackageConfig(TestExternalCall):

    def __init__(self, package_config_command):
        super(TestPackageConfig, self).__init__()
        self.cmd = package_config_command

        self.version = None
        self.cflags = None
        self.libs = None
        self.prefix = None

    def test_config_script_exists(self):
        return super(TestPackageConfig, self).test([self.cmd, '--version'])

    def get_version(self):
        rc = super(TestPackageConfig, self).test([self.cmd, '--version'])
        self.version = self.out
        return rc

    def get_cflags(self):
        rc = super(TestPackageConfig, self).test([self.cmd, '--cflags'])
        self.cflags = self.out
        return rc

    def get_libs(self):
        rc = super(TestPackageConfig, self).test([self.cmd, '--libs'])
        self.libs = self.out
        return rc

    def get_prefix(self):
        rc = super(TestPackageConfig, self).test([self.cmd, '--prefix'])
        self.prefix = self.out
        return rc

    def get_all(self):
        fcs = [self.get_version,
               self.get_cflags,
               self.get_libs,
               self.get_prefix]
        rc = 0
        for f in fcs:
            rc = f()
            if not rc:
                break
        return rc



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
  rc = ret == 1
  if rc:
    context.Result('yes')
  else:
    context.Result('no')
  #context.env.Replace(CXXFLAGS = cache_cxxflags)
  return rc


def CheckGSL(context):
    context.Message('Checking for GSL using gsl-config... ')
    c = TestPackageConfig('gsl-config')
    rc = False
    try:
        rc = c.get_all()
        if not rc:
          raise Exception('gsl-config failed.')

        context.env.Append(CXXFLAGS=c.cflags,
                           CFLAGS=c.cflags,
                           LINKFLAGS=c.libs)

        have_gsl_source = '''
#include <gsl/gsl_version.h>
int main(int, char**){
  return 0;
}
'''
        ret = context.TryLink(have_gsl_source, '.cc')
        rc = ret == 1
        if not rc:
            raise Exception('linking failed.')

    except Exception as e:
        context.Message(e.message)
        context.Result('no')
        return c.returncode

    context.Result('yes')
    return rc



def CheckHDF5(context):
    context.Message('Checking for HDF5 libraries... ')
    hdf5_source = '''
extern "C" {
#include <hdf5.h>
}
int main(int, char**){
  return 0;
}
'''
    rc = False
    try:
        context.env.Append(LINKFLAGS='-lhdf5',
                           CPPPATH=auto_include_directories,
                           LIBPATH=auto_library_directories)
        ret = context.TryLink(hdf5_source, '.cc')
        rc = ret == 1
        if not rc:
            raise Exception('linking failed.')

    except Exception as e:
        context.Message(e.message)
        context.Result('no')
        return rc

    context.Result('yes')
    return rc

# Autoconf
conf = Configure(env, custom_tests = {'CheckCXX11' : CheckCXX11,
                                      'CheckGSL': CheckGSL,
                                      'CheckHDF5': CheckHDF5,
})

# check CXX
if not conf.CheckCXX():
  print('Need C++ compiler.')
  Exit(0)

using_cxx11 = True
if not conf.CheckCXX11():
  print 'Yo! Your C++ is not at std=c++11 yet, disabling C++11 features.'
  using_cxx11 = False

# Add auto-detected directories
conf.env.Append(CPPPATH=auto_include_directories,
                LIBPATH=auto_library_directories)

# check GSL
if not conf.CheckGSL():
    print 'Need the GSL library and `gsl-config\'.'
    Exit(0)

# check HDF5
if not conf.CheckHDF5():
    print 'Need the HDF5 library.'
    Exit(0)


env = conf.Finish()



# Export current environment and call SConscript
Export('env', 'using_cxx11')
SConscript('SConscript')

# SConstruct ends here
