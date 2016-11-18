/* 
 * File:   TCPSocket.cpp
 * Author: dontsov
 * 
 * Created on November 14, 2016, 10:01 PM
 */

#include "TCPSocket.hpp"
#include "SocketAddress.hpp"
#include "SocketUtil.hpp"

/** Инициирует TCP handshake отправляя SYN пакет целевому хосту
* 
* @param inAddress - адрес удаленного хоста
* @return состояние выполнения
*/
int TCPSocket::Connect(const SocketAddress& inAddress) {
    int err = connect(mSocket, &inAddress.mSockAddr, inAddress.GetSize());
    if (err < 0) {
        SocketUtil::ReportError("TCPSocket::Connect");
        return -SocketUtil::GetLastError();
    }
    return NO_ERROR;
}

/** Служит для прослушивания поступающих handshake
* 
* @param inBackLog - максимальное количество входящих подключений
* @return состояние выполнение
*/
int TCPSocket::Listen(int inBackLog) {
    int err = listen(mSocket, inBackLog);
    if (err < 0) {
        SocketUtil::ReportError("TCPSocket::Listen");
        return -SocketUtil::GetLastError();
    }
    
    return NO_ERROR;
}

/** Согласие на входящее соединение, для продолжения TCP handshake
* 
* @param inFromAddress - адрес удаленного хоста которые инициировал соединение
* @return сокет, для взаимодействия с удаленным host
*/
TCPSocketPtr TCPSocket::Accept(SocketAddress& inFromAddress) {
    socklen_t length = inFromAddress.GetSize();
    SOCKET newSocket = accept(mSocket, &inFromAddress.mSockAddr, &length);
    
    if (newSocket != INVALID_SOCKET) {
        return TCPSocketPtr(new TCPSocket(newSocket));
    } else {
        SocketUtil::ReportError("TCPSocket::Accept");
        return nullptr;
    }
}

/** Отправка данных через TCPSocket
* 
* @param inData - данные для передачи, помещаются в socket's send buffer
* @param inLen - размер данных для передачи
* @return количество переданных данных
*/
int32_t TCPSocket::Send(const void* inData, int inLen) {
    int bytesSentCount = send(mSocket, static_cast<const char*>(inData), inLen, 0);
    if (bytesSentCount < 0) {
        SocketUtil::ReportError("TCPSocket::Send");
        return -SocketUtil::GetLastError();
    }
    return bytesSentCount;
}

/** Прием данных через TCPSocket
* 
* @param inBuffer - буфер для копирования данных из socket's receive buffer
* @param inLen - размер буфера для получения данных
* @return размер полученных данных
*/
int32_t TCPSocket::Receive(void* inBuffer, int inLen) {
    int bytesReceivedCount = recv(mSocket, static_cast<char*>(inBuffer), inLen, 0);
    if (bytesReceivedCount < 0) {
        SocketUtil::ReportError("TCPSocket::Received");
        return -SocketUtil::GetLastError();
    }
    return bytesReceivedCount;
}


/** Связывает сокет с выбранным адресом
* 
* @param inToAddress - адрес с которым происходит связывание
* @return состояние выполнения
*/
int TCPSocket::Bind(const SocketAddress& inBindAddress) {
    int error = bind(mSocket, &inBindAddress.mSockAddr, inBindAddress.GetSize());
    if (error != 0) {
        SocketUtil::ReportError("TCPSocket::Bind");
        return SocketUtil::GetLastError();
    }
    
    return NO_ERROR;
}

/** Устанавливает сокет в non-blocking mode
 * 
 * @return состояние выполнения
 */
int TCPSocket::SetNonBlockingMode() {
#ifdef _WIN32
    return ChangeBlockingMode(1, "TCPSocket::SetNonBlockingMode");
#else
    int flags = fcntl(mSocket, F_GETFL, 0);
    return ChangeBlockingMode(flags | O_NONBLOCK, "TCPSocket::SetNonBlockingMode");
#endif
}

/** Устанавливает сокет в blocking mode
 * 
 * @return состояние выполнения
 */
int TCPSocket::SetBlockingMode() {
#ifdef _WIN32
    return ChangeBlockingMode(0, "TCPSocket::SetBlockingMode");
#else
    int flags = fcntl(mSocket, F_GETFL, 0);
    return ChangeBlockingMode(flags & ~O_NONBLOCK, "TCPSocket::SetBlockingMode");
#endif
}

/** Устанавливает сокет в указанное состояние
* 
* @param inFlags - задают состояние сокета
* @param inInvokedMethodName - имя вызывающего метода
* @return состояние выполнения
*/
int TCPSocket::ChangeBlockingMode(int flags, const std::string& invokedMethodName) {
#ifdef _WIN32
    int result = ioctlsocket(mSocket, FIONBIO, &(u_long)arg);
#else
    int result = fcntl(mSocket, F_SETFL, flags);
#endif

    if (result == SOCKET_ERROR) {
        SocketUtil::ReportError(invokedMethodName.c_str());
        return SocketUtil::GetLastError();
    } else {
        return NO_ERROR;
    }
}

TCPSocket::~TCPSocket() {
#if _WIN32
    closesocket(mSocket);
#else
    close(mSocket);
#endif  
}