#include <memory>
#include "EpollServer.hpp"

int main()
{
    std::unique_ptr<EpollServer> server(new EpollServer);
    server->Dispatcher();
    return 0;
}