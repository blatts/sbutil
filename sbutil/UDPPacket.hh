// -*- mode: C++ -*-
// Time-stamp: "2016-02-12 15:55:25 sb"

/*
  file       UDPPacket.hh
  copyright  (c) Sebastian Blatt 2012 -- 2016

  Base class for UDP Packets that contains a binary data vector and
  address:port information for IPV4.

  Subclass your specialized UDP packets from this and override the
  Serialize() member function to produce an updated data member just
  before sending the packet with UDPServer::SendPacket.

  In the same way, you must override Deserialize() which is called by
  UDPClient::ReceiveBlocking to raise your special format from binary.

  You must make sure *by* *hand* that Serialize/Deserialize assume
  network byte order (most significant byte first).

  Make sure that your child classes remain copyable, and call the
  Assign functions in the derived class!

 */


#ifndef UDPPACKET_HH__FB904DDC_A654_4547_9553_71C4C52F4F07
#define UDPPACKET_HH__FB904DDC_A654_4547_9553_71C4C52F4F07

#include <string>
#include <vector>
#include <stdint.h>
#include <sbutil/Representable.hh>

class UDPPacket : public Representable {
  protected:
    std::vector<uint8_t> data;

    // Note: these will be set automatically by
    // UDPClient::ReceiveBlocking to contain the *source* IP address and
    // port. For UDPServer::SendPacket, these are used as the *target*
    // address and port.
    std::string address;
    unsigned short port;

  public:
    const UDPPacket& Assign(const UDPPacket& x);
    const UDPPacket& Assign(const std::vector<uint8_t>& data_,
                            const std::string& address_ = "",
                            unsigned short port_ = 0);
    void Clear();


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

    virtual ~UDPPacket(){}

    const std::vector<uint8_t>& GetData() const {return data;}
    void SetData(const std::vector<uint8_t>& data_){data = data_;}
    void SetData(const uint8_t* buf, size_t length){
      data.clear();
      std::copy(buf, buf+length, std::back_inserter(data));
    }
    const std::string& GetAddress() const {return address;}
    void SetAddress(const std::string& address_) {address = address_;}
    unsigned short GetPort() const {return port;}
    void SetPort(unsigned short port_) {port = port_;}

    std::ostream& Represent(std::ostream& out) const;

    // These are the hooks to override if you have a customized UDP
    // Packet.
    //
    // The Serialize hook should update the UDPPacket::data member
    // from the properties of your child class. For instance, if you
    // have defined some custom data format for your specialized UDP
    // packets, this function will be called by UDPServer::SendPacket
    // to serialize your data.
    //
    // Deserialize() must do the inverse of Serialize and is called by
    // UDPClient::ReceiveBlocking.
    //
    // Both functions should return a bool to indicate parsing errors
    // and should respect the nothrow argument.
    //
    virtual bool Serialize(bool nothrow = false) {(void)nothrow; return true;}
    virtual bool Deserialize(bool nothrow = false) {(void)nothrow; return true;}
};

#endif // UDPPACKET_HH__FB904DDC_A654_4547_9553_71C4C52F4F07

// UDPPacket.hh ends here
