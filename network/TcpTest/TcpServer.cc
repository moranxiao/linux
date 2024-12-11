#include <memory>

#include "TcpServer.hpp"

int main()
{
    std::unique_ptr<TcpServer> server(new TcpServer(8080));
    server->Start();
    return 0;
}