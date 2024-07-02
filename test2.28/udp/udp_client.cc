#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <string>
#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <memory>

#include "../log.hpp"
#include "../thread.hpp"

class Data{
public:
    int _sock;
    uint16_t _port;
    std::string _ip;
};

static void* send(void* args)
{
    threadData* td = (threadData*)args;
    Data* data = (Data*)td->_args;
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(data->_ip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(data->_port); 
    std::cout << td->_name << ":launch success" <<  std::endl; 
    char buffer[1024];
    while(true)
    {
        std::cout << "please input>:";
        gets(buffer);
        if(!strcmp(buffer,"quit")) break;
        sendto(data->_sock,buffer,sizeof(buffer), 0, (struct sockaddr*)&server, sizeof(sockaddr_in));
        
    }
    
    return nullptr;
}

static void* receive(void* args)
{
    threadData* td = (threadData*)args;
    Data* data = (Data*)td->_args;
    std::cout << td->_name << ":launch success" << std::endl; 
    char buffer[1024];
    while(true)
    {
        struct sockaddr_in tmp;
        socklen_t len = sizeof(tmp);
        ssize_t s = recvfrom(data->_sock, buffer, sizeof(buffer), 0,(struct sockaddr*)&tmp, &len);
        if(s > 0)
        {
            std::cout << "receive information:>" << buffer << std::endl;
        }
    }
    return nullptr;
}

static void Usage()
{
    logMessage(ERROR,"please input:>./udp_client x.x.x.x port");
}

int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        Usage();
        exit(0);
    }
    uint16_t server_port = atoi(argv[2]);
    std::string server_ip = argv[1];
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
    {
        logMessage(FATAL, "client create sock failed");
        exit(0);
    }
    logMessage(NORMAL, "client create sock success");
    

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(server_ip.c_str());
    server.sin_port = htons(server_port);

    Data d;
    d._ip = server_ip;
    d._port = server_port;
    d._sock = sock;
    
    std::unique_ptr<Thread> sender(new Thread(1,send,(void*)&d));
    std::unique_ptr<Thread> receiver(new Thread(2,receive, (void*)&d));
    sender->start();
    receiver->start();

    sender->join();
    receiver->join();
    // std::string line;
    // while(line != "quit")
    // {
    //     std::cout << "please input your message:>";
    //     std::getline(std::cin,line);
    //     sendto(sock, line.c_str(), line.size(), 0, (struct sockaddr*)&server, sizeof(server));
        
    //     char buffer[1024];
    //     memset(buffer, 0, sizeof(buffer));
    //     socklen_t len = sizeof(server);
    //     ssize_t s = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&server,&len);
    //     if(s > 0)
    //     {
    //         std::cout << "server echo:" << buffer << std::endl;
    //     }
    // }
    close(sock);
    return 0;
}