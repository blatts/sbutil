// -*- mode: C++ -*-
// Time-stamp: "2016-02-12 08:05:25 sb"

/*
  file       UDPServer.hh
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

    void SendPacket(const char* data, size_t length) const;
    void SendPacket(const std::string& data) const; // careful with embedded zeros!

    template<typename T>
    void SendPacket(const T* binary_data, size_t length) const {
      SendPacket(reinterpret_cast<const char*>(binary_data),
                 sizeof(T) * length);
    }

    template<typename T>
    void SendPacket(const std::vector<T>& binary_data) const{
      SendPacket(reinterpret_cast<const char*>(&(binary_data[0])),
                 sizeof(T) * binary_data.size());
    }

    void SendPacket(const UDPPacket& packet) const;

    uint32_t GetServerIPAddress() const;
    std::string GetServerIPAddressString() const {return server_address;}
    uint16_t GetServerIPPort() const;
};

#endif // UDPSERVER_HH__B7CA5A67_6C37_42DF_A4F5_9D21373080F8

// UDPServer.hh ends here
