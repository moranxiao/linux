#pragma once 

#include "Epoll.hpp"
#include "Sock.hpp"
#include "Log.hpp"

class EpollServer{
    static const int g_timeout = 5000;
    static const int g_maxevents = 128;
public:
    void Accepter()
    {
        std::string clientip;
        uint16_t clientport;
        int sockfd = _sock.Accept(_listensock,&clientip,&clientport);
        if(sockfd >= 0)
        {
            Epoll::AddEpollIN(_epollfd,sockfd);
            logMessage(DEBUG,"add sockfd:%d",sockfd);
        }
    }
    void Recver(int sockfd)
    {
        char buffer[1024];
        int n = recv(sockfd,buffer,sizeof(buffer)-1,0);
        if(n < 0)
        {
            logMessage(WARING,"recv from sock[%d] error", sockfd);
        }
        else if (n == 0)
        {
            Epoll::EpollDelete(_epollfd,sockfd);
            close(sockfd);
            logMessage(DEBUG,"sockfd[%d] closed",sockfd);
        }
        else
        {
            buffer[n-1] = 0;
            logMessage(DEBUG,"echo:%s",buffer);
        }
    }
public:
    EpollServer(uint16_t port = 8080,int maxevents = g_maxevents)
    :_maxevents(maxevents),
    _timeout(g_timeout),
    _port(port)
    {
        _epollfd = Epoll::EpollCreate();
        _listensock = _sock.Socket();
        _sock.Bind(_listensock,_port);
        _sock.Listen(_listensock);
        if(!Epoll::AddEpollIN(_epollfd,_listensock))
        {
            logMessage(ERROR,"add listensock epollin error");
            exit(6);
        }    
        _evs = new struct epoll_event[_maxevents];
        logMessage(DEBUG,"server initiate success");
    }
    void Dispatcher()
    {
        char buffer[1024];
        while(true)
        {
            int n = Epoll::EpollWait(_epollfd,_evs,_maxevents,_timeout);
            if(n > 0)
            {
                for(int i = 0; i < n; i++)
                {
                    int sockfd = _evs[i].data.fd;
                    if(sockfd == _listensock && _evs[i].events & EPOLLIN)
                    {
                        Accepter();
                    }
                    else if(_evs[i].events & EPOLLIN)
                    {
                        Recver(sockfd);
                    }
                }
            }
            else if(n == 0)
            {
                logMessage(DEBUG,"timeout");
            }
            else
            {
                logMessage(ERROR,"EpollWait error");
            }
        }
    }
    ~EpollServer()
    {
        if(_listensock >= 0)
            close(_listensock);
        if(_epollfd >= 0)
            close(_epollfd);
        if(_evs)
            delete[] _evs;
    }
private:
    uint16_t _port;
    int _listensock;
    Sock _sock;
    int _epollfd;
    struct epoll_event* _evs;
    int _maxevents;
    int _timeout;
};