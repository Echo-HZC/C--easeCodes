#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

int main() {
    const int MAXN = 30;
    int n;
    int triangle[MAXN][MAXN] = {0};

    cout << "请输入行数 (1-" << MAXN << "): ";
    cin >> n;

    // 生成杨辉三角
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            if (j == 0 || j == i) {
                triangle[i][j] = 1;
            } else {
                triangle[i][j] = triangle[i-1][j-1] + triangle[i-1][j];
            }
        }
    }

    // 计算最大宽度
    int maxNum = triangle[n-1][n/2];
    int maxWidth = to_string(maxNum).length();
    int cellWidth = maxWidth + 2;  // 每个数字占用的宽度

    for (int i = 0; i < n; i++) {
        // 计算前导空格，让当前行居中
        int leading = (n - 1 - i) * cellWidth / 2;
        for (int k = 0; k < leading; k++) cout << ' ';

        for (int j = 0; j <= i; j++) {
            // 使用 setw 右对齐输出
            cout << setw(maxWidth) << triangle[i][j];
            // 数字间间隔：除了最后一个数字
            if (j < i) {
                // 间隔宽度 = 下一层需要的空间 - 当前数字宽度
                // 简化：固定间隔，根据层数动态调整
                int gap = cellWidth - maxWidth + 1;
                for (int s = 0; s < gap; s++) cout << ' ';
            }
        }
        cout << endl;
    }

    return 0;
}