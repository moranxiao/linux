#include <memory>
#include "Protocol.hpp"
#include "TcpServer.hpp"

//x + y
void NetCal(Connection* conn,const std::string& msg)
{
    Request req;
    if(!req.DeSerialize(msg)) return;
    Response res = req.Execute();
    std::string buffer = res.Serialize();
    conn->outbuffer += buffer;
        
    conn->_server->EnableReadWrite(conn->_sockfd,true,true);
       
}

int main()
{
    std::unique_ptr<TcpServer> server(new TcpServer());
    server->Dispacher(NetCal);
    return 0;
}