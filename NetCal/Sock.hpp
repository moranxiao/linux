#include "Log.hpp"
#include <sys/socket.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>


class Sock{
public:
    int sock()
    {
        int sockfd = socket(AF_INET,SOCK_STREAM,0);
        if(sockfd < 0)
        {
            logMessage(ERROR,"获取套接字错误");
        }
    }
    void bindSock(int sockfd,const std::string& ip,uint16_t port)
    {
        struct sockaddr_in addr; 
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET,ip.c_str(),&addr.sin_addr.s_addr);       
        if(bind(sockfd,(struct sockaddr)&addr,sizeof(addr)) == -1)
        {
            logMessage(ERROR,"bind");
        }
    }
    void listenSock()
    {

    }
    


};