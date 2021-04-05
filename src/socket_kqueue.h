#ifndef BLUEPILL_SOCKET_KQUEUE_H
#define BLUEPILL_SOCKET_KQUEUE_H

#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/event.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "socket_poll.h"

// https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man2/kqueue.2.html
// https://sjmulder.nl/dl/pdf/unsorted/2000%20-%20Lemon%20-%20Kqueue,%20A%20generic%20and%20scalable%20event%20notification%20facility.pdf

namespace bluepill
{
    class SocketKqueue : public SocketPoll
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

#endif