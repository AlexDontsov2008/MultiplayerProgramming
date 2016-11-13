#include "netinet/in.h"
#include "unistd.h"

#include "UDPSocket.hpp"
#include "SocketAddress.hpp"
#include "SocketUtil.hpp"

const int NO_ERROR = 0;

/** Уведомление ОС о том что сокет будет использовать определенный адрес и порт
 * @param Адрес/порт для связывания с соектом.
 * С Этого адреса сокет будет отправлять пакеты.
 */
int UDPSocket::Bind(const SocketAddress& inBindAddress) {
    // Выполняет связывание сокета с заданным адресом.
    // Выполняет 2 цели:
    // 1. Говорит ОС, что данный сокет будет получателем пакетов с местом назначения
    //      совпадающим с указанным адресом(портом).
    // 2. Обозначает source адрес и порт, которые socket lib должна использовать,
    //      когда создает  netrwok и transport layer headers для пакетов,
    //      отправляемых с этого сокета.
    int err = bind(mSocket, &inBindAddress.mSockAddr, inBindAddress.GetSize());
    if (err) {
        SocketUtil::ReportError("UDPSocket::Bind");
        return SocketUtil::GetLastError();
    }

    return NO_ERROR;
}

/** Отправляет данные на UDP socket
 * @param Данные для передачи
 * @param Размер передаваемых данных
 * @param Адрес получателя данных
 */
int UDPSocket::SendTo(const void* inData, int inLen, const SocketAddress& inTo) {
    int byteSentCount = sendto( mSocket, static_cast<const char*>(inData), inLen,
                                0, &inTo.mSockAddr, inTo.GetSize());

    if (byteSentCount >= 0) {
        return byteSentCount;
    } else {
        SocketUtil::ReportError("UDPSocket::SendTo");
        return -SocketUtil::GetLastError();
    }
}

/** Получение данных на UDP socket
 * @param Буфер для получения datagram's
 * @param Размер буфера для приема данных
 * @param Адрес отправителя данных
 */
int UDPSocket::ReceiveFrom(void* inBuffer, int inLen, SocketAddress& outFrom) {
    uint fromLength = outFrom.GetSize();
    int readByteCount = recvfrom(   mSocket, static_cast<char*>(inBuffer), inLen,
                                    0, &outFrom.mSockAddr, &fromLength);

    if (readByteCount >= 0) {
        return readByteCount;
    } else {
        SocketUtil::ReportError("UDPSocket::ReceiveFrom");
        return -SocketUtil::GetLastError();
    }
}

UDPSocket::~UDPSocket() {
    close(mSocket);
}