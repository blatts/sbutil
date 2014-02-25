// -*- mode: C++ -*-
// Time-stamp: "2012-08-13 17:21:41 sb"

/*
  file       UDPServer.cc
  copyright  (c) Sebastian Blatt 2012

 */

#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstring>

#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "UDPServer.hh"
#include "Exception.hh"

UDPServer::UDPServer(unsigned short port)
  : fd_socket(0),
    server_port(port)
{
  memset((void*)&server_socket_address, 0, sizeof(server_socket_address));
  OpenSocket();
}

UDPServer::~UDPServer(){
  CloseSocket();
}

void UDPServer::OpenSocket(){
  if(fd_socket) { // socket already opened
    return;
  }
  if((fd_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
    std::ostringstream os;
    os << "socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) failed\n"
       << strerror(errno);
    throw EXCEPTION(os.str());
  }

#if USE_SO_REUSEADDR != 0
  // use SO_REUSEADDR to avoid hanging in this state
  // http://hea-www.harvard.edu/~fine/Tech/addrinuse.html
  int tr = 0;
  socklen_t l = sizeof(tr);
  if(getsockopt(fd_socket, SOL_SOCKET, SO_REUSEADDR, (void*)&tr, &l) == -1){
    std::ostringstream os;
    os << "getsockopt(SOL_SOCKET, SO_REUSEADDR) failed\n"
       << strerror(errno);
    throw EXCEPTION(os.str());
  }
  // set SO_REUSEADDR if not set
  if(tr != SO_REUSEADDR){
    tr = 1;
    if(setsockopt(fd_socket, SOL_SOCKET, SO_REUSEADDR, &tr, sizeof(tr)) == -1){
      std::ostringstream os;
      os << "setsockopt(SOL_SOCKET, SO_REUSEADDR, 1, sizeof(int)) failed\n"
         << strerror(errno);
      throw EXCEPTION(os.str());
    }
  }
#endif

  server_socket_address.sin_family = AF_INET;
  server_socket_address.sin_port = htons(server_port);
  server_socket_address.sin_addr.s_addr = htonl(INADDR_ANY);

  if(bind(fd_socket, (const sockaddr*)&server_socket_address,
          sizeof(server_socket_address)) == -1)
  {
    std::ostringstream os;
    os << "bind(INADDR_ANY:" << server_port << ") failed\n"
       << strerror(errno);
    throw EXCEPTION(os.str());
  }
}

void UDPServer::CloseSocket(){
  if(fd_socket){
 #if CLOSE_SOCKET_DELAY_SECONDS > 0
    sleep(CLOSE_SOCKET_DELAY_SECONDS);
 #endif
    close(fd_socket);
    fd_socket = 0;
  }
}

UDPPacket UDPServer::ReceiveBlocking(){
  static char buf[MAX_UDP_PACKET_LENGTH];
  if(fd_socket == 0){ // socket not open
    throw EXCEPTION("Socket not opened.");
  }

  sockaddr_in client_socket_address;
  socklen_t length = sizeof(client_socket_address);
  ssize_t n_bytes = recvfrom(fd_socket, buf, MAX_UDP_PACKET_LENGTH, 0,
                             (sockaddr*) &client_socket_address,
                             &length);
  if(n_bytes == -1){
    std::ostringstream os;
    os << "recvfrom() failed\n" << strerror(errno);
    throw EXCEPTION(os.str());
  }
  UDPPacket packet;
  packet.ParseAddress(client_socket_address);
  packet.data.resize(n_bytes);
  std::copy(buf, buf+n_bytes, packet.data.begin());
  return packet;
}


// UDPServer.cc ends here
