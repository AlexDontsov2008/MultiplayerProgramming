/*
 * File:   SocketAddressFactory.hpp
 * Author: dontsov
 *
 * Created on November 10, 2016, 9:56 PM
 */

#ifndef SOCKETADDRESSFACTORY_HPP
#define SOCKETADDRESSFACTORY_HPP

#include <string>

#include "netinet/in.h"
#include "netdb.h"

static const std::string DEFAULT_PORT = "0";

class SocketAddress;

class SocketAddressFactory {
public:
    static SocketAddressPtr CreateIPv4FromString(const std::string& inAddrStr) {
        auto pos = inAddrStr.find_last_of(':');
        std::string host, service;
        if (pos != std::string::npos) {
            host = inAddrStr.substr(0, pos);
            service = inAddrStr.substr(pos + 1);
        } else {
            host = inAddrStr;
            service = DEFAULT_PORT;
        }
        addrinfo hint;
        memset (&hint, 0, sizeof(hint));
        hint.ai_family = AF_INET;

        addrinfo* result = nullptr;
        int error = getaddrinfo(host.c_str(), service.c_str(), &hint, &result);

        if (error && result) {
            freeaddrinfo(result);
            return nullptr;
        }

        while (!result->ai_addr && result->ai_next) {
            result = result->ai_next;
        }

        if (!result->ai_addr) {
            freeaddrinfo(result);
            return nullptr;
        }

        auto toRet = std::make_shared<SocketAddress>(*result->ai_addr);
        freeaddrinfo(result);
        return toRet;
    }
};

#endif /* SOCKETADDRESSFACTORY_HPP */