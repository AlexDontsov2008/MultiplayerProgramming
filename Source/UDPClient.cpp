/* 
 * File:   UDPClient.cpp
 * Author: dontsov
 * 
 * Created on November 17, 2016, 7:50 AM
 */

#include "UDPClient.hpp"
#include "SocketUtil.hpp"
#include "SocketAddressFactory.hpp"

UDPClient::UDPClient(const std::string& inServerAddress) 
: mSocket(SocketUtil::CreateUDPSocket(SocketAddressFamily::INET))
, mServerAddress(SocketAddressFactory::CreateIPv4FromString(inServerAddress)) {}

UDPClient::~UDPClient() {
}


    /* Процесс запуска клиента для отправки данных на сервер */
void UDPClient::Run() {
    printf("UDP Server is Running\n");

    const size_t BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    bool IsUDPClientRun = true;
    while (IsUDPClientRun) {
        printf("Type a sentence to send to server:\n");
        fgets(buffer, BUFFER_SIZE, stdin);
        printf("You typed: %s", buffer);
        // Отправить данные серверу
        const int sendByteCount = mSocket->SendTo(buffer, BUFFER_SIZE, *mServerAddress);
    }
}

/** Установка нового адреса сервера
     * 
     * @param inServerAddress - адрес сервера
     */
void UDPClient::SetServerAddress(const SocketAddressPtr& inServerAddress) {
    mServerAddress = inServerAddress;
}

void UDPClient::SetServerAddress(const std::string& inServerAddress) {
    mServerAddress = SocketAddressFactory::CreateIPv4FromString(inServerAddress);
}