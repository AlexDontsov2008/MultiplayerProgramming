/* 
 * File:   UDPServer.hpp
 * Author: dontsov
 *
 * Created on November 16, 2016, 10:01 PM
 */

#ifndef UDPSERVER_HPP
#define UDPSERVER_HPP

#include "UDPSocket.hpp"
#include "SocketAddress.hpp"
#include "Uncopyable.hpp"

class UDPServer : private Uncopyable {
public:
    explicit UDPServer(const std::string& inServerAddress);
    virtual ~UDPServer();

    /** Процесс запуска сервера для приема и обработки данных */
    void Run();

    /** Получение адреса сервера
     * 
     * @return Адрес сервера
     */
    SocketAddressPtr GetAddress() const;

private:
    UDPSocketPtr mSocket;
    SocketAddressPtr mAddress;

    /** Обработка входящих данных
     * 
     * @param outBuffer - данные для обработки
     */
    void ProcessReceivedData(char* outBuffer, const int inReceivedDataCount);
};

#endif /* UDPSERVER_HPP */