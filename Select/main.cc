#include "SelectServer.hpp"
#include <memory>


int main()
{
    std::unique_ptr<SelectServer> server(new SelectServer());
    server->Start();
    return 0;
}