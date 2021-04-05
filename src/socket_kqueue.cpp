#include "socket_kqueue.h"

namespace bluepill
{

    bool SocketKqueue::SpInvalid(int kfd)
    {
        return kfd == -1;
    }

    int SocketKqueue::SpCreate()
    {
        return kqueue();
    }

    void SocketKqueue::SpRelease(int kfd)
    {
        close(kfd);
    }

    void SocketKqueue::SpDel(int kfd, int sock)
    {
        struct kevent ke;
        EV_SET(&ke, sock, EVFILT_READ, EV_DELETE, 0, 0, NULL);
        kevent(kfd, &ke, 1, NULL, 0, NULL);
        EV_SET(&ke, sock, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
        kevent(kfd, &ke, 1, NULL, 0, NULL);
    }

    int SocketKqueue::SpAdd(int kfd, int sock, void *ud)
    {
        struct kevent ke;
        EV_SET(&ke, sock, EVFILT_READ, EV_ADD, 0, 0, ud);
        if (kevent(kfd, &ke, 1, NULL, 0, NULL) == -1 || ke.flags & EV_ERROR)
        {
            return 1;
        }
        EV_SET(&ke, sock, EVFILT_WRITE, EV_ADD, 0, 0, ud);
        if (kevent(kfd, &ke, 1, NULL, 0, NULL) == -1 || ke.flags & EV_ERROR)
        {
            EV_SET(&ke, sock, EVFILT_READ, EV_DELETE, 0, 0, NULL);
            kevent(kfd, &ke, 1, NULL, 0, NULL);
            return 1;
        }
        EV_SET(&ke, sock, EVFILT_WRITE, EV_DISABLE, 0, 0, ud);
        if (kevent(kfd, &ke, 1, NULL, 0, NULL) == -1 || ke.flags & EV_ERROR)
        {
            SpDel(kfd, sock);
            return 1;
        }
        return 0;
    }

    int SocketKqueue::SpEnable(int kfd, int sock, void *ud, bool read_enable, bool write_enable)
    {
        int ret = 0;
        struct kevent ke;
        EV_SET(&ke, sock, EVFILT_READ, read_enable ? EV_ENABLE : EV_DISABLE, 0, 0, ud);
        if (kevent(kfd, &ke, 1, NULL, 0, NULL) == -1 || ke.flags & EV_ERROR)
        {
            ret |= 1;
        }
        EV_SET(&ke, sock, EVFILT_WRITE, write_enable ? EV_ENABLE : EV_DISABLE, 0, 0, ud);
        if (kevent(kfd, &ke, 1, NULL, 0, NULL) == -1 || ke.flags & EV_ERROR)
        {
            ret |= 1;
        }
        return ret;
    }

    int SocketKqueue::SpWait(int kfd, struct event *e, int max)
    {
        struct kevent ev[max];
        int n = kevent(kfd, NULL, 0, ev, max, NULL);

        int i;
        for (i = 0; i < n; i++)
        {
            e[i].s = ev[i].udata;
            unsigned filter = ev[i].filter;
            bool eof = (ev[i].flags & EV_EOF) != 0;
            e[i].write = (filter == EVFILT_WRITE) && (!eof);
            e[i].read = (filter == EVFILT_READ);
            e[i].error = (ev[i].flags & EV_ERROR) != 0;
            e[i].eof = eof;
        }

        return n;
    }

    void SocketKqueue::SpNonblocking(int fd)
    {
        int flag = fcntl(fd, F_GETFL, 0);
        if (-1 == flag)
        {
            return;
        }

        fcntl(fd, F_SETFL, flag | O_NONBLOCK);
    }
}