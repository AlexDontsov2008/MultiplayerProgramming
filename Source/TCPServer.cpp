/* 
 * File:   TCPServer.cpp
 * Author: dontsov
 * 
 * Created on November 17, 2016, 8:58 PM
 */

#include <vector>

#include "TCPServer.hpp"
#include "SocketUtil.hpp"
#include "SocketAddressFactory.hpp"

TCPServer::TCPServer()
: mListenSocket(SocketUtil::CreateTCPSocket(SocketAddressFamily::INET))
, mAddress(new SocketAddress(INADDR_ANY, DEFAULT_PORT)) {
    int error = mListenSocket->Bind(*mAddress);
    if (error != NO_ERROR) {
        exit(1);
    }
}

TCPServer::TCPServer(const std::string& inAddress)
: mListenSocket(SocketUtil::CreateTCPSocket(SocketAddressFamily::INET))
, mAddress(SocketAddressFactory::CreateIPv4FromString(inAddress)) {
    int error = mListenSocket->Bind(*mAddress);
    if (error != NO_ERROR) {
        exit(1);
    }
  }

TCPServer::~TCPServer() {}

/** Количество подключившихся клиентов
 * 
 * @return количество клиентов
 */
size_t TCPServer::CountOfConnectedClients() const {
    return mReadSockets.size();
}

/** Процесс запуска сервера для приема и обработки данных */
void TCPServer::Run() {
   mReadSockets.push_back(mListenSocket);
    
    int error = mListenSocket->Listen();
    if (error != NO_ERROR) {
        exit(1);
    }

    const bool IsServerRunning = true;
    while (IsServerRunning) {
        if (SocketUtil::Select( &mReadSockets, &mReadableSockets,
                                nullptr, nullptr,
                                nullptr, nullptr)) {
            for (const TCPSocketPtr& socket : mReadableSockets) {
                if (socket == mListenSocket) {
                    SocketAddress newClientAddress;
                    auto newSocket = mListenSocket->Accept(newClientAddress);
                    mReadSockets.push_back(newSocket);
                    printf("Count of connected clients: %zu", CountOfConnectedClients());
                    // Process the new client
                } else {
                    const size_t GOOD_SEGMENT_SIZE = 512;
                    char buffer[GOOD_SEGMENT_SIZE];
                    
                    int receivedByteCount = socket->Receive(buffer, GOOD_SEGMENT_SIZE);
                    if (receivedByteCount > 0) {
                        printf("Received data from client: %s", buffer);
                    }
                }
            }
        }
    }
}