#include <stdio.h>


void test1()
{
    int arr[][4] = {
        11,22,33,44,
        55,66,77,88
    };
    int rows = sizeof(arr) / sizeof(arr[0]);
    int cols = sizeof(arr[0]) / sizeof(arr[0][0]);
    printf("row=%d,col=%d\n", rows, cols);
    printf("-------------------\n");
    int(*pline)[4] = arr;
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            printf("%d ",*(*(pline+i)+j));

        }
        printf("\n");
    }
    printf("-------------------\n");
    int* pa[2] = {arr[0], arr[1]};
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            printf("%d ", pa[i][j]);
        }
        printf("\n");
    }
}

int add(int x,int y)
{
    return x+y;
}
int sub(int x,int y)
{
    return x-y;
}
int mul(int x,int y)
{
    return x*y;
}
int div(int x,int y)
{
    return x/y;
}
int four_operations(int *fuc(int,int), int x,int y)
{
    return fuc(x,y);
}
void test2()
{
    printf("%d\n" , four_operations(div,4,2));
}

void test3()
{
    int al = 3,a2 = 5,b1=77,b2=99;
    const int* pa = &a1;
    pa = a2;
    *pa = 15;
    int* const pb = &bl ;
    *pb = 88;
    pb = &b2;

}
int main()
{
    test2();
    return 0;
}