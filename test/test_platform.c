/* -*- mode: C -*- */
/* Time-stamp: "2018-03-14 15:19:38 srlab" */

/*
  file       test_platform.c
  author     Sebastian Blatt
  created    2018-03-14 15:00:01

  Copyright (C) 2018 Sebastian Blatt

 */

#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

int main(int argc, char** argv){

 #ifdef __CYGWIN__
    printf("%s\n", "__CYGWIN__");
 #endif

 #ifdef __CYGWIN32__
    printf("%s\n", "__CYGWIN32__");
 #endif

 #ifdef __MINGW32__
    printf("%s\n", "__MINGW32__");
 #endif

 #ifdef __MINGW64__
    printf("%s\n", "__MINGW64__");
 #endif

 #ifdef unix
    printf("%s\n", "unix");
 #endif

 #ifdef __unix
    printf("%s\n", "__unix");
 #endif

 #ifdef __unix__
    printf("%s\n", "__unix__");
 #endif

 #ifdef WIN32
    printf("%s\n", "WIN32");
 #endif

 #ifdef _WIN32
    printf("%s\n", "_WIN32");
 #endif

 #ifdef __WIN32
    printf("%s\n", "__WIN32");
 #endif

 #ifdef __WIN32__
    printf("%s\n", "__WIN32__");
 #endif

 #ifdef WIN64
    printf("%s\n", "WIN64");
 #endif

 #ifdef _WIN64
    printf("%s\n", "_WIN64");
 #endif

 #ifdef __WIN64
    printf("%s\n", "__WIN64");
 #endif

 #ifdef __WIN64__
    printf("%s\n", "__WIN64__");
 #endif

 #ifdef WINNT
    printf("%s\n", "WINNT");
 #endif

 #ifdef __WINNT
    printf("%s\n", "__WINNT");
 #endif

 #ifdef __WINNT__
    printf("%s\n", "__WINNT__");
 #endif

    return 0;
}

/* test_platform.c ends here */
