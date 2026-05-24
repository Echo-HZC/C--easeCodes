#include<bits/stdc++.h>

bool is_perfect_number(int a) {    //a=6
    const int A = a;
    int ys[A],cnt = 0;
    for (int i = 1;i < a;i++) {
        if (a % i == 0) {
            ys[cnt] = i;
            cnt++;
        }
    }
    int sum = 0;
    for (int i = 0;i <= cnt;i++) {
        sum += ys[i];
    }
    return sum == a;

}


int main() {
    int n;
    std::cout << "Please enter a number: ";
    std::cin >> n;
    if (is_perfect_number(n)) {
        std::cout << "The number is perfect number.\n";
    }
    else
        std::cout << "The number isn't perfect number.\n";

    std::cin.get();

    return 0;
}