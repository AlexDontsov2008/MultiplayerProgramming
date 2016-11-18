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

TCPServer::TCPServer(const std::string& inAddress) 
: mSocket(SocketUtil::CreateTCPSocket(SocketAddressFamily::INET))
, mAddress(SocketAddressFactory::CreateIPv4FromString(inAddress)) {
    int error = mSocket->Bind(*mAddress);
    if (error != NO_ERROR) {
        exit(1);
    }
    // Установка в non-blocking режим
    error = mSocket->SetNonBlockingMode();
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
    return mClients.size();
}

/** Процесс запуска сервера для приема и обработки данных */
void TCPServer::Run() {
    int error = mSocket->Listen();
    if (error != NO_ERROR) {
        exit(1);
    }

    const size_t BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    const bool IsServerRunning = true;
    
    while (IsServerRunning) {
        SocketAddressPtr clientAddress;
        TCPSocketPtr clientSock = mSocket->Accept(*clientAddress);
        if (clientSock) {
            mClients.push_back(ClientInfo{ clientSock, clientAddress });
            printf("Count of connected clients: %zu", CountOfConnectedClients());
        }

        for (ClientInfo& client : mClients) {
            int32_t receivedBytesCount = client.bindingSocket->Receive(buffer, BUFFER_SIZE);
            if (receivedBytesCount < 0) {
                exit(1);
            }

            printf("Received \"%s\" message from client", buffer);
        }
    }
}