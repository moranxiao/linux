#include <iostream>

using namespace std;

const int N = 1e6+10;

bool st[N];
int prime[N],cnt;


void filter(int n)
{
    for(int i = 2; i <= n; i++)
    {
        if(!st[i]) prime[cnt++] = i;
        for(int j = 0; j < cnt; j++)
        {
            if(prime[j]*i > n) break;
            st[i*prime[j]] = true;
            if(i % prime[j] == 0) break;
        }
    }
}



int main()
{
    int n;
    cin >> n;
    filter(n);
    for(int i = 0; i < cnt; i++)
    {
        cout << prime[i] << ' ';
    }   
    return 0;
}
