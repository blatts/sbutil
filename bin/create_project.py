#!/usr/bin/env python
# -*- mode: Python; coding: latin-1 -*-
# Time-stamp: "2014-02-23 00:02:40 sb"

#  file       create_project.py
#  copyright  (c) Sebastian Blatt 2014

import os, sys, subprocess, re, os.path

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
  replacements = {
    r'project' : project,
    r'today' : '19990909',
    r'year' : '2041'
    }

  copy_replace_template(os.path.join('.','sbutil','templates','main.cc'),
                        os.path.join('.','%.cc' % project),
                        replacements)



# create_project.py ends here
