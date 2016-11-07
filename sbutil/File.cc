// -*- mode: C++ -*-
// Time-stamp: "2016-11-07 10:59:44 sb"

/*
  file       File.cc
  copyright  (c) Sebastian Blatt 2016

 */

#include "File.hh"

#include <ifstream>
#include <iostream>

std::string load_ascii_file(const std::string& filename){
    std::ifstream in;
    // use bare pointer for buffer because std::string is supposedly
    // not guaranteed to be contiguous below C++11 and
    // std::vector<char> is not less cumbersome to use.
    char* buf = nullptr;
    std::string rc;
    try{
        in.open(filename.c_str());
        in.seekg(0, in.end);
        size_t N = in.tellg();
        in.seekg(0, in.beg);
        buf = new char[N + 1];
        in.read(buf, N);
        buf[N] = '\0';
        in.close();
        rc.assign(buf);
    }
    catch(const std::exception& e){
        if(buf){
            delete[] buf;
        }
        throw;
    }
    delete[] buf;
    return rc;
}

// File.cc ends here
