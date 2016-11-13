/*
 * File:   SocketUtil.cpp
 * Author: dontsov
 *
 * Created on November 13, 2016, 1:48 PM
 */

#include "errno.h"

#include "SocketUtil.hpp"

const int ERROR = -1;

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

    LOG( "Error %s: %d- %s", inOperationDesc, errorNum, lpMsgBuf );
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