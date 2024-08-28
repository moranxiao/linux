#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>
#include "Epoll.hpp"
#include "Sock.hpp"

class TcpServer;
class Connection;

using func_t = std::function<void (Connection*)>;
using callback_t = std::function<void (Connection*,std::string&)>;

class Connection{
public:
    Connection(int sockfd,func_t recver,func_t sender,func_t excepter,TcpServer* server)
    :_sockfd(sockfd),
    _recver(recver),
    _sender(sender),
    _excepter(excepter),
    _server(server)
    {

    }
public:
    int _sockfd;
    func_t _recver;
    func_t _sender;
    func_t _excepter;
    std::string inbuffer;
    std::string outbuffer;
    TcpServer* _server;
};


class TcpServer{
private:
    const static uint16_t gport = 8080;
    const static int gnum = 128;

    bool AddConnection(int sockfd,func_t recver,func_t sender,func_t excepter)
    {
        _sock.SetNonBlock(sockfd);
        if(!_epoll.AddSockToEpoll(sockfd,EPOLLIN|EPOLLET))
            return false;
        Connection* conn = new Connection(sockfd,recver,sender,excepter,this);
        _connections.insert(std::make_pair(sockfd,conn));
        return true;
    }
    bool IsConnectionExist(int sockfd)
    {
        auto iter = _connections.find(sockfd);
        if(iter == _connections.end())
            return false;
        return true;
    }
public:
    TcpServer(uint16_t port = gport)
    :_port(port),
    _revs_num(gnum)
    {
        _listensock = _sock.Socket();
        _sock.Bind(_listensock,_port);
        _sock.Listen(_listensock);
        _revs = new struct epoll_event[_revs_num];
        if(!AddConnection(_listensock,std::bind(&TcpServer::Accepter,this,std::placeholders::_1),nullptr,nullptr))
            exit(6);
    }
    void Dispacher(callback_t cb)
    {
        _cb = cb;
        while(true)
        {
            LoopOnce();
        }
    }
    void LoopOnce()
    {
        int n = _epoll.WaitEvents(_revs,_revs_num);
        for(int i = 0; i < n ;i++)
        {
            int sockfd = _revs[i].data.fd;
            uint32_t events= _revs[i].events;
            if(events & EPOLLHUP || events & EPOLLERR) events |= (EPOLLIN | EPOLLOUT);

            if(events & EPOLLIN)
            {
                if(IsConnectionExist(sockfd) && _connections[sockfd]->_recver)
                    _connections[sockfd]->_recver(_connections[sockfd]);
            }
            else if(events & EPOLLOUT)
            {
                if(IsConnectionExist(sockfd) && _connections[sockfd]->_sender)
                    _connections[sockfd]->_sender(_connections[sockfd]);
            }
        }
    }
    void Accepter(Connection* conn)
    {
        while(true)
        {
            int accept_errno = 0;
            std::string client_ip;
            uint16_t client_port;
            int sockfd = _sock.Accept(conn->_sockfd,&client_ip,&client_port,&accept_errno);
            if(sockfd < 0)
            {
                if(accept_errno == EWOULDBLOCK || accept_errno == EAGAIN)
                    break;
                else if(accept_errno == EINTR) continue;
                else
                {
                    logMessage(WARING,"accept sockfd error");
                    return;
                }
            }
            else
            {
                AddConnection(sockfd,
                std::bind(&TcpServer::Recver,this,std::placeholders::_1),
                std::bind(&TcpServer::Sender,this,std::placeholders::_1),
                std::bind(&TcpServer::Excepter,this,std::placeholders::_1));
                logMessage(DEBUG,"addconnection success,sockfd:%d",sockfd);
                break;  
            }
        }
    }
    void Recver(Connection* conn)
    {
        char buffer[1024];
        bool err = false;
        while(true)
        {
            int sockfd = conn->_sockfd;
            errno = 0;
            int n = recv(sockfd,buffer,sizeof(buffer)-1,0);
            if(n > 0)
            {
                buffer[n] = 0;
                conn->inbuffer += buffer;           
            }
            else if(n == 0)
            {
                conn->_excepter(conn);
                err = true;
                break;
            }
            else
            {
                if(errno == EWOULDBLOCK || errno == EAGAIN)
                {
                    break;
                }
                else if(errno == EINTR)
                {
                    continue;
                }
                else
                {
                    conn->_excepter(conn);
                    err = true;
                    break;
                }
            }
        }
        logMessage(DEBUG,"inbuffer:%s",conn->inbuffer.c_str());
        if(!err)
        {
            std::vector<std::string> message;
            SplitMessage(conn->inbuffer,&message);
            for(auto &it : message)
            {
                std::cout << it << std::endl;
                _cb(conn,it);
            }
        }
    }
    void Sender(Connection* conn)
    {
        int sockfd = conn->_sockfd;
        while(true)
        {
            ssize_t n = send(sockfd,conn->outbuffer.c_str(),conn->outbuffer.size(),0);
            if(n > 0)
            {
                conn->outbuffer.erase(0,n);
                if(conn->outbuffer.empty()) break;
            }
            else
            {
                if(errno == EAGAIN || errno == EWOULDBLOCK) break;
                else if(errno == EINTR) continue;
                else
                {
                    logMessage(ERROR, "send error, %d : %s", errno, strerror(errno));
                    conn->_excepter(conn);
                    break;
                }
            }
        }
        if(conn->outbuffer.empty())  EnableReadWrite(sockfd,true,false);
        else  EnableReadWrite(sockfd,true,true);
    }
    void Excepter(Connection* conn)
    {
        if(!IsConnectionExist(conn->_sockfd)) return;
        if(!_epoll.DeleteSock(conn->_sockfd))
        {
            logMessage(WARING,"delete sock from epoll error");
            return;
        }
        logMessage(DEBUG,"Excepter:close sockfd[%d]",conn->_sockfd);
        _connections.erase(conn->_sockfd);
        close(conn->_sockfd);
        delete conn;        
    }
    void EnableReadWrite(int sockfd,bool read,bool write)
    {
        uint32_t events = (read ? EPOLLIN : 0) | (write ? EPOLLOUT : 0);
        if(!_epoll.CtlEpoll(sockfd,events))
            logMessage(ERROR,"CtlEpoll error,sock:%d",sockfd);
        
    }
    ~TcpServer()
    {
        if(_listensock >= 0) close(_listensock);
        if(_revs) delete[] _revs;
        for(auto& it : _connections)
            delete it.second;
    }
private:
    int _listensock;
    uint16_t _port;
    Sock _sock;

    std::unordered_map<int,Connection*> _connections;
    Epoll _epoll;
    struct epoll_event* _revs;
    int _revs_num;
    
    callback_t _cb;
};