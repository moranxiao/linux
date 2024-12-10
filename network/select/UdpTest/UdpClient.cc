#include "../Sock.hpp"
#include "../Log.h"

int main()
{
    int sock = Sock::Socket(false);
    char buffer[] = "hello udpserver!";
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(sendto(sock,buffer,sizeof(buffer),0,(struct sockaddr*)&server,sizeof(server)) == sizeof(buffer))
        LOG(INFO) << "发送成功" << "\n";
    return 0;
}