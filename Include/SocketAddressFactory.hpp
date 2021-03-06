/*
 * File:   SocketAddressFactory.hpp
 * Author: dontsov
 *
 * Created on November 10, 2016, 9:56 PM
 */

#ifndef SOCKETADDRESSFACTORY_HPP
#define SOCKETADDRESSFACTORY_HPP

#include <string>
#include <memory>

#include "SocketAddress.hpp"
#include "SocketUtil.hpp"

class SocketAddressFactory {
public:
    static SocketAddressPtr CreateIPv4FromString(const std::string& inAddrStr);
};

#endif /* SOCKETADDRESSFACTORY_HPP */