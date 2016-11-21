/* 
 * File:   TCPSocket.hpp
 * Author: dontsov
 *
 * Created on November 14, 2016, 10:01 PM
 */

#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP

#include <memory>
#include <string>

#include "SocketSharedInfo.hpp"

class SocketUtil;
class SocketAddress;

class TCPSocket {
public:
    ~TCPSocket();

    /** Инициирует TCP handshake отправляя SYN пакет целевому хосту
     * 
     * @param inAddress - адрес удаленного хоста
     * @return состояние выполнения
     */
    int Connect(const SocketAddress& inAddress);

    /** Служит для прослушивания поступающих handshake
     * 
     * @param inBackLog - максимальное количество входящих подключений
     * @return состояние выполнение
     */
    int Listen(int inBackLog = SOMAXCONN);

    /** Согласие на входящее соединение, для продолжения TCP handshake
     * 
     * @param inFromAddress - адрес удаленного хоста которые инициировал соединение
     * @return сокет, для взаимодействия с удаленным host
     */
    std::shared_ptr<TCPSocket> Accept(SocketAddress& inFromAddress);

    /** Отправка данных через TCPSocket
     * 
     * @param inData - данные для передачи, помещаются в socket's send buffer
     * @param inLen - размер данных для передачи
     * @return количество переданных данных
     */
    int Send(const void* inData, int inLen);

    /** Прием данных через TCPSocket
     * 
     * @param inBuffer - буфер для копирования данных из socket's receive buffer
     * @param inLen - размер буфера для получения данных
     * @return размер полученных данных
     */
    int Receive(void* inBuffer, int inLen);

    /** Связывает сокет с выбранным адресом
     * 
     * @param inToAddress - адрес с которым происходит связывание
     * @return состояние выполнения
     */
    int Bind(const SocketAddress& inToAddress);

    /** Устанавливает сокет в non-blocking mode
     * 
     * @return состояние выполнения
     */
    int SetNonBlockingMode();

    /** Устанавливает сокет в blocking mode
     * 
     * @return состояние выполнения
     */
    int SetBlockingMode();

private:
    friend class SocketUtil;
    TCPSocket(SOCKET inSocket) : mSocket(inSocket) {}

    /** Устанавливает сокет в указанное состояние
     * 
     * @param inFlags - задают состояние сокета
     * @param inInvokedMethodName - имя вызывающего метода
     * @return состояние выполнения
     */
    int ChangeBlockingMode(int inFlags, const std::string& inInvokedMethodName);

private:
    SOCKET mSocket;
};

typedef std::shared_ptr<TCPSocket> TCPSocketPtr;

#endif /* TCPSOCKET_HPP */