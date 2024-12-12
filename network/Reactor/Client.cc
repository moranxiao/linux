#include "../Sock.hpp"
#include "../Log.h"
#include "Protocol.hpp"

int main()
{
    int sock = Sock::Socket();
    Sock::Connect(sock,"127.0.0.1",8080);
    while(1)
    {
        Request req;
        std::cin >> req.x >> req.op >> req.y ;
        std::string buffer = req.Serialize();
        std::string outbuffer = std::to_string(buffer.size()) + "||" + buffer;
        int n = write(sock,outbuffer.c_str(),outbuffer.size());
        
        char inbuffer[10240];
        n = read(sock,inbuffer,sizeof(buffer)-1);
        inbuffer[n] = 0;
        std::cout << inbuffer << std::endl;
    }


    return 0;
}