
#include "UdpServer.hpp"

int main()
{
    UdpServer server(8080);
    while(1)
    {
        std::string buffer;
        std::string client_ip;
        uint16_t client_port;
        server.Recvfrom(&buffer,1024,&client_ip,&client_port);
        LOG(INFO) << "客户端[" << client_ip << "]-[" << client_port << "]:" << buffer << '\n'; 
    }
    return 0;
}