/*
 * File:   SocketUtil.hpp
 * Author: dontsov
 *
 * Created on November 13, 2016, 1:48 PM
 */

#ifndef SOCKETUTIL_HPP
#define SOCKETUTIL_HPP

#include <vector>

#include "UDPSocket.hpp"
#include "TCPSocket.hpp"

enum SocketAddressFamily {
    INET = AF_INET,
    INET6 = AF_INET6
};

typedef std::vector<TCPSocketPtr> SocketsVector;

class SocketUtil {
public:
    static void ReportError(const char* inOperationDesc);
    static int GetLastError();

    static UDPSocketPtr CreateUDPSocket(SocketAddressFamily inFamily);
    static TCPSocketPtr CreateTCPSocket(SocketAddressFamily inFamily);

    /** Заполняет коллекцию сокетов из вектора
     * 
     * @param outSet - коллекция сокетов для заполнения
     * @param inSockets - исходный вектор сокетов
     * @param ioNaxNfds - максимальный пронумерованный сокет (Для Posixf-систем)
     * @return заполненная коллекция сокетов
     */
    static fd_set* FillSetFromVector(fd_set& outSet, const SocketsVector* inSockets, int& ioNaxNfds);

    /** Заполняет вектор из коллекции сокетов
     * 
     * @param outSockets - вектор, заполняемый методом
     * @param inSockets - вектор, содержащий все сокеты для проверки.
     * @param inSet - коллекция сокетов для заполнения
     */
    static void FillVectorFromSet(SocketsVector* outSockets, const SocketsVector* inSockets, const fd_set& inSet);

    /** Проверяет состояние переданных сокетов
    * 
    * @param inReadSet - доступные сокеты для чтения
    * @param outReadSet - сокеты готовые для чтения 
    * @param inWriteSet - доступные сокеты для записи
    * @param outWriteSet - сокеты готовые для записи
    * @param inExceptSet - доступные сокеты для проверки ошибок 
    * @param outExceptSet - сокеты готовые для проверки ошибок
    * @return количество готовых к действию сокетов
    */
    static int Select(  const SocketsVector* inReadSet, SocketsVector* outReadSet,
                        const SocketsVector* inWriteSet, SocketsVector* outWriteSet,
                        const SocketsVector* inExceptSet, SocketsVector* outExceptSet);
};

#endif /* SOCKETUTIL_HPP */