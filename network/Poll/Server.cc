#include <memory>

#include "PollServer.hpp"


int main()
{
    std::unique_ptr<PollServer> server(new PollServer(8080));
    server->Start();
    return 0;
}