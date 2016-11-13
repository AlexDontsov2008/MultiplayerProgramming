/*
 * File:   SocketAddress.hpp
 * Author: dontsov
 *
 * Created on November 10, 2016, 8:23 AM
 */

#ifndef SOCKETADDRESS_HPP
#define SOCKETADDRESS_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <memory>

#include "netinet/in.h"

/** Каждый network layer packet требует source/destination адреса.
 *  Если пакет обертывает transport layer требуется source/destination порт.
 *  Для передачи этой информации в socket library и из нее, используется  данный тип данных.
 */
class SocketAddress {
public:
    SocketAddress(uint32_t inAddress, uint16_t inPort) {
        GetAsSockAddrIn()->sin_family =  PF_INET;
        GetAsSockAddrIn()->sin_port = htons(inPort);
        GetAsSockAddrIn()->sin_addr.s_addr = htonl(inAddress);
        memset(GetAsSockAddrIn()->sin_zero, 0, sizeof(GetAsSockAddrIn()->sin_zero));
    }

    SocketAddress(const sockaddr& inSockAddr) {
        memcpy(&mSockAddr, &inSockAddr, sizeof(sockaddr));
    }

    SocketAddress() {
        GetAsSockAddrIn()->sin_family = AF_INET;
        GetIP4Ref() = INADDR_ANY;
        GetAsSockAddrIn()->sin_port = 0;
    }

    size_t GetSize() const {
        return sizeof (sockaddr);
    }

    void Info() const {
        std::cout << "**************** SocketAddress Info ****************" << std::endl;
        const auto sockAddrPtr = GetAsSockAddrIn();

        std::cout << "sin_family: ";
        switch (sockAddrPtr->sin_family) {
            case PF_INET:
                std::cout << "IPv4" << std::endl;
                break;
            case PF_INET6:
                std::cout << "IPv6" << std::endl;
                break;
            default:
                std::cout << "Unspecified" << std::endl;
        }

        std::cout << "Port: " << ntohs(sockAddrPtr->sin_port) << std::endl;
    }

private:
    friend class UDPSocket;

    sockaddr mSockAddr;

    uint32_t& GetIP4Ref() {
        return GetAsSockAddrIn()->sin_addr.s_addr;
    }

    const uint32_t& GetIP4Ref() const {
        return GetAsSockAddrIn()->sin_addr.s_addr;
    }

    sockaddr_in* GetAsSockAddrIn() {
        return reinterpret_cast<sockaddr_in*>( &mSockAddr );
    }

    const sockaddr_in* GetAsSockAddrIn() const {
        return reinterpret_cast<const sockaddr_in*>( &mSockAddr );
    }
};

typedef std::shared_ptr<SocketAddress> SocketAddressPtr;

#endif /* SOCKETADDRESS_HPP */