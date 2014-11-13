// -*- mode: C++ -*-
// Time-stamp: "2014-11-13 15:28:51 sb"

/*
  file       ProgressIndicator.cc
  copyright  (c) Sebastian Blatt 2012, 2013, 2014

 */

#include "ProgressIndicator.hh"
#include "OutputManipulator.hh"

#include <sstream>
#include <string>


ProgressIndicator::ProgressIndicator()
  : line_width(50),
    percent_width(6),
    start_time(),
    current_position(0)
{}


std::ostream& ProgressIndicator::Show(std::ostream& out, size_t i, size_t N) {
  if(i == 0){
    current_position = 0;
    start_time.Now();
  }

  double p = ((double)(i+1))/((double)N);


  if(p > ((double)current_position)/((double)line_width)){
    out << '\r' << right_justified<int>((int)(100.0 * p), percent_width-2) << "% "
        << std::string(current_position+1, '|')
        << std::string(line_width-current_position-1, '.');
    out.flush();
    ++current_position;
  }

  if(i == N - 1){
    RelativeTime dt = Timestamp() - start_time;

    std::ostringstream os;
    os << "  calculation took ";
    dt.RepresentPretty(os);
    os << ".";

    out << '\r' << os.str();
    size_t l = os.str().size();
    if(l < line_width + percent_width){
      out << std::string(line_width + percent_width - l, ' ') << std::endl;
    }
  }
  return out;
}

// ProgressIndicator.cc ends here
