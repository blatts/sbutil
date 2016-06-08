// -*- mode: C++ -*-
// Time-stamp: "2016-02-12 16:01:42 sb"

/*
  file       UDPServer.hh
  copyright  (c) Sebastian Blatt 2012 -- 2016

  Provides a simple wrapper around an unbound connectionless socket
  that can be used to send UDP packets to arbitrary IPV4 addresses.

  Idea: instantiate one of these and call SendPacket.


*/


#ifndef UDPSERVER_HH__B7CA5A67_6C37_42DF_A4F5_9D21373080F8
#define UDPSERVER_HH__B7CA5A67_6C37_42DF_A4F5_9D21373080F8

#include <sbutil/UDPPacket.hh>
#include <stdint.h>

class UDPServer {
  private:
    int fd_socket;

  public:
    UDPServer();
    virtual ~UDPServer();

    void OpenSocket();
    void CloseSocket();

    // The nothrow parameter can be used to prevent Exception handling
    // for performance reasons. Downside: only boolean value is
    // returned.
    //
    bool SendPacket(const std::string& target_address,
                    unsigned short target_port,
                    const char* data,
                    size_t length,
                    bool nothrow = false) const;


    // Convenience wrappers for SendPacket.


    // careful with embedded zeros!
    bool SendPacket(const std::string& target_address,
                    unsigned short target_port,
                    const std::string& data,
                    bool nothrow = false) const {
      return SendPacket(target_address, target_port, data.c_str(), data.size(), nothrow);
    }

    template<typename T>
    bool SendPacket(const std::string& target_address,
                    unsigned short target_port,
                    const T* binary_data,
                    size_t length,
                    bool nothrow = false) const
    {
      return SendPacket(target_address,
                        target_port,
                        reinterpret_cast<const char*>(binary_data),
                        sizeof(T) * length,
                        nothrow);
    }

    template<typename T>
    bool SendPacket(const std::string& target_address,
                    unsigned short target_port,
                    const std::vector<T>& binary_data,
                    bool nothrow = false) const{
      return SendPacket(target_address,
                        target_port,
                        reinterpret_cast<const char*>(&(binary_data[0])),
                        sizeof(T) * binary_data.size(),
                        nothrow);
    }

    bool SendPacket(UDPPacket& packet, bool nothrow=false) const {
      packet.Serialize(nothrow);
      return SendPacket(packet.GetAddress(),
                        packet.GetPort(),
                        packet.GetData(),
                        nothrow);
    }
};

#endif // UDPSERVER_HH__B7CA5A67_6C37_42DF_A4F5_9D21373080F8

// UDPServer.hh ends here
