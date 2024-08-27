#pragma once

#include <functional>
#include "Epoll.hpp"
#include "Sock.hpp"

class TcpServer;
class Connection;

using Func_t = void(*)(Connection*);

class Connection{
public:
    Connection(int sockfd,Func_t recver,Func_t sender,Func_t excepter)
    :_sockfd(sockfd),
    _recver(recver),
    _sender(sender),
    _excepter(excepter)
    {

    }
public:
    int _sockfd;
    Func_t _recver;
    Func_t _sender;
    Func_t _excepter;
    std::string* inbuffer;
    std::string* outbuffer;
    TcpServer* _server;
};


class TcpServer{
private:
    const static uint16_t gport = 8080;
public:
    TcpServer(uint16_t port = gport)
    :_port(port)
    {
        _listensock = _sock.Socket();
        _sock.Bind(_listensock,_port);
        _sock.Listen(_listensock);
        
    }
private:
    int _listensock;
    uint16_t _port;
    Sock _sock;
    int _pollsock;
};