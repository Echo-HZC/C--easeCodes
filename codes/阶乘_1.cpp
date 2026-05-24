#include<iostream>
using namespace std;

long long factorial(int a){
    if(a==0) return 1;
    long long sum=1;
    while(a){
        sum*=a;
        a--;
    }
    return sum;
}

int main()
{
    int n;
    cin>>n;
    cout<<factorial(n)<<endl;
    cin.get();
    return 0;
}