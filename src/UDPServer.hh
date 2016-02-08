// -*- mode: C++ -*-
// Time-stamp: "2016-02-08 15:50:54 sb"

/*
  file       UDPClient.hh
  copyright  (c) Sebastian Blatt 2012 -- 2016

*/


#ifndef UDPSERVER_HH__B7CA5A67_6C37_42DF_A4F5_9D21373080F8
#define UDPSERVER_HH__B7CA5A67_6C37_42DF_A4F5_9D21373080F8

#include "UDPPacket.hh"
#include <stdint.h>

class UDPServer {
  private:
    int fd_socket;
    std::string server_address;
    unsigned short server_port;
    struct sockaddr_in server_socket_address;

  public:
    UDPServer(const std::string& server_address_,
              unsigned short server_port_);
    ~UDPServer();

    void OpenSocket();
    void CloseSocket();
    void SendPacket(const std::string& data) const;
    void SendPacket(const uint32_t* binary_data, size_t length) const;
    uint32_t GetServerIPAddress() const;
    std::string GetServerIPAddressString() const {return server_address;}
    uint16_t GetServerIPPort() const;
};

#endif // UDPSERVER_HH__B7CA5A67_6C37_42DF_A4F5_9D21373080F8

// UDPServer.hh ends here
