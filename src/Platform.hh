// -*- mode: C++ -*-
// Time-stamp: "2014-02-22 22:34:55 sb"

/*
  file       Platform.hh
  copyright  (c) Sebastian Blatt 2014

  Use compiler macros to decide which platform we are on. The specific
  combinations and the rationale for each is very nicely explained here

    http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system

  Implementation here is limited to win32, linux, and osx.

*/


#ifndef PLATFORM_HH__5E8B7EE0_AF6C_43A9_AD57_7151BF624896
#define PLATFORM_HH__5E8B7EE0_AF6C_43A9_AD57_7151BF624896

#define SBUTIL_PLATFORM_UNKNOWN 0
#define SBUTIL_PLATFORM_LINUX 1
#define SBUTIL_PLATFORM_WIN32 2
#define SBUTIL_PLATFORM_WIN64 3
#define SBUTIL_PLATFORM_OSX 4
#define SBUTIL_PLATFORM_CYGWIN 5

#ifdef SBUTIL_PLATFORM
#undef SBUTIL_PLATFORM
#endif

// Linux
#if defined (__linux__)

#define SBUTIL_PLATFORM SBUTIL_PLATFORM_LINUX

#endif


// OSX
#if defined (__APPLE__) && defined (__MACH__)

#define SBUTIL_PLATFORM SBUTIL_PLATFORM_OSX

#endif


// Windows 32 bit
#if defined (_WIN32)

#define SBUTIL_PLATFORM SBUTIL_PLATFORM_WIN32

#endif

// Windows 64 bit
#if defined (_WIN64)

#define SBUTIL_PLATFORM SBUTIL_PLATFORM_WIN64

#endif

// Cygwin (posix)
#if defined (__CYGWIN__) && !defined(_WIN32)

#define SBUTIL_PLATFORM SBUTIL_PLATFORM_CYGWIN

#endif

// catch all
#if not defined (SBUTIL_PLATFORM)
#define SBUTIL_PLATFORM SBUTIL_PLATFORM_UNKNOWN
#endif

// Query macros

#define SBUTIL_IS_PLATFORM_UNKNOWN (SBUTIL_PLATFORM == SBUTIL_PLATFORM_UNKNOWN)
#define SBUTIL_IS_PLATFORM_LINUX (SBUTIL_PLATFORM == SBUTIL_PLATFORM_LINUX)
#define SBUTIL_IS_PLATFORM_OSX (SBUTIL_PLATFORM == SBUTIL_PLATFORM_OSX)
#define SBUTIL_IS_PLATFORM_CYGWIN (SBUTIL_PLATFORM == SBUTIL_PLATFORM_CYGWIN)
#define SBUTIL_IS_PLATFORM_WIN32 (SBUTIL_PLATFORM == SBUTIL_PLATFORM_WIN32)
#define SBUTIL_IS_PLATFORM_WIN64 (SBUTIL_PLATFORM == SBUTIL_PLATFORM_WIN64)

// "Windows" = _WIN32 || _WIN64
#define SBUTIL_IS_PLATFORM_WINDOWS \
  ((SBUTIL_PLATFORM == SBUTIL_PLATFORM_WIN64) ||  \
   (SBUTIL_PLATFORM == SBUTIL_PLATFORM_WIN32))







#endif // PLATFORM_HH__5E8B7EE0_AF6C_43A9_AD57_7151BF624896

// Platform.hh ends here
