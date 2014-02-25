// -*- mode: C++ -*-
// Time-stamp: "2012-02-16 19:08:00 sb"

/*
  file       UDPPacket.hh
  copyright  (c) Sebastian Blatt 2012

 */


#ifndef UDPPACKET_HH__FB904DDC_A654_4547_9553_71C4C52F4F07
#define UDPPACKET_HH__FB904DDC_A654_4547_9553_71C4C52F4F07

#include <string>
#include <stdint.h>
#include <netinet/in.h>
#include "Representable.hh"

class UDPPacket : public Representable {
  private:
  public:
    std::string address;
    unsigned short port;
    std::string data;

    UDPPacket();
    void ParseAddress(const sockaddr_in& socket_address);
    std::ostream& Represent(std::ostream& out) const;
};

#endif // UDPPACKET_HH__FB904DDC_A654_4547_9553_71C4C52F4F07

// UDPPacket.hh ends here
