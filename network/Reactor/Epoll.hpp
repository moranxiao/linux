#pragma once

#include <sys/epoll.h>
#include <unistd.h>
#include <cstdlib>

class Epoll{
public:
    Epoll()
    {
        _epollFd = epoll_create(512);
        if(_epollFd < 0) exit(5);
    }
    bool AddEvent(int sock,uint32_t events)
    {
        struct epoll_event evt;
        evt.data.fd = sock;
        evt.events = events;
        if(!epoll_ctl(_epollFd,EPOLL_CTL_ADD,sock,&evt)) return true;
        return false;
    }
    bool CtrlEpoll(int sock, uint32_t events)
    {
        events |= EPOLLET;
        struct epoll_event ev;
        ev.events = events;
        ev.data.fd = sock;
        int n = epoll_ctl(_epollFd, EPOLL_CTL_MOD, sock, &ev);
        return n == 0;
    }
    bool DeleteSock(int sock)
    {
        if(epoll_ctl(_epollFd,EPOLL_CTL_DEL,sock,nullptr)) return false;
        return true;
    }
    int EpollWait(struct epoll_event* revts,int maxEvents,int timeout)
    {
        return epoll_wait(_epollFd,revts,maxEvents,timeout);
    }
    ~Epoll()
    {
        if(_epollFd >= 0) close(_epollFd);
    }
private:
    int _epollFd;
};