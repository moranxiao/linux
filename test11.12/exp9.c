#include <stdio.h>
#include <float.h>
#include <math.h>

int main()
{
    float a,b;
    char op;
    scanf("%f %c%f",&a,&op,&b);
    switch(op)
    {
        case '+':
        printf("%f+%f=%f\n",a,b,(double)a+b);
        break;
        case '-':
        printf("%f-f=%f\n",a,b,(double)a-b);
        break;
        case '*':
        printf("%f*%f=%f\n",a,b,(double)a*b);
        break;
        case '/':
        if( fabs(b) < FLT_EPSILON)
            printf("DIV BY ZERO\n");
        else
            printf("%f/%f=%f\n",a,b,(double)a/b);
        break;
        default:
            printf("invalid operator\n");
    }
    return 0;
}