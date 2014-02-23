#!/usr/bin/env python
# -*- mode: Python; coding: latin-1 -*-
# Time-stamp: "2014-02-22 23:46:38 sb"

#  file       create_project.py
#  copyright  (c) Sebastian Blatt 2014

import os, sys, subprocess

def run_external(cmd):
  print "run_external: \"%s\"" % (cmd)
  try:
    p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    rc = p.communicate()[0]
    return rc
  except OSError:
    print "Failed running external command."
    return None

if __name__ == "__main__":
  project = sys.argv[1]

  print "Make project directory \"%s\"" % project
  os.mkdir(project)

  # change to project directory
  top_directory = os.getcwd()
  os.chdir(project)

  print "Clone sbutil git repository"
  run_external(['git', 'clone', 'https://github.com/blatts/sbutil.git'])


# create_project.py ends here
