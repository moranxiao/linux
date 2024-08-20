#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>

bool SetNoBlock(int fd)
{
    int fl = fcntl(fd,F_GETFL);
    if(fl < 0)
    {
        perror("fcntl");
        return false;
    }
    fcntl(fd,F_SETFL,fl | O_NONBLOCK);
    return true;
}

int main()
{
    SetNoBlock(0);
    char buffer[1024];
    while(true)
    {   
        errno = 0;
        sleep(1);
        ssize_t n = read(0,buffer,sizeof(buffer)-1);
        if(n < 0)
        {
            if((errno & EWOULDBLOCK) || errno & EAGAIN)
            {
                std::cout << "read empty" << std::endl;
            }
            else if(errno & EINTR)
            {
                std::cout << "signal interrupt" << std::endl;
            }
            else
            {
                perror("read error");
            }
        }
        else if(n == 0)
        {
            perror("file 0 closed");
        }
        else
        {
            buffer[n-1] = 0;
            std::cout << buffer << std::endl;
        }
    }
    return 0;
}