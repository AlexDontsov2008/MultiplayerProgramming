/* 
 * File:   UDPClient.hpp
 * Author: dontsov
 *
 * Created on November 17, 2016, 7:50 AM
 */

#ifndef UDPCLIENT_HPP
#define UDPCLIENT_HPP

#include "SocketAddress.hpp"
#include "UDPSocket.hpp"

class UDPClient {
public:
    UDPClient(const std::string& inServerAddress);
    virtual ~UDPClient();

    /* Процесс запуска клиента для отправки данных на сервер */
    void Run();

    /** Установка нового адреса сервера
     * 
     * @param inServerAddress - адрес сервера
     */
    void SetServerAddress(const SocketAddressPtr& inServerAddress);
    void SetServerAddress(const std::string& inServerAddress);

private:
    UDPSocketPtr mSocket;
    SocketAddressPtr mServerAddress;
};

#endif /* UDPCLIENT_HPP */