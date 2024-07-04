#include "TcpServer.hpp"
#include "protocol.hpp"

using namespace NetCal;

void Usage(char* process)
{
    logMessage(ERROR,"Usage:./%s port",process);
}

void Task(int sockfd)
{
    while(true)
    {
        char buffer[1024];
        memset(buffer,0,sizeof buffer);
        Request req;
        int sz = read(sockfd,buffer,sizeof buffer);
        if(sz <= 0)
            break;
        req.DeSerialize(buffer);
        Response rep = req.Execute();

        std::string str = rep.Serialize();
        std::cout << req.Serialize() << std::endl;
        if(write(sockfd,str.c_str(),str.length())<= 0)
        {
            logMessage(WARING,"服务端发生结果失败");
            break;
        }           
    }
    close(sockfd);
}
int main(int argc,char* argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        return -1;
    }
    TcpServer server(atoi(argv[1]));
    server.pushTask(Task);
    server.start();
    return 0;
}