/*
 * File:   SocketUtil.cpp
 * Author: dontsov
 *
 * Created on November 13, 2016, 1:48 PM
 */



#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "Windows.h"
#include "Ws2tcpip.h"
#else
#include "errno.h"
#endif

#include "SocketUtil.hpp"

#ifdef _linux
const int ERROR = -1;
#endif

UDPSocketPtr SocketUtil::CreateUDPSocket(SocketAddressFamily inFamily) {
    SOCKET sock = socket(inFamily, SOCK_DGRAM, IPPROTO_UDP);
    if (sock != ERROR) {
        return UDPSocketPtr(new UDPSocket(sock));
    } else {
        ReportError("SocketUtil::CreateUDPSocket");
        return nullptr;
    }
}

void SocketUtil::ReportError(const char* inOperationDesc) {
#if _WIN32
    LPVOID lpMsgBuf;
    DWORD errorNum = GetLastError();

    FormatMessage(
                    FORMAT_MESSAGE_ALLOCATE_BUFFER |
                    FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL,
                    errorNum,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    (LPTSTR) &lpMsgBuf,
                    0, NULL );

    printf( "Error %s: %d- %s", inOperationDesc, errorNum, lpMsgBuf );
#else
    printf("Error: %s", inOperationDesc);
#endif
}

int SocketUtil::GetLastError() {
#if _WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}