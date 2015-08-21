// -*- mode: C++ -*-
// Time-stamp: "2015-08-20 21:25:19 sb"

/*
  file       Tiff.cc
  copyright  (c) Sebastian Blatt 2009 -- 2015

 */

#include "Tiff.hh"
#include "Exception.hh"

#include <memory>
#include <iostream>
#include <sstream>
#include <tiffio.h>



// simple class used with a TIFF file pointer to be used with an
// auto_ptr so that the file is closed when leaving the scope.
class TiffFile {
  public:
    TIFF* image;
    TiffFile() : image(NULL) {}
    ~TiffFile(){close_file();}
    bool open_file(const std::string& f){
      image = TIFFOpen(f.c_str(),"r");
      return image != NULL;
    }
    void close_file(){
      if(image){
        TIFFClose(image);
      }
      image = NULL;
    }
};


Tiff::Tiff()
  : buffer(), width(0), height(0), bits_per_sample(8)
{}

void Tiff::free_buffer(){
  buffer.clean();
  width = height = 0;
}


Tiff::~Tiff(){
  free_buffer();
}

bool Tiff::load_file(const std::string& f, size_t n){
  std::auto_ptr<TiffFile> p(new TiffFile);

  try{
    if(!p->open_file(f)){
      throw EXCEPTION("Could not open image.");
    }

    // libtiff automatically opens the first frame N=0 when opening a
    // file. If a frame N>0 is requested, need to switch with
    // TIFFSetDirectory(3).
    if(n>0){
      if((TIFFSetDirectory(p->image,(tdir_t)n))==0){
        std::ostringstream os;
        os << "Failed switching to frame " << n << ".";
        throw EXCEPTION(os.str());
      }
    }

    {/* Utterly random. Somehow, libtiff remembers the address of
        bps and sets its value to zero on the next call to
        TIFFGetField(), even with another tag that is successfully
        returned. Use bits_per_sample to save the value for another
        day and make bps a local variable.
      */
      uint16 bps = 0;
      if((TIFFGetField(p->image, TIFFTAG_BITSPERSAMPLE, &bps) == 0)){
        throw EXCEPTION("Undefined bits per sample.");
      }
      if(!(bps == 16 || bps == 8)){
        throw EXCEPTION("This program only supports 8 or 16 bits per sample.");
      }
      bits_per_sample = bps;
    }

    tsize_t strip_size = TIFFStripSize(p->image);
    int strip_max = TIFFNumberOfStrips(p->image);

    unsigned long buffer_size = strip_max * strip_size;
    buffer.allocate(buffer_size/(bits_per_sample/8),bits_per_sample/8);

    unsigned long image_offset = 0;
    char* nude_pointer = static_cast<char*>(buffer.expose());
    for(int s=0; s<strip_max; ++s){
      long res = TIFFReadEncodedStrip(p->image,s,nude_pointer+image_offset,strip_size);
      if(res == -1){
        throw EXCEPTION("Error reading input strip");
      }
      image_offset += res;
    }

    uint16 tiff_width = 0;
    if(TIFFGetField(p->image,TIFFTAG_IMAGEWIDTH,&tiff_width)==0){
      throw EXCEPTION("image width undefined");
    }

    width = tiff_width;
    height = buffer_size/(width*(bits_per_sample/8));

    /*std::cout << "width = " << width << "\n"
              << "height = " << height << "\n"
              << "bufsize = " << buffer_size << "\n"
              << "bits_per_sample = " << bits_per_sample << "\n"
              << "strip_size = " << strip_size << "\n"
              << "strip_max = " << strip_max
              << std::endl;
    */
  }
  catch(Exception& e){
    free_buffer();
    std::cerr << "Tiff::load_image : " << e << std::endl;
    return false;
  }

  return true;
}

unsigned short Tiff::get(size_t x, size_t y) const {
  if(x>width-1 || y>height-1){
    std::cerr << "Tiff:get : index (" << x << "," << y << ") out of bounds ("
              << width-1 << "," << height-1 << ")" << std::endl;
    return 0;
  }
  const unsigned char* address = buffer.const_get(y*width+x);
  unsigned short rc = *address;
  if(buffer.get_chunk()>1){
    rc |= (*(address+1)) << 8;
  }
  return rc;
}

size_t Tiff::get_number_of_frames(const std::string& f) {
#if __cplusplus < 201100L
  std::auto_ptr<TiffFile> p(new TiffFile);
#else
  std::unique_ptr<TiffFile> p(new TiffFile);
#endif
  size_t rc = 0u;
  try{
    if(!p->open_file(f)){
      throw EXCEPTION("Could not open image.");
    }
    do{
      ++rc;
    } while(TIFFReadDirectory(p->image));
  }
  catch(const Exception& e){
    std::cerr << "Tiff::load_image : " << e << std::endl;
    return 0;
  }
  return rc;
}


// tiff.cc ends here
