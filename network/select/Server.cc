#include <memory>

#include "SelectServer.hpp"


int main()
{
    std::unique_ptr<SelectServer> server(new SelectServer(8080));
    server->Start();
    return 0;
}