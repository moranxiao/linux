#include <stdio.h>
#include <math.h>


void test1()
{
    int arr1[10];
    for(int i = 0; i < 10; i++)
    {
        if(i >= 7)
            arr1[i] = 0;
        else
            arr1[i] = i+1;
    }
    int arr2[10] = {1,2,3,4,5,6,7,0,0,0};
    float arr3[10];
    for(int i = 0; i < 10; i++)
    {
        if(i >= 8)
            arr3[i] = 0.0;
        else
        
            arr3[i] = 1.2 + i*1.1;
    }
    float arr4[10] = {1.2,2.3,3.4,4.5,5.6,6.7,7.8,8.9,0.0,0.0};
    int arr5[] = {1,2,3,4,5};
    
    int sz1 = sizeof(arr1) / sizeof(arr1[0]);
    printf("arr1=");
    for(int i = 0; i < sz1; i++)
    {
        printf("%d ", arr1[i]);
    }
    printf("sz1=%d",sz1);
    printf("\n");
    
    int sz2 = sizeof(arr2) / sizeof(arr2[0]);
    printf("arr2=");
    for(int i = 0; i < sz2; i++)
    {
        printf("%d ", arr2[i]);
    }
    printf("sz2=%d",sz2);
    printf("\n");

    int sz3 = sizeof(arr3) / sizeof(arr3[0]);
    printf("arr3=");
    for(int i = 0; i < sz3; i++)
    {
        printf("%.1f ", arr3[i]);
    }
    printf("sz3=%d",sz3);
    printf("\n");

    int sz4 = sizeof(arr4) / sizeof(arr4[0]);
    printf("arr4=");
    for(int i = 0; i < sz4; i++)
    {
        printf("%.1f ", arr4[i]);
    }
    printf("sz4=%d",sz4);
    printf("\n");

    int sz5 = sizeof(arr5) / sizeof(arr5[0]);
    printf("arr5=");
    for(int i = 0; i < sz5; i++)
    {
        printf("%d ", arr5[i]);
    }
    printf("sz5=%d",sz5);
    printf("\n");
}
void test2()
{
    int iarr[] = {22,55,11,88,33};
    int sz = sizeof(iarr) / sizeof(iarr[0]);
    for(int i = 0; i < sz; i++)
    {
        iarr[i] += 2;
        printf("%d ", iarr[i]);
    }
    printf("\n");
} 
int is_leapyear(int year)
{
    if(year % 400 == 0 || (year % 100 != 0 && year % 4 == 0))
        return 1;
    return 0;
}
void test3()
{
    int days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int year,day;
    do
    {
        printf("input year&day:>");
        scanf("%d %d", &year, &day);
        if(is_leapyear(year) && day <= 366 && day > 0)
        {
            days[1] = 29; 
            break;
        }
        if(!is_leapyear(year) && day <= 365 && day > 0)
            break;
    }while(1);
    for(int i = 0; i < 12; i++)
    {
        if(day < days[i])
        {
            printf("month:%d date:%d\n", i+1, day);
            break;
        }
        else
            day-=days[i];
    }    
}
#define N 4
int isNumber(int n)
{
    int tmp = n;
    int val[N];
    int sum = 0;
    for(int i = 0; i < N; i++)
    {
        val[i] = tmp%10;
        tmp /= 10;
    }
    for(int i = 0; i < N; i++)
    {
        int mult = 1;
        for(int j = 0; j < N; j++)
        {
            mult *= val[i];
        }
        sum += mult;
    }
    if(sum == n)
        return 1;
    return 0;
}
void test4()
{
    for(int i = 1000; i < 10000; i++)
    {
        if(isNumber(i))
            printf("%d ", i);
    }
    printf("\n");
}

int is_perfect(int n)
{
    int sum = 1;
    for(int i = 2; i*i <= n; i++)
    {
        if(n % i == 0)
            sum += i + n/i;
    }
    if(n == sum)
        return 1;
    return 0;
}
void test5()
{  
    for(int i = 1; i <= 10000; i++)
    {
        if(is_perfect(i))
            printf("%d ", i);
    }
    printf("\n");
}

int is_prime(int n)
{
    for(int i = 2; i <= sqrt(n); i++)
    {
        if(n % i == 0)
            return 0;
    }
    return 1;
}
int is_GDBH(int n)
{
    for(int i = 3; i <= n/2; i+=2)
    {
        if(is_prime(i) && is_prime(n-i))
            return 1;
    }
    return 0;
}
void test6()
{
    int i;
    for(i = 8; i <= 1000; i+=2)
    {
        if(!is_GDBH(i))
        {
            printf("no\n");
            break;
        }
    }
    if(i > 1000)
        printf("yes\n");
}
void swap(int* a,int * b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
void sort1(int arr[],int n)
{
    for(int i = 1; i < n; i++)
    {
        int tmp = i-1; 
        for(int j = i; j < n; j++)
        {
            if(arr[j] > arr[tmp])
                tmp = j;
        }
        swap(arr+tmp,arr+i-1);
    }
}
void sort2(int arr[],int n)
{
    for(int i = 0; i < n-1; i++)
    {
        for(int j = 1; j < n-i; j++)
        {
            if(arr[j-1] < arr[j])
            {
                swap(arr+j-1,arr+j);
            }
        }
    }
}
void sort3(int* arr,int n)
{
    sort1(arr,n);
}
void test7()
{
    int iarr[] = {22,55,11,88,33};
    sort3(iarr, 5);
    for(int i = 0; i < 5; i++)
    {
        printf("%d ", iarr[i]);
    }
    printf("\n");
}
void test8()
{
    int a = 1, b = 2;
    printf("old a=%d,b=%d\n", a, b);
    swap(&a,&b);
    printf("new a=%d,b=%d\n", a, b);

}

int search(int* arr,int sz,int num)
{
    for(int i = 0; i < sz; i++)
    {
        if(arr[i] == num)
            return i;
    }
    return -1;
}
void test9()
{
    int iarr[] = {22,55,11,88,33};
    int i = 0;
    do{
        int num = 0;
        printf("please input num:>");
        scanf("%d", &num);
        int index = search(iarr,5,num);
        if(index >= 0)
        {
            printf("index=%d\n", index);
            i = 0;
        }
        else
        {
            i++;
            printf("no exit num\nsurplus chance:%d\n", 3-i);
        }
    }while(i < 3);
}
int half_search(int* arr,int sz,int num)
{
    int begin=0, end = sz-1;
    int mid = (begin + end)/2;
    while(begin <= end)
    {
        if(arr[mid] > num)
        {
            end = mid-1;
        }
        else if(arr[mid] < num)
        {
            begin = mid+1;
        }
        else
        {
            return mid;
        }
        mid = (begin + end)/2;
    }
    return -1;
}
void down_sort(int arr[], int n)
{
    for(int i = 1; i < n; i++)
    {
        int tmp = i-1; 
        for(int j = i; j < n; j++)
        {
            if(arr[j] < arr[tmp])
                tmp = j;
        }
        swap(arr+tmp,arr+i-1);
    }
}
void test10()
{
    int iarr[] = {22,55,11,88,33};
    down_sort(iarr,5);
    int i = 0;
    do{
        int num = 0;
        printf("please input num:>");
        scanf("%d", &num);
        int index = half_search(iarr,5,num);
        if(index >= 0)
        {
            printf("index=%d\n", index);
            i = 0;
        }
        else
        {
            i++;
            printf("no exit num\nsurplus chance:%d\n", 3-i);
        }
    }while(i < 3);
}

const int days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
int leap_year(int year)
{
    if((year % 400 == 0) || (year % 4  == 0 && year % 100 != 0))
    {
        return 1;
    }
    return 0;
}
void inputDate()
{
    int year,month,day;
    int leap = 0;
    do{
        printf("please input year month day >:");
        scanf("%d %d %d", &year,&month,&day);
        if(year > 0 && month >= 1 && month <= 12 && day > 0)
        {
            if(leap_year(year))
                leap = 1;
            else
                leap = 0;
            
            if((month == 2 && day <= days[month-1] + leap)
            || (month != 2 && day <= days[month-1]))
                break;
            
        }
        printf("input error\n");
    }while(1);
    int count = 0;
    for(int i = 1; i < month; i++)
    {
        count += days[i-1];
    }
    count += day;
    printf("%d\n", count);
}
void test11()
{
    inputDate();
}
void reverse(int* arr,int sz)
{
    int begin = 0;
    int end = sz-1;
    while(begin < end)
    {
        swap(arr+begin,arr+end);
        begin++;
        end--;
    }
}
void test12()
{
    int iarr[] = {22,55,11,88,33};
    int sz =sizeof(iarr)/sizeof(iarr[0]);
    reverse(iarr,sz);
    for(int i = 0; i < sz; i++)
    {
        printf("%d ", iarr[i]);
    }
    printf("\n");
}
int main()
{
    test12();
    return 0;
}