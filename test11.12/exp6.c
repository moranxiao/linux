#include <stdio.h>

const int days[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
int leap_year(int year)
{
    if((year % 400 == 0) || (year % 4  == 0 && year % 100 != 0))
    {
        return 1;
    }
    return 0;
}
int main()
{
    int year,month;
    scanf("%d %d", &year, &month);
    if(month == 2)
        printf("%d\n",days[month] + leap_year(year));
    else
        printf("%d\n",days[month]);
    return 0;
}