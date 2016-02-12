// -*- mode: C++ -*-
// Time-stamp: "2016-02-12 16:22:06 sb"

/*
  file       UDPPacket.cc
  copyright  (c) Sebastian Blatt 2012 -- 2016

 */

#include "UDPPacket.hh"
#include "OutputManipulator.hh"

const UDPPacket& UDPPacket::Assign(const UDPPacket& x) {
  data = x.GetData();
  address = x.GetAddress();
  port = x.GetPort();
  return *this;
}

const UDPPacket& UDPPacket::Assign(const std::vector<uint8_t>& data_,
                                   const std::string& address_,
                                   unsigned short port_)
{
  data = data_;
  address = address_;
  port = port_;
  return *this;
}

void UDPPacket::Clear(){
  data.clear();
  address.clear();
  port = 0;
}



std::ostream& UDPPacket::Represent(std::ostream& out) const {
  out << "UDP Packet";
  if(port != 0 && !address.empty()){
    out << " with address " << address << ":" << port << "\n";
  }
  else{
    out << " without address information.\n";
  }
  out << "Packet data:\n";

  static const size_t w = 10;
  for(size_t i=0; i<data.size(); ++i){
    if(i % w == 0){
      out << right_justified<size_t>(i, 6) << "  ";
    }
    out << hex_form<uint8_t>(data[i]);
    if(i % w == w-1){
      out << "\n";
    }
    else{
      out << " ";
    }
  }
  return out;
}


// UDPPacket.cc ends here
