// -*- mode: C++ -*-
// Time-stamp: "2016-02-12 16:01:52 sb"

/*
  file       UDPClient.hh
  copyright  (c) Sebastian Blatt 2012 -- 2016

  Provides a simple wrapper around a "connectionless" socket that is
  bound to an IPV4 client_address at client_port. The socket can be
  used to receive UDP packets using recvfrom.

  Idea: instantiate one of these and call ReceiveBlocking.

*/


#ifndef UDPCLIENT_HH__F52FD72C_DA19_4450_A2DA_46ADADB36303
#define UDPCLIENT_HH__F52FD72C_DA19_4450_A2DA_46ADADB36303

#include "UDPPacket.hh"
#include <netinet/in.h>

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
    unsigned long timeout_microsecond;

  protected:
    unsigned short client_port;
    struct sockaddr_in client_socket_address;

  public:
    UDPClient(unsigned short port);
    virtual ~UDPClient();

    void OpenSocket();
    void CloseSocket();

    void SetTimeout(unsigned long timeout_microsecond);

    // Since receiving of packets over the network can produce errors
    // that are not the result of programmer brain damage, we can turn
    // off the usual exception throwing on errors by setting nothrow =
    // true to avoid unwinding the stack every time something
    // unexpected happens.
    //
    typedef enum {
      OK,            // everything worked
      TIMEOUT,       // timeout was set, and socket timed out
      ERR_SOCKET,    // socket not open
      ERR_RECVFROM,  // other error from recvfrom
      ERR_SERIALIZE  // error from UDPPacket::Deserialize
      } receive_state_t;

    receive_state_t ReceiveBlocking(UDPPacket& p, bool nothrow = false);
};



#endif // UDPCLIENT_HH__F52FD72C_DA19_4450_A2DA_46ADADB36303

// UDPClient.hh ends here
