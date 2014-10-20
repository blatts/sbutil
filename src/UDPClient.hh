// -*- mode: C++ -*-
// Time-stamp: "2014-10-20 14:55:54 sb"

/*
  file       UDPClient.hh
  copyright  (c) Sebastian Blatt 2012, 2013, 2014

 */


#ifndef UDPCLIENT_HH__B7CA5A67_6C37_42DF_A4F5_9D21373080F8
#define UDPCLIENT_HH__B7CA5A67_6C37_42DF_A4F5_9D21373080F8

#include "UDPPacket.hh"
#include <stdint.h>

class UDPClient {
  private:
    int fd_socket;
    std::string server_address;
    unsigned short server_port;
    struct sockaddr_in server_socket_address;

  public:
    UDPClient(const std::string& server_address_,
               unsigned short server_port_);
    ~UDPClient();

    void OpenSocket();
    void CloseSocket();
    void SendPacket(const std::string& data) const;
    void SendPacket(const uint32_t* binary_data, size_t length) const;
    uint32_t GetServerIPAddress() const;
    std::string GetServerIPAddressString() const {return server_address;}
    uint16_t GetServerIPPort() const;
};

#endif // UDPCLIENT_HH__B7CA5A67_6C37_42DF_A4F5_9D21373080F8

// UDPClient.hh ends here
