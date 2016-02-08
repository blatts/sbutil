// -*- mode: C++ -*-
// Time-stamp: "2016-02-08 15:51:51 sb"

/*
  file       UDPClient.hh
  copyright  (c) Sebastian Blatt 2012 -- 2016

*/


#ifndef UDPCLIENT_HH__F52FD72C_DA19_4450_A2DA_46ADADB36303
#define UDPCLIENT_HH__F52FD72C_DA19_4450_A2DA_46ADADB36303

#include "UDPPacket.hh"

// Windows maximal UDP packet length see
// http://stackoverflow.com/questions/1098897/what-is-the-largest-safe-udp-packet-size-on-the-internet
#define MAX_UDP_PACKET_LENGTH 65507

// Wait before closing server socket to avoid TIME_WAIT errors
#define CLOSE_SOCKET_DELAY_SECONDS 2

// Use SO_REUSEADDR?
#define USE_SO_REUSEADDR 1

class UDPClient{
  private:
    int fd_socket;
  protected:
    unsigned short server_port;
    struct sockaddr_in server_socket_address;

  public:
    UDPClient(unsigned short port);
    ~UDPClient();

    void OpenSocket();
    void CloseSocket();

    UDPPacket ReceiveBlocking();
};



#endif // UDPCLIENT_HH__F52FD72C_DA19_4450_A2DA_46ADADB36303

// UDPClient.hh ends here
