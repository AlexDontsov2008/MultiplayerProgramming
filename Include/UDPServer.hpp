/* 
 * File:   UDPServer.hpp
 * Author: dontsov
 *
 * Created on November 16, 2016, 10:01 PM
 */

#ifndef UDPSERVER_HPP
#define UDPSERVER_HPP

#include "UDPSocket.hpp"
#include "SocketAddress.hpp"
#include "Uncopyable.hpp"

class UDPServer : private Uncopyable {
public:
    explicit UDPServer(const std::string& inServerAddress);
    virtual ~UDPServer();

    void Run();
    SocketAddressPtr GetAddress() const;

private:
    UDPSocketPtr mSocket;
    SocketAddressPtr mAddress;
    

};

#endif /* UDPSERVER_HPP */