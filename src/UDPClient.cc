// -*- mode: C++ -*-
// Time-stamp: "2014-06-02 10:29:20 sb"

/*
  file       UDPClient.cc
  copyright  (c) Sebastian Blatt 2012, 2013, 2014

 */

#include "Platform.hh"

#include <sstream>
#include <iomanip>
#include <cstdio>
#include <cstring>

#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#if (SB_UTIL_PLATFORM == SB_UTIL_PLATFORM_OSX)
#include <unistd.h>
#endif

#include "Exception.hh"
#include "UDPClient.hh"
#include "OutputManipulator.hh"

UDPClient::UDPClient(const std::string& server_address_,
                       unsigned short server_port_)
  : fd_socket(0),
    server_address(server_address_),
    server_port(server_port_),
    client_address(0)
{
  memset(&server_socket_address, 0, sizeof(server_socket_address));
  OpenSocket();
}

UDPClient::~UDPClient(){
  CloseSocket();
}

void UDPClient::OpenSocket(){
  if(fd_socket != 0){ // socket already opened
    return;
  }

  if((fd_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
    std::ostringstream os;
    os << "socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) failed\n"
       << strerror(errno);
    throw EXCEPTION(os.str());
  }

  server_socket_address.sin_family = AF_INET;
  server_socket_address.sin_port = htons(server_port);
  if(inet_aton(server_address.c_str(), &server_socket_address.sin_addr) == 0){
    std::ostringstream os;
    os << "inet_aton(" << server_address << ") failed\n"
       << strerror(errno);
    throw EXCEPTION(os.str());
  }
}

void UDPClient::CloseSocket(){
  if(fd_socket){
    close(fd_socket);
    fd_socket = 0;
  }
}

void UDPClient::SendPacket(const std::string& data) const {
  if(fd_socket == 0){
    throw EXCEPTION("Socket not open.");
  }
  if(sendto(fd_socket, data.c_str(), data.size(), 0,
            (const sockaddr*) &server_socket_address,
            sizeof(server_socket_address)) == -1)
  {
    std::ostringstream os;
    os << "sendto(" << server_address << ":" << server_port << ", "
       << "\"" << data << "\" failed\n"
       << strerror(errno);
    throw EXCEPTION(os.str());
  }
}

void UDPClient::SendPacket(const uint32_t* binary_data, size_t length) const {
  if(fd_socket == 0){
    throw EXCEPTION("Socket not open.");
  }
  if(sendto(fd_socket, (const char*)binary_data, sizeof(uint32_t)*length, 0,
            (const sockaddr*) &server_socket_address,
            sizeof(server_socket_address)) == -1)
  {
    std::ostringstream os;
    os << "sendto(" << server_address << ":" << server_port << ", \n";
    for(size_t j=0; j<length; ++j){
      os << hex_form<uint32_t>(binary_data[j]);
      if(j < length - 1){
        os << ",";
      }
      else{
        os << ")";
      }
      os << "\n";
    }
    os << strerror(errno);
    throw EXCEPTION(os.str());
  }
}

uint32_t UDPClient::GetServerIPAddress() const {
  return ntohl(server_socket_address.sin_addr.s_addr);
}

uint16_t UDPClient::GetServerIPPort() const {
  return ntohs(server_socket_address.sin_port);
}



// UDPClient.cc ends here
