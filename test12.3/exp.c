#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>

int is_leapyear(int year)
{
    if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
    {
        return 1;
    }
    return 0;
}
void test1()
{
    int sum = 0;
    for (int i = 2020; i <= 2030; i++)
    {
        if (is_leapyear(i))
            sum++;
    }
    printf("%d\n", sum);
}
int is_prime(int num)
{
    for (int i = 2; i * i <= num; i++)
    {
        if (num % i == 0)
        {
            return 0;
        }
    }
    return 1;
}
void test2()
{
    int num;
    printf("please input 100~1000 num>:");
    scanf("%d", &num);
    if (num < 100 || num > 1000)
    {
        printf("error");
        exit(0);
    }

    int count = 1;
    for (int i = 2; i <= num; i++)
    {
        if (is_prime(i))
        {
            printf("%-3d ", i);
            count++;
        }
        if (count > 5)
        {
            printf("\n");
            count %= 5;
        }
    }
}
void test3()
{
    int sum = 0;
    for (int i = 2; i <= 100; i++)
    {
        if (is_prime(i))
        {
            sum += i;
        }
    }
    printf("%d\n", sum);
}
int recursion_fib(int n)
{
    if (n < 3)
    {
        return 1;
    }
    return recursion_fib(n - 1) + recursion_fib(n - 2);
}
int fib(int n)
{
    if (n < 3)
        return 1;
    int a = 1;
    int b = 1;
    int result;
    for (int i = 3; i <= n; i++)
    {
        result = a + b;
        a = b;
        b = result;
    }
    return result;
}
void test4()
{
    int n;
    scanf("%d", &n);
    // if(n<20 || n > 30)
    // {
    //     printf("error\n");
    //     return;
    // }
    int sum = 0;
    for (int i = 1; i <= n; i++)
    {
        sum += fib(i);
    }
    printf("sum=%d\n", sum);
}
long long fact(int n)
{
    static int fac = 1;
    fac*= n;
    return fac;
}
void test5()
{
    int n;
    for(int i = 1; i < 11; i++)
    {
        printf("n=%d:%d\n", i, fact(i));
    }
}
int randomGenerator()
{
    srand((unsigned int)time(NULL));
    return rand()%91 + 10;
}
void guessNumber(int n)
{
    int num;
    do{
        printf("please input 10~100 num:>");
        scanf("%d", &num);
        if(num > n)
        {
            printf("large\n");
        }
        else if(num < n)
        {
            printf("small\n");
        }
        else
        {
            printf("right\n");
            break;
        }
    }while(1);
}
void test6()
{
    int n = randomGenerator();
    guessNumber(n);
}
int chick_num(int heads,int legs)
{
    for(int i = 0; i <= heads; i++)
    {
        if(i*2+(heads - i)*4 == legs)
            return i;
    }
    return -1;
}
void test7()
{
    int heads, legs;
repeat:
    scanf("%d %d", &heads, &legs);
    if(legs % 2 != 0 || legs > 4*heads || legs < 2*heads)
    {
        printf("intput error\n");
        goto repeat;
    }   
    int chick = chick_num(heads,legs);
    printf("chick=%d,rabbit=%d\n", chick,heads-chick);
}
#include <assert.h>
void test8()
{
    int a = 1, b = 1, sum = 0;
    for (int i = 0; i < 5; i++)
    {
        sum += a+ b;
    }
    //assert(a == 6&& b== 1&& sum ==20 );
    printf("a=%d, b=%d, sum=%d" , a, b, sum);
}
void func() {
    printf("in func()!\n");
    exit(0);
}
void test9() {
    printf("before func()!\n");
    func();
    printf("after func()!");
}

int main()
{
    test9();
    return 0;
}