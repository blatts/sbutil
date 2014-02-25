// -*- mode: C++ -*-
// Time-stamp: "2013-06-18 10:24:17 sb"

/*
  file       UDPServer.hh
  copyright  (c) Sebastian Blatt 2012, 2013

 */


#ifndef UDPSERVER_HH__F52FD72C_DA19_4450_A2DA_46ADADB36303
#define UDPSERVER_HH__F52FD72C_DA19_4450_A2DA_46ADADB36303

#include "UDPPacket.hh"

// Windows maximal UDP packet length see
// http://stackoverflow.com/questions/1098897/what-is-the-largest-safe-udp-packet-size-on-the-internet
#define MAX_UDP_PACKET_LENGTH 65507

// Wait before closing server socket to avoid TIME_WAIT errors
#define CLOSE_SOCKET_DELAY_SECONDS 2

// Use SO_REUSEADDR?
#define USE_SO_REUSEADDR 1

class UDPServer{
  private:
    int fd_socket;
  protected:
    unsigned short server_port;
    struct sockaddr_in server_socket_address;

  public:
    UDPServer(unsigned short port);
    ~UDPServer();

    void OpenSocket();
    void CloseSocket();

    UDPPacket ReceiveBlocking();
};



#endif // UDPSERVER_HH__F52FD72C_DA19_4450_A2DA_46ADADB36303

// UDPServer.hh ends here
