#include "HttpServer.hpp"
#include "Log.hpp"
#include "Usage.hpp"
#include "Sock.hpp"
#include <memory>
#include <vector>
#include "Utility.hpp"
#include <iostream>
#include <fstream>


#define ROOT "./wwwtest"
#define HOME_PAGE "/index.html"

void HanderHttpRequest(int sockfd)
{
    char buffer[10240];

    ssize_t sz = recv(sockfd,buffer,sizeof(buffer)-1,0);
    if(sz <= 0) return;
    buffer[sz] = 0;
    std::cout << buffer << "----------------\n" << std::endl;
    
    std::vector<std::string> vline;
    CutString(buffer,"\r\n",&vline);
    std::vector<std::string> vblock;
    CutString(vline[0]," ", &vblock);

    std::string file = vblock[1];
    std::string target = ROOT;
    
    if(file == "/") file = HOME_PAGE;
    target += file;

    std::ifstream in(target);
    std::string content;

    if(in.is_open())
    {
        std::string line;
        while(getline(in,line))
        {
            content += line;
        }
        in.close();
    }

    std::string httpResponse = "HTTP/1.1 200 OK\r\n";
    httpResponse += "\r\n";
    httpResponse += content;
    
    send(sockfd,httpResponse.c_str(),httpResponse.size(),0);


}

int main(int argc,char* argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }
    std::unique_ptr<HttpServer> server(new HttpServer(atoi(argv[1]),HanderHttpRequest));
    server->Start();
    return 0;
}