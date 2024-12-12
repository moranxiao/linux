#include <memory>
#include <unordered_map>
#include "TcpServer.hpp"

using calcul_t = std::function<int (int,int)>;

void CallBack(const Request& req,Response& resp)
{
    std::unordered_map<std::string,calcul_t> calculs;
    calculs["+"] = [](int x,int y){ return x+y;};
    calculs["-"] = [](int x,int y){ return x-y;};
    calculs["*"] = [](int x,int y){ return x*y;};
    calculs["/"] = [](int x,int y){ return x/y;};
    if(req.op == "/" && req.y == 0) 
    {
        resp.res = 0;
        resp.status = -1;
    }
    else{
        resp.res = calculs[req.op](req.x,req.y);
        resp.status = 0;
    }
}

int main()
{
    std::unique_ptr<TcpServer> server(new TcpServer(CallBack,8080));
    server->Dispatch();
    return 0;
}