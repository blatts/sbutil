// -*- mode: C++ -*-
// Time-stamp: "2012-02-14 11:56:20 sb"

/*
  file       UDPClient.hh
  copyright  (c) Sebastian Blatt 2012

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
    uint32_t client_address; // resolved local address of client

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
