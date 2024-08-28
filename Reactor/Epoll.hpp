#pragma once

#include <sys/epoll.h>
#include <unistd.h>



class Epoll{
private:
    static const int gnum = 128;
    static const int gtimeout = 5000;


public:
    Epoll(int timeout = gtimeout)
    :_timeout(timeout)
    {
        CreateEpoll();
    }
    void CreateEpoll()
    {
        _epollfd = epoll_create(gnum);
        if(_epollfd < 0)
            exit(5);
    }
    bool AddSockToEpoll(int sockfd,uint32_t events)
    {
        struct epoll_event ev;
        ev.data.fd = sockfd;
        ev.events = events;
        int n = epoll_ctl(_epollfd,EPOLL_CTL_ADD,sockfd, &ev);
        return  n == 0;
    }
    int WaitEvents(epoll_event* evs,int maxevents)
    {
        int n = epoll_wait(_epollfd,evs,maxevents,_timeout);
        return n;
    }
    bool CtlEpoll(int sockfd,uint32_t events)
    {
        struct epoll_event ev;
        ev.data.fd = sockfd;
        ev.events = events;
        return epoll_ctl(_epollfd,EPOLL_CTL_MOD,sockfd,&ev) == 0;
    }
    bool DeleteSock(int sockfd)
    {
        return epoll_ctl(_epollfd,EPOLL_CTL_DEL,sockfd,nullptr) == 0;
    }
    ~Epoll()
    {
        if(_epollfd >= 0) close(_epollfd);
    }
private:
    int _epollfd;
    int _timeout;
};