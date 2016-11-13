/*
 * File:   SocketUtil.hpp
 * Author: dontsov
 *
 * Created on November 13, 2016, 1:48 PM
 */

#ifndef SOCKETUTIL_HPP
#define SOCKETUTIL_HPP

#include "UDPSocket.hpp"

enum SocketAddressFamily {
    INET = AF_INET,
    INET6 = AF_INET6
};

class SocketUtil {
public:
    static void ReportError(const char* inOperationDesc);
    static int GetLastError();

    static UDPSocketPtr CreateUDPSocket(SocketAddressFamily inFamily);
};

#endif /* SOCKETUTIL_HPP */