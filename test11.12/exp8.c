#include <stdio.h>

int is_triangle(int a,int b,int c)
{
    if(a + b > c \
    && a - b < c \
    && a - b < c)
        return 1;
    return 0;
}
int is_right(int a,int b,int c)
{
    if(a *a + b*b ==c*c \
        || a*a+c*c==b*b \
        || b*b+c*c==a*a
        )
        return 1;
    return 0;
}
int main()
{
    int a,b,c;
    scanf("%d %d %d", &a, &b, &c);
    if(is_triangle(a,b,c) == 0)
    {
        printf("不是三角形\n");
        return 0;
    }
    if((a == b) + (a == c) + (b == c) == 0)
    {
        if(is_right(a,b,c))
            printf("直角三角形\n");
        else
            printf("普通三角形\n");
    }
    else if((a == b) + (a == c) + (b == c) == 1)
    {
        if(is_right(a,b,c))
            printf("等腰直角三角形\n");
        else
            printf("等腰三角形\n");
    }
    else
        printf("等边三角形\n");
    return 0;
}