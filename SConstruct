#!/usr/bin/env python3
# -*- mode: Python; coding: latin-1 -*-
# Time-stamp: "2019-12-10 17:43:28 sb"

#  file       SConstruct
#  copyright  (c) Sebastian Blatt 2018, 2019

import os
import platform
import subprocess
import copy
import re


def determine_platform():
    """Where are we running?"""
    x = (os.name, platform.system())
    if x == ('posix', 'Darwin'):
        return 'osx'
    elif x == ('posix', 'Linux'):
        return 'linux'
    elif x[0] == 'posix' and x[1][0:10] == 'MINGW64_NT':
        return 'mingw64'
    elif x == ('nt', 'Windows'):
        return 'windows'
    else:
        raise BaseException("SConstruct: unsupported (os, platform) = {}".format(x))

PLATFORM = determine_platform()
print("Running on \"{}\"".format(PLATFORM))


def auto_detect_base_directories():
    """Auto-detect common base directories for library installation.
    Return tuple of arrays (base_directories, include_directories,
    library_directories).
    """
    auto_rootdirs = None
    if PLATFORM == 'linux':
        auto_rootdirs = ('/usr', '/usr/local', '/opt')
    elif PLATFORM == 'osx':
        auto_rootdirs = ('/usr/local', '/opt')
    elif PLATFORM == 'mingw64':
        auto_rootdirs = ('/mingw64',)
    elif PLATFORM == 'windows':
        auto_rootdirs = ()

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

    return auto_rootdirs, auto_include_directories, auto_library_directories

AUTO_ROOTDIRS = []
AUTO_INCLUDE_DIRS = []
AUTO_LIB_DIRS = []

if PLATFORM != 'windows':
    AUTO_ROOTDIRS, AUTO_INCLUDE_DIRS, AUTO_LIB_DIRS = auto_detect_base_directories()


def auto_detect_file(parent_paths, relative_path):
    """Try to find a regular file specified by `relative_path' with
    respect to all members in `parent_paths'. If it is found, return
    the first of `parent_paths' it was found in.
    """
    rc = []
    for p in parent_paths:
        candidate = os.path.join(p, relative_path)
        if os.path.isfile(candidate):
            rc.append(p)
    if len(rc) > 1:
        print('SConstruct WARNING: found "{}" in multiple ' + \
              'root directories:\n  {}\n' + \
              '  default to {}'.format(relative_path, rc, rc[0]))

    return rc[0], os.path.join(rc[0], relative_path)

def auto_detect_header(relative_path):
    return auto_detect_file(AUTO_INCLUDE_DIRS, relative_path)


def auto_detect_file_regex(parent_paths, regex):
    rc = []
    for p in parent_paths:
        files = os.listdir(p)
        libs = []
        for f in files:
            m = re.match(regex, f)
            if m is not None:
                rc.append(p)
                libs.append(os.path.join(p, f))
    return rc[0] if rc else None, libs

def auto_detect_library(linkflag):
    pattern = None
    if PLATFORM == 'osx':
        pattern = r'lib' + linkflag + r'(\.[0-9]+)*\.(a|dylib)$'
    elif PLATFORM == 'linux':
        pattern = r'lib' + linkflag + r'\.(a|so)(\.[0-9]+)*$'
    elif PLATFORM == 'mingw64':
        pattern = r'lib'
    elif PLATFORM == 'windows':
        pattern = linkflag + r'\.dll$'

    return auto_detect_file_regex(AUTO_LIB_DIRS, pattern)

def auto_detect_package(headers, linkflags):
    hs = [auto_detect_header(h) for h in headers]
    ls = [auto_detect_library(l) for l in linkflags]
    hbases = [h[0] for h in hs]
    lbases = [l[0] for l in ls]

    if len(hbases) == 0 and len(headers) != 0:
        print("Could not find requested headers:")
        for h in headers:
            print('  "{}"'.format(h))
        print("in base directories")
        for x in AUTO_INCLUDE_DIRS:
            print('  "{}"'.format(x))
        Exit(1)

    if len(lbases) == 0 and len(linkflags) != 0:
        print("Could not find requested libraries:")
        for l in linkflags:
            print('  "{}"'.format(h))
        print("in base directories")
        for x in AUTO_LIB_DIRS:
            print('  "{}"'.format(x))
        Exit(1)

    if len(set(hbases)) > 1:
        print('Headers found in multiple base directories:')
        print('  requested headers:')
        for h in headers:
            print('    "{}"'.format(h))
        print('  headers found:')
        for x in hs:
            print('    "{}"'.format(x[1]))
        Exit(1)

    if len(set(lbases)) > 1:
        print('Libraries found in multiple base directories:')
        print('  requested link flags: {}'.format(', '.join(linkflags)))
        print('  libraries found:')
        for x in ls:
            print('    "{}"'.format(x[0]))
            for y in x[1]:
                print('      "{}"'.format(y))
        Exit(1)

    return (list(set(hbases)), list(set(lbases)))


def MakeAutoPackageTest(name,
                        headers = None,
                        libs = None,
                        use_extern_c = False):

    def run_test(context):
        context.Message('Checking for {} package... '.format(name))
        idirs, ldirs = auto_detect_package(headers, libs)

        include_src = '\n'.join(['#include <{}>'.format(h) for h in headers])
        if use_extern_c:
            include_src = 'extern "C" {\n' + include_src + '\n}\n'

        main_function_src = "int main(int, char**){\n  return 0;\n}\n"
        src = include_src + '\n' + main_function_src

        rc = False
        try:
            context.env.AppendUnique(LIBS=libs,
                                     CPPPATH=idirs,
                                     LIBPATH=ldirs)
            ret = context.TryLink(src, '.cc')
            rc = ret == 1
            if not rc:
                raise BaseException('linking failed.')

        except BaseException as e:
            context.Message(e.message)
            context.Result('no')
            return rc

        context.Result('yes')
        return rc

    return run_test

class TestExternalCall(object):
    def __init__(self):
        self.command = None
        self.out = None
        self.err = None
        self.returncode = None

    def test(self, command_list):
        try:
            p = subprocess.Popen(command_list,
                                 stdout=subprocess.PIPE,
                                 stderr=subprocess.PIPE)
            self.out, self.err = p.communicate()
            self.out = self.out.decode('utf-8')
            self.err = self.err.decode('utf-8')
            self.returncode = p.returncode
            if self.returncode != 0:
                raise BaseException('Command %s failed with code %d.\n%s' %
                                (command_list, self.returncode, self.err))
            return self.returncode == 0
        except OSError as e:
            print(e)
            Exit(1)

class TestPackageConfig(TestExternalCall):
    def __init__(self, package_config_command):
        super(TestPackageConfig, self).__init__()
        self.cmd = package_config_command

        self.version = None
        self.cflags = []
        self.includedirs = []
        self.libs = []
        self.libdirs = []
        self.lflags = []
        self.prefix = None

    def test_config_script_exists(self):
        return super(TestPackageConfig, self).test([self.cmd, '--version'])

    def get_version(self):
        rc = super(TestPackageConfig, self).test([self.cmd, '--version'])
        self.version = self.out
        return rc

    def get_cflags(self):
        rc = super(TestPackageConfig, self).test([self.cmd, '--cflags'])
        flags = re.split(r'\s+', self.out)
        for f in flags:
            if f[0:2] == '-I':
                self.includedirs.append(f[2:])
            else:
                self.cflags.append(f)
        return rc

    def get_libs(self):
        rc = super(TestPackageConfig, self).test([self.cmd, '--libs'])
        flags = re.split(r'\s+', self.out)
        for f in flags:
            if f[0:2] == '-L':
                self.libdirs.append(f[2:])
            elif f[0:2] == '-l':
                self.libs.append(f[2:])
            else:
                self.lflags.append(f)

        return rc


    def get_prefix(self):
        rc = super(TestPackageConfig, self).test([self.cmd, '--prefix'])
        self.prefix = self.out
        return rc

    def get_all(self):
        fcs = [self.get_version, self.get_cflags, self.get_libs,
               self.get_prefix]
        rc = 0
        for f in fcs:
            rc = f()
            if not rc:
                break
        return rc

    def append_to_environment(self, env):
        env.AppendUnique(CXXFLAGS=self.cflags)
        env.AppendUnique(CFLAGS=self.cflags)
        env.AppendUnique(CPPPATH=self.includedirs)
        env.AppendUnique(LIBS=self.libs)
        env.AppendUnique(LINKFLAGS=self.lflags)
        env.AppendUnique(LIBPATH=self.libdirs)


def MakePkgConfigTest(name,
                      external_command,
                      headers = None,
                      use_extern_c = False):

    def run_test(context):
        context.Message('Checking for {} package using {}... '.format(name,
                                                                      external_command))
        c = TestPackageConfig(external_command)
        rc = False
        try:
            rc = c.get_all()
            if not rc:
                raise BaseException('{} failed'.format(external_command))
            c.append_to_environment(context.env)

            include_src = '\n'.join(['#include <{}>'.format(h) for h in headers])
            if use_extern_c:
                include_src = 'extern "C" {\n' + include_src + '\n}\n'

            main_function_src = "int main(int, char**){\n  return 0;\n}\n"
            src = include_src + '\n' + main_function_src

            ret = context.TryLink(src, '.cc')
            rc = ret == 1
            if not rc:
                raise BaseException('linking failed.')

        except BaseException as e:
            #context.Message(e.message)
            context.Message(str(e))
            context.Result('no')
            return c.returncode

        context.Result('yes')
        return rc

    return run_test


def create_environment():
    """Import or create build environment and allow replacing compiler
     from environment variables if set.
    """
    env = None
    try:
        Import('env')
    except:
        env = Environment()

    for key in ['CC', 'CXX']:
        if key in os.environ:
            env[key] = os.environ[key]
    return env



def CheckCXX11(context):
    cxx11_compatible_source = '''
int main(int, char**){
  return 0;
}
'''
    context.Message('Checking whether `%s\' accepts -std=c++11... ' % \
                    context.env['CXX'])
    #cache_cxxflags = context.env['CXXFLAGS']
    context.env.AppendUnique(CXXFLAGS='-std=c++11')
    ret = context.TryLink(cxx11_compatible_source, '.cc')
    rc = ret == 1
    if rc:
        context.Result('yes')
    else:
        context.Result('no')
    #context.env.Replace(CXXFLAGS = cache_cxxflags)
    return rc


def do_autoconf(env):
    """Using all of the custom tests above, finally to the autoconf step
    here.
    """
    tests = {'CheckCXX11': CheckCXX11,
             'CheckGSL': MakePkgConfigTest('GSL',
                                           'gsl-config',
                                           headers = ['gsl/gsl_version.h'],
                                           use_extern_c = True),
             'CheckHDF5': MakeAutoPackageTest('HDF5',
                                              headers = ['hdf5.h'],
                                              libs = ['hdf5']),
             # 'CheckGLFW3': MakeAutoPackageTest('GLFW3',
             #                                   headers = ['GLFW/glfw3.h'],
             #                                   libs = ['glfw3']),
    }
    conf = Configure(env, custom_tests=tests)

    # check CXX
    if not conf.CheckCXX():
        print('Need C++ compiler.')
        Exit(0)

    using_cxx11 = True
    if not conf.CheckCXX11():
        print('Yo! Your C++ is not at std=c++11 yet, disabling C++11 features.')
        using_cxx11 = False
    Export('using_cxx11')

    # check GSL
    if not conf.CheckGSL():
        print('Need the GSL library and `gsl-config\'.')
        Exit(0)

    # check HDF5
    if not conf.CheckHDF5():
        print('Need the HDF5 library.')
        Exit(0)

    # check GLFW3
    # if not conf.CheckGLFW3():
    #     print('Need the GLFW3 library.')
    #     Exit(0)

    env = conf.Finish()

env = create_environment()
# if not env.GetOption('clean'):
#     do_autoconf(env)
#     SConscript('SConscript')
# else:
#     Export('env')


# need to fix this
do_autoconf(env)

# Export current environment and call SConscript
Export('env')

SConscript('SConscript')

# SConstruct ends here
