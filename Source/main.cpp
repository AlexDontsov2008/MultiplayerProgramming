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
#include "TCPServer.hpp"

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
    /* ********************* SERVER ********************* */
    TCPServer server;
    server.Run();

#else
    /* ********************* CLIENT ********************* */
    TCPClient client("5.228.255.178:DEFAULT_PORT");
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