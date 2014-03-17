/* -*- mode: C++ -*- */
/* Time-stamp: "2014-02-26 14:14:06 sb" */

/*
  file       Tiff.hh
  copyright  (c) Sebastian Blatt 2009 -- 2014

  Wrapper class to support loading 8 bit and 16 bit grayscale TIFF
  files via libtiff. Load file as respective number of bits, but
  expose unsigned short datatype to the outside world only.

  Might be a bit slow, since get() becomes more expensive.

*/


#ifndef TIFF_HH__10D20E5F_9EA0_45CC_86BC_53CEDC5778FD
#define TIFF_HH__10D20E5F_9EA0_45CC_86BC_53CEDC5778FD

#include <string>
#include "VariableBuffer.hh"

class Tiff {
  private:
    // make non-copyable for now
    Tiff(const Tiff&){}
    Tiff& operator=(const Tiff&){return *this;}

  public:
    Tiff();
    ~Tiff();
    size_t get_width() const {return width;} // Width of image in pixels.
    size_t get_height() const {return height;} // Height of image in pixels.
    size_t get_area() const {return width*height;} // Area of image in pixels^2.
    size_t get_size() const {return width*height*bits_per_sample/8;} // Size of image in bytes.
    size_t get_max_val() const {return ((1<<bits_per_sample)-1);} // Maximal image amplitude allowed by data type.

    // Load TIFF frame N from file F, where N is the TIFF directory
    // number. See TIFFReadDirectory(3) for more information.
    bool load_file(const std::string& f, size_t n=0);

    // Get image amplitude at coordinates X and Y.
    unsigned short get(size_t x, size_t y) const;

    // Determine the number of frames = number of TIFF directories in
    // TIFF file F. Return 0 on error.
    static size_t get_number_of_frames(const std::string& f);

  private:
    void free_buffer();
    VariableBuffer buffer;
    size_t width;
    size_t height;
    size_t bits_per_sample;
};

#endif /* TIFF_HH__10D20E5F_9EA0_45CC_86BC_53CEDC5778FD */

/* Tiff.hh ends here */
