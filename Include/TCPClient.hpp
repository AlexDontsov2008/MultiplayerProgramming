#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include "TCPSocket.hpp"
#include "SocketAddress.hpp"

class TCPClient {
public:
    explicit TCPClient(const std::string& inServerAddress);
    virtual ~TCPClient();

    /** ������� ������� ������� ��� ������ � ��������� ������ */
    void Run();

private:
    TCPSocketPtr mSocket;
    SocketAddressPtr mServerAddress;
};

#endif