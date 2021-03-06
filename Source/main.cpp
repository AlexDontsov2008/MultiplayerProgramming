/*
 * File:   main.cpp
 * Author: dontsov
 *
 * Created on November 9, 2016, 10:38 PM
 */

#include "UDPServer.hpp"
#include "UDPClient.hpp"
#include "TCPServer.hpp"
#include "TCPClient.hpp"

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
    TCPClient client("192.168.0.104:48000");
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