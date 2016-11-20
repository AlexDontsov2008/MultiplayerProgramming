/*
 * File:   SocketUtil.cpp
 * Author: dontsov
 *
 * Created on November 13, 2016, 1:48 PM
 */

#include <vector>

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


/** Заполняет коллекцию сокетов из вектора
 * 
 * @param outSet - коллекция сокетов для заполнения
 * @param inSockets - исходный вектор сокетов
 * @param ioNaxNfds - максимальный пронумерованный сокет (Для Posixf-систем)
 * @return заполненная коллекция сокетов
 */
fd_set* SocketUtil::FillSetFromVector(fd_set& outSet, const SocketsVector* inSockets, int& ioNaxNfds) {
    if (inSockets) {
        FD_ZERO(&outSet);
        
        for (const TCPSocketPtr& socket : *inSockets) {
            FD_SET(socket->mSocket, &outSet);
#ifndef _WIN32
            ioNaxNfds = std::max(ioNaxNfds, socket->mSocket);
#endif
        }
        return &outSet;
    } else {
        return nullptr;
    }
}

/** Заполняет вектор из коллекции сокетов
* 
* @param outSockets - вектор, заполняемый методом
* @param inSockets - вектор, содержащий все сокеты для проверки.
* @param inSet - коллекция сокетов для заполнения
*/
void SocketUtil::FillVectorFromSet(SocketsVector* outSockets, const SocketsVector* inSockets, const fd_set& inSet) {
    if (inSockets && outSockets) {
        for (const TCPSocketPtr& socket : *inSockets) {
            if (FD_ISSET(socket->mSocket, &inSet)) {
                outSockets->push_back(socket);
            }
        }
    }
}

/** Проверяет состояние переданных сокетов
 * 
 * @param inReadSet - доступные сокеты для чтения
 * @param outReadSet - сокеты готовые для чтения 
 * @param inWriteSet - доступные сокеты для записи
 * @param outWriteSet - сокеты готовые для записи
 * @param inExceptSet - доступные сокеты для проверки ошибок 
 * @param outExceptSet - сокеты готовые для проверки ошибок
 * @return количество готовых к действию сокетов
 */
int SocketUtil::Select( const SocketsVector* inReadSet, SocketsVector* outReadSet,
                        const SocketsVector* inWriteSet, SocketsVector* outWriteSet,
                        const SocketsVector* inExceptSet, SocketsVector* outExceptSet) {
    fd_set read, write, except;
    int ndfs = 0;

    fd_set* readPtr = FillSetFromVector(read, inReadSet, ndfs);
    fd_set* writePtr = FillSetFromVector(write, inWriteSet, ndfs);
    fd_set* exceptPtr = FillSetFromVector(except, inExceptSet, ndfs);

    int toRet = select(ndfs + 1, readPtr, writePtr, exceptPtr, nullptr);

    if (toRet > 0) {
        FillVectorFromSet(outReadSet, inReadSet, read);
        FillVectorFromSet(outWriteSet, inWriteSet, write);
        FillVectorFromSet(outExceptSet, inExceptSet, except);
    }
    return toRet;
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