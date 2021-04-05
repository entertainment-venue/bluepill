#ifndef BLUEPILL_SOCKET_SERVER_H
#define BLUEPILL_SOCKET_SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

#include "socket_poll.h"

#ifdef __linux__
#include "socket_epoll.h"
#endif

#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
#include "socket_kqueue.h"
#endif

namespace bluepill
{
    #define MAX_EVENT 64

    SocketPoll *createPoll()
    {
#ifdef __linux__
#include "socket_epoll.h"
        return new SocketEpoll();
#endif

#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
        return new SocketKqueue();
#endif
    }

    class SocketServer
    {
    public:
        SocketServer() {}
        ~SocketServer() {}

        int CreateListenFd();
        int Listen(int listenfd);

    private:
        int port_;

        // The maximum number of connections the kernel should queue for this socket.
        int backlog_;
    };
}

#endif