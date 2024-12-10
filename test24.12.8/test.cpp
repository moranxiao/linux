#include <sys/select.h>
#include <cstdio>
#include <unistd.h>

using namespace std;

int main()
{
    fd_set read_fd;
    FD_ZERO(&read_fd);
    FD_SET(0,&read_fd);
    while(true)
    {
        printf("> ");
        fflush(stdout);
        int ret = select(1,&read_fd,nullptr,nullptr,nullptr);
        if(ret < 0)
        {
            perror("select error");
            continue;
        }
        else if(ret == 0)
        {
            printf("超时");
            continue;
        }

        if(FD_ISSET(0,&read_fd))
        {
            char buf[1024] = {0};
            read(0, buf, sizeof(buf) - 1);
            printf("input: %s", buf);
        } else {
            printf("error! invaild fd\n");
            continue;
        }
        FD_ZERO(&read_fd);
        FD_SET(0, &read_fd);
    }
    return 0;
        
}
