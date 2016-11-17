/*
 * File:   main.cpp
 * Author: dontsov
 *
 * Created on November 9, 2016, 10:38 PM
 */

#include <memory>
#include <algorithm>
#include <cassert>

#include "SocketAddress.hpp"
#include "SocketAddressFactory.hpp"
#include "SocketUtil.hpp"
#include "UDPServer.hpp"

using namespace std;

const size_t BUFFER_SIZE = 1024;

int main(int argc, char** argv) {
#ifdef _WIN32
    WSAData wsaData;
    int wsaStartupErr = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if( wsaStartupErr != 0)
    {
        printf("WSAStartup error: %d", wsaStartupErr);
        exit(1);
    }
#endif

#ifdef SERVER
    /* ********************* UDP SERVER ********************* */
    UDPServer server("192.168.0.104:7891");
    server.Run();
#endif

#ifdef CLIENT
    /* ********************* UDP CLIENT ********************* */
    // 1. Создать UDP socket
    auto udpSocket = SocketUtil::CreateUDPSocket(INET);
    assert(udpSocket);
    // 2. Задать SocketAddress сервера
    auto serverAddress = SocketAddressFactory::CreateIPv4FromString("192.168.0.105:7891");
    assert(serverAddress);
    // 4. Цикл UDP клиента
    char buffer[BUFFER_SIZE];
    bool IsUDPClientRun = true;
    while (IsUDPClientRun) {
        printf("Type a sentence to send to server:\n");
        fgets(buffer, BUFFER_SIZE, stdin);
        printf("You typed: %s", buffer);

        // Отправить данные серверу
        const int sendByteCount = udpSocket->SendTo(buffer, BUFFER_SIZE, *serverAddress);
    }
#endif

#ifdef _WIN32
    int wsaCleanupErr = WSACleanup();
    if( wsaCleanupErr != 0)
    {
        printf("WSACleanup error: %d", wsaCleanupErr);
        exit(1);
    }
#endif
    return 0;
}