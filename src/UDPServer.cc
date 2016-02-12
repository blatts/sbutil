// -*- mode: C++ -*-
// Time-stamp: "2016-02-12 16:03:01 sb"

/*
  file       UDPServer.cc
  copyright  (c) Sebastian Blatt 2012 -- 2016

 */

#include "Platform.hh"

#include <sstream>
#include <iomanip>
#include <cstdio>
#include <cstring>

#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#if SBUTIL_IS_PLATFORM_OSX || SBUTIL_IS_PLATFORM_LINUX
#include <unistd.h>
#endif

#include "Exception.hh"
#include "UDPServer.hh"
#include "OutputManipulator.hh"

UDPServer::UDPServer()
  : fd_socket(0)
{
  OpenSocket();
}

UDPServer::~UDPServer(){
  CloseSocket();
}

void UDPServer::OpenSocket(){
  if(fd_socket != 0){ // socket already opened
    return;
  }

  if((fd_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
    std::ostringstream os;
    os << "socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) failed\n"
       << strerror(errno);
    throw EXCEPTION(os.str());
  }
}

void UDPServer::CloseSocket(){
  if(fd_socket){
    close(fd_socket);
    fd_socket = 0;
  }
}

bool UDPServer::SendPacket(const std::string& target_address,
                           unsigned short target_port,
                           const char* data,
                           size_t length,
                           bool nothrow) const
{
  if(fd_socket == 0){
    if(nothrow){
      return false;
    }
    else{
      throw EXCEPTION("Socket not open.");
    }
  }

  struct sockaddr_in target;
  memset(&target, 0, sizeof(target));
  target.sin_family = AF_INET;
  target.sin_port = htons(target_port);

  if(inet_aton(target_address.c_str(), &target.sin_addr) == 0){
    if(nothrow){
      return false;
    }
    else{
      std::ostringstream os;
      os << "inet_aton(" << target_address << ") failed\n"
         << strerror(errno);
      throw EXCEPTION(os.str());
    }
  }


  if(sendto(fd_socket, data, length, 0,
            (const sockaddr*) &target,
            sizeof(target)) == -1)
  {
    if(nothrow){
      return false;
    }
    else{

      std::ostringstream os;
      os << "sendto(" << target_address << ":" << target_port << ", "
         << "\"" << data << "\" failed\n"
         << strerror(errno);
      throw EXCEPTION(os.str());
    }
  }

  return true;
}




// UDPServer.cc ends here
