// -*- mode: C++ -*-
// Time-stamp: "2014-11-13 14:48:56 sb"

/*
  file       ProgressIndicator.hh
  copyright  (c) Sebastian Blatt 2012, 2013, 2014

 */


#ifndef PROGRESSINDICATOR_HH__4D3DDF44_D63B_4C2D_8A5C_72426F719D9C
#define PROGRESSINDICATOR_HH__4D3DDF44_D63B_4C2D_8A5C_72426F719D9C

#include "Timestamp.hh"

#include <iostream>

class ProgressIndicator {
  private:
    size_t line_width;
    size_t percent_width;
    Timestamp start_time;
    size_t current_position;
  public:
    ProgressIndicator();

    // Emits '\r' to update progress bar. If OUT is not a terminal
    // capable of handling carriage return correctly, this will
    // produce a lot of lines.
    std::ostream& Show(std::ostream& out, size_t i, size_t N);
};


#endif // PROGRESSINDICATOR_HH__4D3DDF44_D63B_4C2D_8A5C_72426F719D9C

// ProgressIndicator.hh ends here
