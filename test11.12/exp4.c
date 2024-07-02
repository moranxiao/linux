#include <stdio.h>
int main()
{
    printf("%d\n", !3);
    printf("%d\n", !0.0);
    printf("%d\n", !- 2.8);
    printf("%d\n", !'a');
    printf("%d\n", 3 && -5.0);
    printf("%d\n", -3 || 0.0);
    printf("%d\n", 0 && 0.0);
    printf("%d\n", 0.0 || 0);
    return 0;
}
