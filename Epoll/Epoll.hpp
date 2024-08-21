#pragma once

#include <sys/epoll.h>
#include "Log.hpp"

class Epoll{
public:
    static int EpollCreate()
    {
        int sock = epoll_create(512);
        if(sock < 0)
        {
            logMessage(ERROR,"Epoll create error");
            exit(5);
        }
        return sock;
    }
    static bool AddEpollIN(int epollfd,int fd)
    {
        struct epoll_event ev;
        ev.data.fd = fd;
        ev.events = EPOLLIN;
        int n = epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
        return n == 0;
    }
    static int EpollWait(int epollfd,struct epoll_event evs[],int maxevents,int timeout)
    {
        return epoll_wait(epollfd,evs,maxevents,timeout);
    }
    static bool EpollDelete(int epollfd,int fd)
    {
        int n = epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,nullptr);
        return n == 0;
    }
};