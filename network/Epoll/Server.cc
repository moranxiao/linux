#include <memory>

#include "EpollServer.hpp"

int main()
{
    std::unique_ptr<EpollServer> server(new EpollServer(8080));
    server->Start();
    return 0;
}