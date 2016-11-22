#include "TCPClient.hpp"
#include "SocketUtil.hpp"
#include "SocketAddressFactory.hpp"

TCPClient::TCPClient(const std::string& inServerAddress)
: mSocket(SocketUtil::CreateTCPSocket(SocketAddressFamily::INET))
, mServerAddress(SocketAddressFactory::CreateIPv4FromString(inServerAddress)) {
}


TCPClient::~TCPClient() {}


/** Запуск клиента и подключение к серверу */
void TCPClient::Run() {
    int error = mSocket->Connect(*mServerAddress);
    if (error != NO_ERROR) {
        exit(1);
    }

    const size_t GOOD_SEGMENT_SIZE = 512;
    char buffer[GOOD_SEGMENT_SIZE];
    const bool IsClientRunning = true;
    while (IsClientRunning) {
        printf("Enter message to send to the server:\n");
        fgets(buffer, GOOD_SEGMENT_SIZE, stdin);
        int sentByteCount = mSocket->Send(buffer, GOOD_SEGMENT_SIZE);
        if (sentByteCount > 0) {
            printf("Sent data to Server\n");
        }
    }
}