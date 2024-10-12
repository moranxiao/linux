#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int  main()
  {  int  i;
char name[20]; //存储用户输入的文件路径
     printf("Please input a directory name:\n");
     scanf("%s",name);  
     if (i=fork()==0) //创建子进程，并且子进程执行if内的语句
	//子进程执行进程替换，替换为ls -l ····· 命令
         execl("/bin/ls","ls","-l",name,NULL); 
   }

