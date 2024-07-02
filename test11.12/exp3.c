#include <stdio.h>
int main()
{
    int a, b, c;
    a = b = 2; c = 3;
    printf("%d\n", a && b);
    printf("%d\n", a >= b || b > c);
    printf("%d\n", !(a >= b));
    printf("%d\n", !(b > c));
    return 0;
}
