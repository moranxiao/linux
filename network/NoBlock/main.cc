#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <iostream>

bool SetNonBlock(int fd)
{
    int fl = fcntl(fd,F_GETFL);
    if(fl < 0) return false;
    fcntl(fd,F_SETFL, fl | O_NONBLOCK);
    return true;
}


int main()
{
    SetNonBlock(0);
    while(1)
    {
        char buffer[1024];
        int n = read(0,buffer,sizeof buffer);
        if(n == 0)
        {
            //stdin已关闭
            break;
        }
        else if(n > 0)
        {
            std::cout << buffer << std::endl;
        }
        else{
            if(errno == EWOULDBLOCK || errno == EAGAIN)
            {
                std::cout << "未输入数据" << std::endl;
                sleep(1);
                continue;
            }
            else if(errno == EINTR)
            {
                continue;
            }
            else{
                std::cout << "发生错误" << std::endl;
            }
        }
    }   
    return 0;
}