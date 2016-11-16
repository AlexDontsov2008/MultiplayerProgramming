/* 
 * File:   UDPServer.cpp
 * Author: dontsov
 * 
 * Created on November 16, 2016, 10:01 PM
 */

#include "UDPServer.hpp"
#include "SocketUtil.hpp"
#include "SocketAddressFactory.hpp"

UDPServer::UDPServer(const std::string& inServerAddress) 
: mSocket(SocketUtil::CreateUDPSocket(SocketAddressFamily::INET))
, mAddress(SocketAddressFactory::CreateIPv4FromString(inServerAddress)) {
    int error = mSocket->Bind(*mAddress);
    if (error != 0) {
        exit(1);
    }
}

UDPServer::~UDPServer() {}

void UDPServer::Run() {
    printf("UDP Server is Running\n");
    
    constexpr size_t BUFFER_SIZE = 1024;
    SocketAddressPtr clientAddress = std::make_shared<SocketAddress>();
    char buffer[BUFFER_SIZE];
    
    bool IsUDPServerRun = true;
    while (IsUDPServerRun) {
        // Получить данные
        const int readByteCount = mSocket->ReceiveFrom(buffer, BUFFER_SIZE, *clientAddress);
        if (readByteCount < 0) {
            exit(1);
        }

        printf("Received data from client: %s", buffer);
        
        // Обработать данные
        for (int i = 0; i < readByteCount; ++i) {
            buffer[i] = toupper(buffer[i]);
        }
        // Отправить обработанные данные клиенту.
        const int sendByteCount = mSocket->SendTo(buffer, readByteCount, *clientAddress);
    }
}

SocketAddressPtr UDPServer::GetAddress() const {
    return mAddress;
}