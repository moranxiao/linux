#include "PollServer.hpp"
#include <memory>

int main()
{
    std::unique_ptr<PollServer> server(new PollServer());
    server->Start();
    return 0;
}