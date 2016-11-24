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

const size_t TCPServer::DEFAULT_PORT = 48000;

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
int TCPServer::CountOfConnectedClients() const {
    return mReadSockets.size() - 1;
}

/** Процесс запуска сервера для приема и обработки данных */
void TCPServer::Run() {
   mReadSockets.push_back(mListenSocket);
    
    int error = mListenSocket->Listen();
    if (error != NO_ERROR) {
        exit(1);
    }

    SocketsVector readableSockets;
    const bool IsServerRunning = true;
    while (IsServerRunning) {
        if (SocketUtil::Select( &mReadSockets, &readableSockets,
                                nullptr, nullptr,
                                nullptr, nullptr)) {
            for (const TCPSocketPtr& socket : readableSockets) {
                if (socket == mListenSocket) {
                    SocketAddress newClientAddress;
                    auto newSocket = mListenSocket->Accept(newClientAddress);
                    mReadSockets.push_back(newSocket);
                    printf("Count of connected clients: %d\n", CountOfConnectedClients());
                    // Process the new client
                } else {
                    const size_t GOOD_SEGMENT_SIZE = 512;
                    char buffer[GOOD_SEGMENT_SIZE];
                    
                    int receivedByteCount = socket->Receive(buffer, GOOD_SEGMENT_SIZE);
                    if (receivedByteCount > 0) {
                        SocketAddress socketAddr;
                        socket->GetPeerName(socketAddr);
                        const std::string clientIP = socketAddr.ToString();
                        printf("Received data: %s", buffer);
                        printf("Client IP: %s\n", clientIP.c_str());
                    }                    
                }
            }
            readableSockets.clear();
        }
    }
}