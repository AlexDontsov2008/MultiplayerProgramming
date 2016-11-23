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

#include "SocketSharedInfo.hpp"

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

    std::string ToString() const {
        const socklen_t BUFFER_SIZE = 128;
        char destinationBuffer[BUFFER_SIZE];
        const sockaddr_in* s = GetAsSockAddrIn();
#if _WIN32
        InetNtop( s->sin_family, const_cast< in_addr* >( &s->sin_addr ), (PWSTR)destinationBuffer, sizeof( destinationBuffer ) );
        sprintf(destinationBuffer, "%s:%d", destinationBuffer, ntohs(s->sin_port));
#else
        const char* result = inet_ntop(s->sin_family, &s->sin_addr, destinationBuffer, BUFFER_SIZE);
        if (!result) {
            // TODO
            exit(1);
        }
#endif
        return destinationBuffer;
    }

private:
    friend class UDPSocket;
    friend class TCPSocket;
    sockaddr mSockAddr;

#ifdef _WIN32
	uint32_t& GetIP4Ref() {
		return *(reinterpret_cast<uint32_t*>(&GetAsSockAddrIn()->sin_addr.S_un.S_addr));
    }
	const uint32_t& GetIP4Ref() const {
		return *(reinterpret_cast<const uint32_t*>(&GetAsSockAddrIn()->sin_addr.S_un.S_addr));
    }
#else
    uint32_t& GetIP4Ref() {
        return GetAsSockAddrIn()->sin_addr.s_addr;
    }

    const uint32_t& GetIP4Ref() const {
        return GetAsSockAddrIn()->sin_addr.s_addr;
    }
#endif
    sockaddr_in* GetAsSockAddrIn() {
        return reinterpret_cast<sockaddr_in*>( &mSockAddr );
    }

    const sockaddr_in* GetAsSockAddrIn() const {
        return reinterpret_cast<const sockaddr_in*>( &mSockAddr );
    }
};

typedef std::shared_ptr<SocketAddress> SocketAddressPtr;

#endif /* SOCKETADDRESS_HPP */