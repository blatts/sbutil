// -*- mode: C++ -*-
// Time-stamp: "2016-02-12 17:41:35 sb"

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

class CoutMutex {
  private:
    std::mutex cout_mutex;
    std::mutex cerr_mutex;
    std::mutex clog_mutex;

  public:
    CoutMutex(){}

    void emit(const std::string& source,
              const std::string& msg,
              const std::string& marker,
              std::ostream& out)
    {
      std::lock_guard<std::mutex> lock(cout_mutex);

      out << marker << " "
          << Timestamp()
          << right_justified<std::string>(source, 10) << ": "
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
