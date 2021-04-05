#include "socket_epoll.h"

namespace bluepill
{

    bool SocketEpoll::SpInvalid(int efd)
    {
        return efd == -1;
    }

    int SocketEpoll::SpCreate()
    {
        return epoll_create(1024);
    }

    void SocketEpoll::SpRelease(int efd)
    {
        close(efd);
    }

    int SocketEpoll::SpAdd(int efd, int sock, void *ud)
    {
        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.ptr = ud;
        if (epoll_ctl(efd, EPOLL_CTL_ADD, sock, &ev) == -1)
        {
            return 1;
        }
        return 0;
    }

    void SocketEpoll::SpDel(int efd, int sock)
    {
        epoll_ctl(efd, EPOLL_CTL_DEL, sock, NULL);
    }

    int SocketEpoll::SpEnable(int efd, int sock, void *ud, bool read_enable, bool write_enable)
    {
        struct epoll_event ev;
        ev.events = (read_enable ? EPOLLIN : 0) | (write_enable ? EPOLLOUT : 0);
        ev.data.ptr = ud;
        if (epoll_ctl(efd, EPOLL_CTL_MOD, sock, &ev) == -1)
        {
            return 1;
        }
        return 0;
    }

    int SocketEpoll::SpWait(int efd, struct event *e, int max)
    {
        struct epoll_event ev[max];
        int n = epoll_wait(efd, ev, max, -1);
        int i;
        for (i = 0; i < n; i++)
        {
            e[i].s = ev[i].data.ptr;
            unsigned flag = ev[i].events;
            e[i].write = (flag & EPOLLOUT) != 0;
            e[i].read = (flag & EPOLLIN) != 0;
            e[i].error = (flag & EPOLLERR) != 0;
            e[i].eof = (flag & EPOLLHUP) != 0;
        }

        return n;
    }

    void SocketEpoll::SpNonblocking(int fd)
    {
        int flag = fcntl(fd, F_GETFL, 0);
        if (-1 == flag)
        {
            return;
        }

        fcntl(fd, F_SETFL, flag | O_NONBLOCK);
    }
}

#endif