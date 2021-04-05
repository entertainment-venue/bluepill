#include "socket_server.h"

namespace bluepill
{

    int SocketServer::CreateListenFd()
    {
        int listenfd = socket(AF_INET, SOCK_STREAM, 0);
        int val = 1;
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

        struct sockaddr_in listen_addr;
        listen_addr.sin_family = AF_INET;
        listen_addr.sin_addr.s_addr = INADDR_ANY;
        listen_addr.sin_port = htons(port_);
        if (bind(listenfd, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) < 0)
        {
            return -1;
        }

        if (listen(listenfd, backlog_) < 0)
        {
            return -1;
        }

        int opts;
        if ((opts = fcntl(listenfd, F_GETFL)) < 0)
        {
            return -1;
        }
        opts = opts | O_NONBLOCK;
        if (fcntl(listenfd, F_SETFL, opts) < 0)
        {
            return -1;
        }

        struct linger li;
        memset(&li, 0, sizeof(li));
        li.l_onoff = 1;
        li.l_linger = 0;

        int ret;
        if (0 != (ret = setsockopt(listenfd, SOL_SOCKET, SO_LINGER, (const char *)&li, sizeof(li))))
        {
            return -1;
        }
        int var = 1;
        if (0 != (ret = setsockopt(listenfd, IPPROTO_TCP, TCP_NODELAY, &var, sizeof(var))))
        {
            return -1;
        }
        return listenfd;
    }

    int SocketServer::Listen(int listenfd)
    {
        SocketPoll *socket_poll = createPoll();
        int efd = socket_poll->SpCreate();
        if (socket_poll->SpInvalid(efd))
        {
            return -1;
        }

        if (socket_poll->SpAdd(efd, listenfd, NULL))
        {
            return -1;
        }

        for (;;)
        {
            struct event ev[1];
            int event_n = socket_poll->SpWait(efd, ev, 1);
            if (event_n != 1)
            {
                continue;
            }

            struct sockaddr_in cliaddr;
            socklen_t len = sizeof(cliaddr);
            int connfd;
            if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len)) < 0)
            {
                continue;
            }
        }

        return 0;
    }

}