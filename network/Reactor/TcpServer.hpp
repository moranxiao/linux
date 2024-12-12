#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <cassert>

#include "Epoll.hpp"
#include "../Sock.hpp"
#include "../Log.h"
#include "Protocol.hpp"

class Connection;
class TcpServer;

using func_t = std::function<void(Connection*)>;
using callBack_t = std::function<void(const Request& ,Response&)>;

class Connection{
public:
    Connection(int sock,TcpServer* psvr)
    :_sock(sock),_psvr(psvr)
    {}
    void SetCallBack(func_t recv,func_t send,func_t except)
    {
        _cbRecv = recv;
        _cbSend = send;
        _cbExcept = except;
    }
public:
    int _sock;
    //回调函数
    func_t _cbRecv;
    func_t _cbSend;
    func_t _cbExcept;
    //缓冲区
    std::string _inbuffer;
    std::string _outbuffer;
    //server的回指针
    TcpServer* _psvr;
};


class TcpServer{
    const static int TIMEOUT = -1;
    const static int MAXEVENTS = 108;
public:
    TcpServer(callBack_t cb,uint16_t port,const std::string& ip = "0.0.0.0")
    :_ip(ip),_port(port),_timeout(TIMEOUT),_cb(cb)
    {
        //初始化listensock
        _listenSock = Sock::Socket(true);
        Sock::Bind(_listenSock,_ip,_port);
        Sock::Listen(_listenSock);
        
        //初始化接收事件数组
        _revts = new struct epoll_event[MAXEVENTS];
        _maxEvents = MAXEVENTS;

        //将listenSock构建一个对象管理起来
        AddConnection(_listenSock,std::bind(&TcpServer::Accepter,this,std::placeholders::_1),nullptr,nullptr);
        LOG(INFO) << "server initialize success\n";
    }
    void Dispatch()
    {
        while(1)
        {
            LoopOnce();
        }
    }
    ~TcpServer()
    {
        if(_revts) delete[] _revts;
        for(auto& e : _connections)
        {
            close(e.first);
            delete e.second;
        }
    }
    bool EnableReadWrite(int sock,bool read,bool write)
    {
        return _epoll.CtrlEpoll(sock,(read?EPOLLIN:0) | (write?EPOLLOUT:0));
    }
private:

    void AddConnection(int sock,func_t recv,func_t send,func_t except)
    {
        assert(sock >= 0);
        //创建Connection对象
        Connection* conn = new Connection(sock,this);
        //管理对象
        _connections.insert(std::make_pair(sock,conn));
        //设置连接的回调函数，每个连接会有相应事件的处理方法
        _connections[sock]->SetCallBack(recv,send,except);
        //对于ET模式下的epoll，接收到了事件必须一次性处理完，所以需要循环处理
        //为了使sock处理到最后不阻塞，必须设定为非阻塞状态
        Sock::SetNonBlock(sock);
        //添加sock事件进epoll
        _epoll.AddEvent(sock,EPOLLIN | EPOLLET);
    }
    void Accepter(Connection* conn)
    {  
        while(1)
        {
            int sock = Sock::Accept(_listenSock,nullptr,nullptr);
            if(sock >= 0)
            {
                AddConnection(sock,
                std::bind(&TcpServer::Recver,this,std::placeholders::_1),
                std::bind(&TcpServer::Sender,this,std::placeholders::_1),
                std::bind(&TcpServer::Excepter,this,std::placeholders::_1));
                LOG(INFO) << "获取到新连接 sock:" << sock << std::endl;
            }
            else{
                //因为是非阻塞，如果sock为-1有可能是读完了，也有可能是信号中断了，也有可能是真正的错误
                if(errno == EWOULDBLOCK && errno == EAGAIN)
                    break;
                else if(errno == EINTR) continue;
                else{
                    //发生错误
                    LOG(ERROR) << "获取链接错误\n"; 
                }
            }
        }
    }
    void Recver(Connection* conn)
    {
        int sock = conn->_sock;
        while(1)
        {
            char buffer[10240];
            ssize_t n = read(sock,buffer,sizeof(buffer)-1);
            if(n > 0){
                buffer[n] = 0;
                conn->_inbuffer += buffer;
            }
            else if(n == 0){
                //对方关闭连接
                conn->_cbExcept(conn);
                return;
            }
            else{
                if(errno == EAGAIN || errno == EWOULDBLOCK) break;
                else if(errno == EINTR) continue;
                else{
                    //发生错误
                    conn->_cbExcept(conn);
                    return;
                }  
            }
        }
        //将inbuffer内容序列化
        std::vector<Request> reqs;
        Encode(conn->_inbuffer,&reqs);
        for(auto& e : reqs)
        {
            Response resp; 
            _cb(e,resp);
            Decode(resp,&conn->_outbuffer);
        }
        conn->_psvr->EnableReadWrite(sock,true,true);

        LOG(INFO) << "sock[" << sock << ']' << ":inbuffer" << conn->_inbuffer << "\n";
        LOG(INFO) << "sock[" << sock << ']' << ":outbuffer" << conn->_outbuffer << "\n";
    }

    void Sender(Connection* conn)
    {
        int sock = conn->_sock;
        if(sock < 0) assert(false);
        while(1)
        {
            ssize_t n = write(sock,conn->_outbuffer.c_str(),conn->_outbuffer.size());
            if(n < 0){
                if(errno == EAGAIN || errno == EWOULDBLOCK)
                    break;
                else if(errno == EINTR) continue;
                else{
                    //出错
                    conn->_cbExcept(conn);
                    return;
                }
            }
            else if(n == 0){
                //对端关闭连接
                conn->_cbExcept(conn);
                return;
            }
            else{
                conn->_outbuffer.erase(0,n);
                if(conn->_outbuffer.empty()) break;
            }
        }
        if(conn->_outbuffer.empty())
        {
            EnableReadWrite(sock,true,false);
        }
        else{
            EnableReadWrite(sock,true,true);
        }
    }
    void Excepter(Connection* conn)
    {
        int sock = conn->_sock;
        _epoll.DeleteSock(sock);
        _connections.erase(sock);
        delete conn;
        close(sock);
        LOG(INFO) << "连接关闭:" << sock << std::endl;
    }
    void HandleEvent(int n)
    {
        for(int i = 0; i < n; i++)
        {
            int sock = _revts[i].data.fd;
            if(IsExistConnection(sock))
            {
                if(_revts[i].events & EPOLLHUP || _revts[i].events & EPOLLERR);
                if(_revts[i].events & EPOLLIN && _connections[sock]->_cbRecv != nullptr)
                    _connections[sock]->_cbRecv(_connections[sock]);
                if(_revts[i].events & EPOLLOUT && _connections[sock]->_cbSend != nullptr)
                {
                    _connections[sock]->_cbSend(_connections[sock]);
                } 
            }
            else{
                assert(false);
            }
        }
    }
    void LoopOnce()
    {
        int n = _epoll.EpollWait(_revts,_maxEvents,_timeout);
        switch(n)
        {
            case 0:
            LOG(INFO) << "time out\n";
            break;
            case -1:
            LOG(WARNING) << "epoll error\n";
            break;
            default:
            HandleEvent(n);
        }
    }
    bool IsExistConnection(int sock)
    {
        auto it = _connections.find(sock);
        if(it == _connections.end())
            return false;
        return true;
    }
private:
    int _listenSock;
    std::string _ip;
    uint16_t _port;
    Epoll _epoll;
    struct epoll_event* _revts;
    int _maxEvents;
    int _timeout;
    std::unordered_map<int,Connection*> _connections;
    callBack_t _cb;
};