#pragma once

#include <sys/epoll.h>
#include <cstdlib>
#include <unistd.h>

class Epoll{
public:
    Epoll()
    {
        _epoll = epoll_create(512);
        if(_epoll < 0)
            exit(5);
    }
    bool AddEventToEpoll(int sock,uint32_t events)
    {
        struct epoll_event evt;
        evt.data.fd = sock;
        evt.events = events;
        if(epoll_ctl(_epoll,EPOLL_CTL_ADD,sock,&evt) == -1)
            return false;
        return true;
    }
    bool DeleteSockFromEpoll(int sock)
    {
        if(epoll_ctl(_epoll,EPOLL_CTL_DEL,sock,nullptr) == -1) return false;
        return true;
    }
    int EpollWait(struct epoll_event* revts,int maxEvents,int timeout)
    {
        return epoll_wait(_epoll,revts,maxEvents,timeout);
    }
    ~Epoll()
    {
        if(_epoll >= 0) close(_epoll);
    }
private:
    int _epoll;
};