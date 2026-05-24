/*3.1.1 题目描述
对于一个正整数 ，我们将其转换为不含前导零的二进制表示,如果这个二进制序列从左向右读与从右向左读完全相
同，则称该数为二进制回文数。例如,9的二进制表示为 (1001)↓2，是二进制回文数； 12的二进制表示为(1100)↓2 ，不是二进制回文数。
你的任务是：给定一个正整数n ，计算在 1到 n的范围内二进制回文数的数量。
3.1.2 输入格式
输入一行，包含一个正整数n 。
3.1.3 输出格式
输出一行，包含一个数，表示在1 到 n的范围内二进制回文数的数量。
3.1.4 样例
3.1.4.1 输入样例
15
3.1.4.2 输出样例
6
3.1.5 样例解释
3.1.5 样例解释
样例 1 中，1到 范围内1 、3 、5 、7 、9 、 15是二进制回文数
3.1.6 数据范围
1<=n<=10^5*/

#include<bits/stdc++.h>

std:: string nums[100001];

int main()
{
    int n;
    std::cin >> n;
    for(int i=1;i<=n;i++) {
        int x = i;
        std::string binary;
        while (x > 0) {
            binary += (x % 2) + '0';
            x /= 2;
        }
        std::reverse(binary.begin(), binary.end());
        nums[i] = binary;
    }
    int sum = 0;
    for(int i=1;i<=n;i++) {
        std::string str = nums[i];
        int len = str.length();
        bool is = true;
        for(int j=0;j<len/2;j++) {
            if (str[j] != str[len-1-j] ) {
                is = false;
            }
        }
        if (is) {
            sum++;
        }
    }
    std::cout << sum << std::endl;
    return 0;
}