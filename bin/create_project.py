#!/usr/bin/env python
# -*- mode: Python; coding: latin-1 -*-
# Time-stamp: "2014-02-23 11:43:17 sb"

#  file       create_project.py
#  copyright  (c) Sebastian Blatt 2014

import os
import os.path
import sys
import subprocess
import time
import re

def run_external(cmd):
  print "run_external: \"%s\"" % (cmd)
  try:
    p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    rc = p.communicate()[0]
    return rc
  except OSError:
    print "Failed running external command."
    return None

def copy_replace_template(ifile, ofile, replacements):
  x = None
  with open(ifile, 'r') as f:
    x = f.read()
  for k, v in replacements.items():
    x = re.sub(r':::%s:::' % k, v, x)
  with open(ofile, 'w') as f:
    f.write(x)

if __name__ == "__main__":
  project = sys.argv[1]

  print "Make project directory \"%s\"" % project
  os.mkdir(project)

  # change to project directory
  top_directory = os.getcwd()
  os.chdir(project)

  print "Clone sbutil git repository"
  run_external(['git', 'clone', 'https://github.com/blatts/sbutil.git'])

  # now have access to templates, copy and substitute
  tm = time.localtime()
  replacements = {
    r'project' : project,
    r'today' : time.strftime('%Y%m%d', tm),
    r'year' : time.strftime('%Y', tm),
    r'author' : 'Sebastian Blatt'
    }

  files = [('main.cc', '%s.cc' % project),
           ('SConstruct', 'SConstruct'),
           ('SConscript', 'SConscript')]

  for x in files:
    copy_replace_template(os.path.join('.','sbutil','templates', x[0]),
                          os.path.join('.', x[1]),
                          replacements)



# create_project.py ends here
