/* 
 * File:   SocketSharedInfo.hpp
 * Author: dontsov
 *
 * Created on November 16, 2016, 9:07 PM
 */

#ifndef SOCKETSHAREDINFO_HPP
#define SOCKETSHAREDINFO_HPP

#ifdef _WIN32
#define WIN32_LEAD_AND_MEAN
#define NOMINAX

#include "WinSock2.h"
#include "Windows.h"
#include "Ws2tcpip.h"
typedef int socklen_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

typedef int SOCKET;
const int NO_ERROR = 0;
const int INVALID_SOCKET = -1;
const int SOCKET_ERROR = -1;
#endif

#endif /* SOCKETSHAREDINFO_HPP */