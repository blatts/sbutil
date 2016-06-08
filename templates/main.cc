// -*- mode: C++ -*-
// Time-stamp: "2016-06-08 14:10:55 sb"

/*
  file       main.cc
  copyright  (c) :::author::: :::year:::

 */

#define PROGRAM_NAME ":::project:::"
#define PROGRAM_DESCRIPTION "My new program"
#define PROGRAM_COPYRIGHT "Copyright (C) :::author::: :::year:::"
#define PROGRAM_VERSION ":::today:::"

#define DEBUG 1

#include <iostream>

#include <sbutil/CommandLine.hh>

static const char* program_options[] =
{
  "Output file", "output", "o", "test.txt",
  };

int main(int argc, char** argv)
{
  CommandLine cl(argc, argv);
  DWIM_CommandLine(cl,
                   PROGRAM_NAME,
                   PROGRAM_DESCRIPTION,
                   PROGRAM_VERSION,
                   PROGRAM_COPYRIGHT,
                   program_options,
                   sizeof(program_options)/sizeof(char*)/4);

  return 0;
}

// :::project:::.cc ends here
