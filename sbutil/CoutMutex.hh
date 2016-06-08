// -*- mode: C++ -*-
// Time-stamp: "2016-02-26 18:48:17 sb"

/*
  file       CoutMutex.hh
  copyright  (c) Sebastian Blatt 2016

  Provide mutexes and a logging interface for std::cout, std::cerr,
  std::clog.

 */


#ifndef COUTMUTEX_HH__CAE719D4_D1A6_11E5_917A_901B0E4C2C6B
#define COUTMUTEX_HH__CAE719D4_D1A6_11E5_917A_901B0E4C2C6B

#include <iostream>
#include <thread>

#include <sbutil/OutputManipulator.hh>

class CoutMutex {
  private:
    std::mutex cout_mutex;
    std::mutex cerr_mutex;
    std::mutex clog_mutex;
    size_t source_width;

  public:
    CoutMutex()
      : source_width(10)
    {}

    void SetSourceWidth(size_t source_width_){
      source_width = source_width_;
    }


    void emit(const std::string& source,
              const std::string& msg,
              const std::string& marker,
              std::ostream& out)
    {
      std::lock_guard<std::mutex> lock(cout_mutex);

      out << marker << " "
          << Timestamp()
          << right_justified<std::string>(source, source_width) << ": "
          << msg << std::endl;
    }

    void cout(const std::string& source,
              const std::string& msg,
              const std::string& marker = "")
    {
      emit(source, msg, marker, std::cout);
    }

    void cerr(const std::string& source,
              const std::string& msg,
              const std::string& marker = "")
    {
      emit(source, msg, marker, std::cerr);
    }

    void clog(const std::string& source,
              const std::string& msg,
              const std::string& marker = "")
    {
      emit(source, msg, marker, std::clog);
    }
};


#endif // COUTMUTEX_HH__CAE719D4_D1A6_11E5_917A_901B0E4C2C6B

// CoutMutex.hh ends here
