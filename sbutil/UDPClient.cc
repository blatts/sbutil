// -*- mode: C++ -*-
// Time-stamp: "2016-02-12 16:28:21 sb"

/*
  file       UDPClient.cc
  copyright  (c) Sebastian Blatt 2012 -- 2016

*/

#include <sbutil/Platform.hh>

#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstring>

#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#if (SB_UTIL_PLATFORM == SB_UTIL_PLATFORM_OSX)
#include <unistd.h>
#endif

#include <sbutil/UDPClient.hh>
#include <sbutil/Exception.hh>

UDPClient::UDPClient(unsigned short port)
  : fd_socket(0),
    timeout_microsecond(0),
    client_port(port)
{
  memset((void*)&client_socket_address, 0, sizeof(client_socket_address));
  OpenSocket();
}

UDPClient::~UDPClient(){
  CloseSocket();
}

void UDPClient::OpenSocket(){
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

  client_socket_address.sin_family = AF_INET;
  client_socket_address.sin_port = htons(client_port);
  client_socket_address.sin_addr.s_addr = htonl(INADDR_ANY);

  if(bind(fd_socket, (const sockaddr*)&client_socket_address,
          sizeof(client_socket_address)) == -1)
  {
    std::ostringstream os;
    os << "bind(INADDR_ANY:" << client_port << ") failed\n"
       << strerror(errno);
    throw EXCEPTION(os.str());
  }
}

void UDPClient::CloseSocket(){
  if(fd_socket){
 #if CLOSE_SOCKET_DELAY_SECONDS > 0
    sleep(CLOSE_SOCKET_DELAY_SECONDS);
 #endif
    close(fd_socket);
    fd_socket = 0;
  }
}

void UDPClient::SetTimeout(unsigned long timeout_microsecond_){
  timeout_microsecond = timeout_microsecond_;

  struct timeval tv;
  memset(&tv, 0, sizeof(tv));
  tv.tv_sec = timeout_microsecond / 1000000;
  tv.tv_usec = timeout_microsecond % 1000000;

  if(setsockopt(fd_socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1){
    std::ostringstream os;
    os << "setsockopt(SOL_SOCKET, SO_RCVTIMEO, " << tv.tv_sec << " s, "
       << tv.tv_usec << " us) failed\n"
       << strerror(errno);
    throw EXCEPTION(os.str());
  }
}


UDPClient::receive_state_t UDPClient::ReceiveBlocking(UDPPacket& p, bool nothrow){
  static uint8_t buf[MAX_UDP_PACKET_LENGTH];

  if(fd_socket == 0){ // socket not open
    if(nothrow){
      return receive_state_t::ERR_SOCKET;
    }
    else{
      throw EXCEPTION("Socket not opened.");
    }
  }

  sockaddr_in sender_socket_address;
  memset(&sender_socket_address, 0, sizeof(sender_socket_address));
  socklen_t length = sizeof(sender_socket_address);
  ssize_t n_bytes = recvfrom(fd_socket, buf, MAX_UDP_PACKET_LENGTH, 0,
                             (sockaddr*) &sender_socket_address,
                             &length);

  // If the socket allows timeout, signal this.
  if(timeout_microsecond > 0 && n_bytes == -1 && errno == EAGAIN){
    return receive_state_t::TIMEOUT;
  }

  // FIXME: Should add all possible failure modes of recvfrom() in a
  // transparent way. Need to switch on value of errno to do this.
  if(n_bytes == -1){
    if(nothrow){
      return receive_state_t::ERR_RECVFROM;
    }
    else{
      std::ostringstream os;
      os << "recvfrom() failed\n" << strerror(errno);
      throw EXCEPTION(os.str());
    }
  }

  p.SetData(buf, n_bytes);
  p.SetAddress(inet_ntoa(sender_socket_address.sin_addr));
  p.SetPort(ntohs(sender_socket_address.sin_port));
  // std::cerr << "ReceiveBlocking: " << std::string(buf, buf+n_bytes) << std::endl;
  // std::cerr << "ReceiveBlocking: " << p << std::endl;

  if(!p.Deserialize(nothrow)){
    return receive_state_t::ERR_SERIALIZE;
  }

  return receive_state_t::OK;
}


// UDPClient.cc ends here
