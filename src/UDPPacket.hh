// -*- mode: C++ -*-
// Time-stamp: "2016-02-12 11:39:14 sb"

/*
  file       UDPPacket.hh
  copyright  (c) Sebastian Blatt 2012 -- 2016

 */


#ifndef UDPPACKET_HH__FB904DDC_A654_4547_9553_71C4C52F4F07
#define UDPPACKET_HH__FB904DDC_A654_4547_9553_71C4C52F4F07

#include <string>
#include <vector>
#include <stdint.h>
#include "Representable.hh"

class UDPPacket : public Representable {
  protected:
    std::vector<uint8_t> data;

    // Members will only be used when receiving from a socket.
    std::string address;
    unsigned short port;

  public:
    const UDPPacket& Assign(const UDPPacket& x);
    const UDPPacket& Assign(const std::vector<uint8_t>& data_,
                            const std::string& address_ = "",
                            unsigned short port_ = 0);

    UDPPacket()
      : data(), address(), port(0)
    {}
    UDPPacket(const std::vector<uint8_t>& data_,
              const std::string& address_ = "",
              unsigned short port_ = 0)
      : data(data_), address(address_), port(port_)
    {
      Assign(data_, address_, port_);
    }

    UDPPacket(const UDPPacket& x) : data(), address(), port(0) {Assign(x);}
    const UDPPacket& operator=(const UDPPacket& x) {return Assign(x);}

    const std::vector<uint8_t>& GetData() const {return data;}
    void SetData(const std::vector<uint8_t>& data_){data = data_;}
    void SetData(const uint8_t* buf, size_t length){
      data.clear();
      std::copy(buf, buf+length, std::back_inserter(data));
    }
    const std::string& GetAddress() const {return address;}
    void SetAddress(const std::string& address_) {address = address_;}
    unsigned short GetPort() const {return port};
    void SetPort(unsigned short port_) {port = port_};


    std::ostream& Represent(std::ostream& out) const;
};

#endif // UDPPACKET_HH__FB904DDC_A654_4547_9553_71C4C52F4F07

// UDPPacket.hh ends here
