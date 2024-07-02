#include <stdio.h>

int main()
{
    int score;
    scanf("%d", &score);
    if(score >= 90)
        printf("优\n");
    else if(score >= 80)
        printf("良\n");
    else if(score >= 60)
        printf("及格\n");
    else
        printf("不及格\n");
    return 0;
}
