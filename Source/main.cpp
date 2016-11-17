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
#include "UDPClient.hpp"

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
    UDPServer server("192.168.0.105:7891");
    server.Run();
#else
    /* ********************* UDP CLIENT ********************* */
    UDPClient client("192.168.0.104:7891");
    client.Run();
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