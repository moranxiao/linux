#pragma once

#include "../Sock.hpp"
#include "../Log.h"

#include <cstring>
class UdpServer{
public:
    UdpServer(uint16_t port,const std::string& ip = "0.0.0.0")
        :_ip(ip),_port(port)
    {
        _sockfd = Sock::Socket(false);
        Sock::Bind(_sockfd,ip,port);

        LOG(INFO) << "服务器初始化成功" << std::endl;        
    }

    ssize_t Recvfrom(std::string* buff,size_t len, std::string* client_ip,uint16_t* client_port)
    {
        if(buff == nullptr) return 0;
        struct sockaddr_in client;
        memset(&client,0,sizeof client);
        socklen_t socklen = sizeof(client);
        char buffer[10240];
        
        int n = recvfrom(_sockfd,buffer,len,0,(struct sockaddr*)&client,&socklen);
        if(n < 0)
        {
            LOG(ERROR) << "recvfrom"  << '\n';
            if(errno == EAGAIN || errno == EWOULDBLOCK) LOG(DEBUG) << "非阻塞\n";
            else if(errno == ENOTCONN) LOG(DEBUG) << "ENOTCONN\n";
            LOG(DEBUG) << errno << '\n';
            return -1;
        } 
        if(client_ip)
        {
            char ip[32];
            *client_ip = inet_ntop(AF_INET,&client.sin_addr,ip,sizeof(client));    
        }
        if(client_port)
        {
            *client_port = ntohs(client.sin_port);
        }
        *buff = buffer;
        LOG(INFO) << *client_ip << ' ' << *client_port << '\n';
        return n;
    }


private:
    std::string _ip;
    uint16_t _port;
    int _sockfd;
};