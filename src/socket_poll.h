#ifndef BLUEPILL_SOCKET_POLL_H
#define BLUEPILL_SOCKET_POLL_H

// Refer from https://github.com/cloudwu/skynet/blob/master/skynet-src/socket_poll.h

#include <stdbool.h>

namespace bluepill
{
    struct event
    {
        void *s;
        bool read;
        bool write;
        bool error;
        bool eof;
    };

    class SocketPoll
    {
    public:
        virtual bool SpInvalid(int kfd) = 0;
        virtual int SpCreate() = 0;
        virtual void SpRelease(int kfd) = 0;
        virtual void SpDel(int kfd, int sock) = 0;
        virtual int SpAdd(int kfd, int sock, void *ud) = 0;
        virtual int SpEnable(int kfd, int sock, void *ud, bool read_enable, bool write_enable) = 0;
        virtual int SpWait(int kfd, struct event *e, int max) = 0;
        virtual void SpNonblocking(int fd) = 0;

    protected:
        virtual ~SocketPoll() {}
    };
}

#endif