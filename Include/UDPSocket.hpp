/*
 * File:   UDPSocket.hpp
 * Author: dontsov
 *
 * Created on November 12, 2016, 6:34 PM
 */

#ifndef UDPSOCKET_HPP
#define UDPSOCKET_HPP

#include <memory>

#ifdef _linux
#include "sys/socket.h"
#include "netdb.h"

typedef int SOCKET;
#endif


class SocketAddress;
class SocketUtil;

class UDPSocket {
public:
    ~UDPSocket();
    int Bind(const SocketAddress& inToAddress);
    int SendTo(const void* inData, int inLen, const SocketAddress& inTo);
    int ReceiveFrom(void* inBuffer, int inLen, SocketAddress& outFrom);

private:
    friend class SocketUtil;
    UDPSocket(SOCKET inSocket) : mSocket(inSocket) {}
    SOCKET mSocket;
};

typedef std::shared_ptr<UDPSocket> UDPSocketPtr;

#endif /* UDPSOCKET_HPP */