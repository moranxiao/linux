#include <memory>
#include "Protocol.hpp"
#include "TcpServer.hpp"



int main()
{
    std::unique_ptr<TcpServer> server(new TcpServer());
        
    return 0;
}