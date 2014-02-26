// -*- mode: C++ -*-
// Time-stamp: "2014-02-26 14:12:36 sb"

/*
  file       VariableBuffer.hh
  copyright  (c) Sebastian Blatt 2009 -- 2014

  This file is licensed under the Creative Commons Attribution 3.0
  Unported License (see http://creativecommons.org/licenses/by/3.0).

 */


#ifndef VARIABLEBUFFER_HH__A92669FD_8C48_406F_B02B_0E324BBB3260
#define VARIABLEBUFFER_HH__A92669FD_8C48_406F_B02B_0E324BBB3260

class VariableBuffer {
  public:
    VariableBuffer()
      : data(NULL), chunk(0) {}
    virtual ~VariableBuffer(){clean();}
    void allocate(size_t size_, size_t chunk_){
      clean();
      data = new unsigned char[size_*chunk_];
      chunk = chunk_;
    }
    void clean() {
      if(data){
        delete[] data;
        data = NULL;
      }
      chunk = 0;
    }
    void* expose() {return static_cast<void*>(data);}
    const void* const_expose() const {return static_cast<void*>(data);}
    unsigned char* get(size_t i) const {return &(data[i*chunk]);}
    const unsigned char* const_get(size_t i) const {return &(data[i*chunk]);}
    const size_t get_chunk() const {return chunk;}
  private:
    unsigned char* data;
    size_t chunk;
};


#endif // VARIABLEBUFFER_HH__A92669FD_8C48_406F_B02B_0E324BBB3260

// VariableBuffer.hh ends here
