#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
void test1()
{
    int sum = 0;
    int i = 1;
    for(i = 1; i <= 100; i++)
    {
        sum+=i;
    }
    printf("%d\n",sum);
    sum = 0;
    i = 1;
    do{
        sum+=i;
        i++;
    }while(i<=100);
    printf("%d\n",sum);
    sum = 0;
    i = 1;
    while(i<=100)
    {
        sum+=i;
        i++;
    }
    printf("%d\n",sum);
}
void test2()
{
    int sum = 0;
    int i = 0;
    for(i = 0; i <= 100; i+=2)
    {
        sum+=i;
    }
    printf("%d\n",sum);
    sum = 0;
    i = 0;
    do{
        sum+=i;
        i+=2;
    }while(i<=100);
    printf("%d\n",sum);
    sum = 0;
    i = 0;
    while(i<=100)
    {
        sum+=i;
        i+=2;
    }
    printf("%d\n",sum);
}
void test3()
{
    int sum = 0;
    for(int i = 13; i <=163; i+=10)
    {
        sum+=i;
    }
    printf("%d\n",sum);
}
void test4()
{
    int n = 0;
    scanf("%d", &n);
    int sum = 0;
    int tmp = 1;
    for(int i = 1; i <= n; i++)
    {
        tmp*=i;
        sum += tmp;
    }
    printf("%d\n",sum);

}
void test5()
{
    for(int i = 1; i <= 9; i++)
    {
        for(int j = i; j <= 9; j++)
        {
            printf("%d*%d=%-2d ", i, j, i*j);
        }
        printf("\n");
    }
}
// void test6()
// {
//     int sum = 10;
//     while((sum-1)%5 || (sum-5)%6 || (sum-4)%7 || (sum - 10)%11)
//     {
//         sum++;
//     }
//     printf("%d\n",sum);
// }
void test6()
{
    int sum = 0;
    int line = 0;
    do{
        line++;
        sum = line*5+1;
    }while((sum-5)%6 || (sum -4)%7 ||(sum-10)%11);
    printf("%d\n",sum);
}
void test7()
{
    srand((unsigned int)time(NULL));
    int num = rand() % 100+1;
    int n = 0;
    do{
        printf("please input your num>:");
        scanf("%d", &n);
        if(n > num)
        {
            printf("your num is large\n");
        }
        if(n < num)
        {
            printf("your num is small\n");
        }
    }while(num != n);
    printf("your num is right\n");
}
void test8()
{
    for(int i = 1; i <= 7; i++)
    {
        for(int j = 1; j <= 7; j++)
        {
            if(j > abs(4-i) && j <= 7-abs(4-i))
            {
                printf("* ");
            }
            else
            {
                printf("  ");
            }
        }
        printf("\n");
    }
}
void test9()
{
    int sum = 0;
    for(int i = 1; i <= 5; i++)
    {
        int tmp = 0;   
        for(int j = i; j > 1;j--)
        {
            tmp += j;
            tmp *= 10;
        }
        tmp += 1;
        printf("%d+",tmp);
        sum += tmp;
    }
    putchar(8);
    printf("=%d\n",sum);
}

void test10()
{    
    int tmpl,tmp2,tmp3;
    for (int i = 3; i > 0; i--) 
    {
        tmpl = i;
        tmp2= (i + 1) * 10 + (i + 2);
        tmp3 = (i + 3)* 10 * 10 + (i +4) * 10 + (i + 5);
        printf("%d %d %d\n", tmpl,tmp2,tmp3);
    }
}
void test11()
{
    for(int i = 100; i < 1000; i++)
    {
        int num1 = i%10;
        int num2 = i/10%10;
        int num3 = i/100;
        if(num1*num1*num1 + num2*num2*num2+num3*num3*num3 == i)
        {
            printf("%d ", i);
        }
    }
    printf("\n");
}

void test12()
{
    for(int i = 1000; i < 10000; i++)
    {
        int a = i%10;
        int b = i/10%10;
        int c = i/100%10;
        int d = i/1000;
        if(a*a*a*a + b*b*b*b+c*c*c*c+d*d*d*d == i)
            printf("%d ", i);
    }
    printf("\n");
}
int gcd(int x,int y)
{
    while(y != 0)
    {
        int tmp = x%y;
        x = y;
        y = tmp;
    }
    return x;
}
void test()
{
    int a,b;
    scanf("%d %d",&a,&b);
    int max_factor = gcd(a,b);
    int min_mul = a*b / max_factor;
    printf("max_factor=%d,min_mul=%d\n", max_factor, min_mul);
}
int main()
{
    test12();
    return 0;
}