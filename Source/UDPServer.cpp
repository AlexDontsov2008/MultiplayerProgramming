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

/** Процесс запуска сервера для приема и обработки данных */
void UDPServer::Run() {
    printf("UDP Server is Running\n");
    
    constexpr size_t BUFFER_SIZE = 1024;
    SocketAddressPtr clientAddress = std::make_shared<SocketAddress>();
    char buffer[BUFFER_SIZE];
    
    bool IsUDPServerRun = true;
    while (IsUDPServerRun) {
        // Получить данные
        const int receivedByteCount = mSocket->ReceiveFrom(buffer, BUFFER_SIZE, *clientAddress);
        if (receivedByteCount < 0) {
            exit(1);
        }

        printf("Received data from client: %s", buffer);
        
        // Обработать данные
        ProcessReceivedData(buffer, receivedByteCount);
        // Отправить обработанные данные клиенту.
        const int sentByteCount = mSocket->SendTo(buffer, receivedByteCount, *clientAddress);
        if (sentByteCount < 0) {
            exit (1);
        }
    }
}

    /** Получение адреса сервера
     * 
     * @return Адрес сервера
     */
SocketAddressPtr UDPServer::GetAddress() const {
    return mAddress;
}

    /** Обработка входящих данных
     * 
     * @param outBuffer - данные для обработки
     */
void UDPServer::ProcessReceivedData(char* outBuffer, const int receivedByteCount) {
    for (int i = 0; i < receivedByteCount; ++i) {
        outBuffer[i] = toupper(outBuffer[i]);
    }
}