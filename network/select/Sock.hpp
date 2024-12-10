#pragma once
#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

class Sock{
    static const int gbacklog = 15;
public:
    //创建套接字
    static int Socket(bool is_tcp = true)
    {
        int sock;
        if(is_tcp)
            sock = socket(AF_INET,SOCK_STREAM,0);
        else
            sock = socket(AF_INET,SOCK_DGRAM,0);
        if(sock < 0) exit(1);
        return sock;
    }

    //绑定ip和端口
    static void Bind(int sock,const std::string& ip,uint16_t port)
    {
        struct sockaddr_in local;
        local.sin_family = AF_INET;
        //网络协议决定字节序为大端，所以需要用htons更正小端
        local.sin_port = htons(port);
        //ip也是同理
        local.sin_addr.s_addr = ip.empty() ? INADDR_ANY : inet_addr(ip.c_str());
        if(bind(sock,(struct sockaddr*)&local,sizeof(local)))
        {
            exit(2);
        }
    }

    static void Listen(int listen_sock)
    {
        if(listen(listen_sock,gbacklog))
            exit(3);
    }
    static int Accept(int listen_sock,std::string* ip,uint16_t* port)
    {
        struct sockaddr_in client;
        socklen_t sockaddr_len;
        int sock = accept(listen_sock,(struct sockaddr*)&client,&sockaddr_len);
        if(ip)
        {
            char des[32];
            *ip = inet_ntop(AF_INET,&client.sin_addr,des,sizeof(client));
        } 
        if(port) *port = ntohs(client.sin_port);
        return sock;
    }
    static void Connect(int sock,const std::string& server_ip,uint16_t server_port)
    {
        struct sockaddr_in server;
        server.sin_addr.s_addr = inet_addr(server_ip.c_str());
        server.sin_family = AF_INET;
        server.sin_port = htons(server_port);
        if(connect(sock,(struct sockaddr*)&server,sizeof(server)))
            exit(4);
    }
    //size_t 是 unsigned int
    //ssize_t 就是 int
};

