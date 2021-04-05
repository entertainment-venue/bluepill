#ifndef BLUEPILL_SOCKET_EPOLL_H
#define BLUEPILL_SOCKET_EPOLL_H

#include <netdb.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "socket_poll.h"

namespace bluepill
{
    class SocketEpoll : public SocketPoll
    {
    protected:
        bool SpInvalid(int kfd);
        int SpCreate();
        void SpRelease(int kfd);
        void SpDel(int kfd, int sock);
        int SpAdd(int kfd, int sock, void *ud);
        int SpEnable(int kfd, int sock, void *ud, bool read_enable, bool write_enable);
        int SpWait(int kfd, struct event *e, int max);
        void SpNonblocking(int fd);
    };
}