#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>

int main()
{
    int fd;
    int a[10], i;
    for (i = 0; i < 10; i++)
        a[i] = i + 1;
    //创建文件名为aaa的文件，user权限rwx,group权限rx,other权限rx
    fd = creat("aaa", 0755);
    printf("lockf 40 bytes in parent…\n");
    fflush(stdout);
    //对文件0~40字节的位置,加锁   
    // F_LOCK=1
    lockf(fd, 1, 40);
    printf("…locked.\n");
    fflush(stdout);
    if (fork() == 0)
    {
        //子进程
        printf("Enter child, write 20 bytes in child…\n");
        fflush(stdout);
        //向文件中写入20字节
        write(fd, a, 20);
        printf("…writeen.\n");
        fflush(stdout);
        printf("lockf 80 bytes in child…\n");
        fflush(stdout);
        //对文件20 ~ 100字节位置加锁
        lockf(fd, 1, 80);
        printf("…locked in child.\n");
        fflush(stdout);
        sleep(2);
        //对文件20 ~ 100字节位置解锁
        lockf(fd, 0, 80);
        printf("…child unlocked.\n");
        fflush(stdout);
        exit(0);
    }
    printf("Parent sleep now…\n");
    //直接让出时间片
    sleep(0);
    printf("…parent wakeup.\n");
    printf("Parent unlock now…\n");
    fflush(stdout);
    //对文件0~40位置解锁
    lockf(fd, 0, 40);
    printf("…Parent unlocked.\n");
    fflush(stdout);
    //等待子进程
    wait(0);
    printf("Program end.\n");
    return 0;
}
