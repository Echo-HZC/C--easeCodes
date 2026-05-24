#include<bits/stdc++.h>
using namespace std;

long long sum=1;
long long factorial(int a) {
    if (a == 0) return 1;
    if (a == 1) return sum;
    sum *= a;
    a--;
    return factorial(a);
}

int main()
{
    int n;
    cin>>n;
    cout<<factorial(n)<<endl;
    cin.get();
    return 0;
}
