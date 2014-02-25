// -*- mode: C++ -*-
// Time-stamp: "2014-02-25 16:24:06 sb"

/*
  file       UDPPacket.cc
  copyright  (c) Sebastian Blatt 2012, 2013, 2014

 */

#include "UDPPacket.hh"
#include "OutputManipulator.hh"

#include <arpa/inet.h>

UDPPacket::UDPPacket()
  : address(""),
    port(0),
    data("")
{}

void UDPPacket::ParseAddress(const sockaddr_in& socket_address){
  address = inet_ntoa(socket_address.sin_addr);
  port = ntohs(socket_address.sin_port);
}

std::ostream& UDPPacket::Represent(std::ostream& out) const {
  out << "UDP Packet from " << address << ":" << port << "\n"
      << "Packet data follows:\n";
  static const size_t w = 10;
  for(size_t i=0; i<data.size(); ++i){
    if(i % w == 0){
      out << right_justified<size_t>(i, 6) << "  ";
    }
    out << hex_form<uint8_t>(static_cast<uint8_t>(data[i]));
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
