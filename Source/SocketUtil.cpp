/*
 * File:   SocketUtil.cpp
 * Author: dontsov
 *
 * Created on November 13, 2016, 1:48 PM
 */

#include "SocketUtil.hpp"
#include "SocketSharedInfo.hpp"

UDPSocketPtr SocketUtil::CreateUDPSocket(SocketAddressFamily inFamily) {
    SOCKET sock = socket(inFamily, SOCK_DGRAM, IPPROTO_UDP);
    if (sock != INVALID_SOCKET) {
        return UDPSocketPtr(new UDPSocket(sock));
    } else {
        ReportError("SocketUtil::CreateUDPSocket");
        return nullptr;
    }
}

TCPSocketPtr SocketUtil::CreateTCPSocket(SocketAddressFamily inFamily) {
    SOCKET sock = socket(inFamily, SOCK_STREAM, IPPROTO_TCP);
    if (sock != INVALID_SOCKET) {
        return TCPSocketPtr(new TCPSocket(sock));
    } else {
        ReportError("SocketUtil::CreateTCPSocket");
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