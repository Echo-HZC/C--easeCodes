#include<bits/stdc++.h>

int main()
{

    long long stop;
    int cnt = 0, individual = 0;
    std::cout << "Please enter a number:" << std::endl;
    std::cin >> stop;
    std::cout <<
        "There are these Pythagorean numbers between 1 and this number:"
    << std::endl;
    for (long long i = 1;i <= stop;i++) {
        for (long long j = i +1 ;j <= stop;j++) {
            for (long long k = j + 1;k <= stop;k++) {
                if (i*i + j*j == k*k) {
                    cnt++;
                    individual++;
                    std::cout << "(" << i << "," << j << "," << k << ")" << "\t";
                    if (individual == 3) {
                        individual = 0;
                        std::cout << std::endl;
                    }
                }
            }
        }
    }

    std::cout << std::endl;
    std::cout << "There are " << cnt << " individual(s)" << "in total:" << std::endl;
    return 0;
}