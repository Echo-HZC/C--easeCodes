/*给定两条直线，求它们的交点位置
输入有四行：
第一行输入直线l上的一点的x,y坐标，用空格隔开
第二行输入直线l上的另一点的x,y坐标，用空格隔开
第三行输入直线l'上的一点的x,y坐标，用空格隔开
第四行输入直线l'上的另一点的x,y坐标，用空格隔开

输出只有一行，两条直线的交点的x,y坐标
输入输出有适当的提示语*/
#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    double x1, y1, x2, y2;  // 直线l上的两点
    double x3, y3, x4, y4;  // 直线l'上的两点

    // 输入提示
    cout << "=== 直线交点计算器 ===" << endl << endl;

    cout << "请输入直线l上的第一个点坐标 (x y): ";
    cin >> x1 >> y1;

    cout << "请输入直线l上的第二个点坐标 (x y): ";
    cin >> x2 >> y2;

    cout << "请输入直线l'上的第一个点坐标 (x y): ";
    cin >> x3 >> y3;

    cout << "请输入直线l'上的第二个点坐标 (x y): ";
    cin >> x4 >> y4;

    // 计算直线l的参数: A1*x + B1*y + C1 = 0
    // 通过两点式转换为一般式
    double A1 = y2 - y1;
    double B1 = x1 - x2;
    double C1 = x2*y1 - x1*y2;

    // 计算直线l'的参数: A2*x + B2*y + C2 = 0
    double A2 = y4 - y3;
    double B2 = x3 - x4;
    double C2 = x4*y3 - x3*y4;

    // 计算行列式判断两直线是否平行
    double det = A1 * B2 - A2 * B1;

    cout << endl << "=== 计算结果 ===" << endl;

    if (det == 0) {
        // 检查是否重合
        if (A1 * C2 == A2 * C1 && B1 * C2 == B2 * C1) {
            cout << "两条直线重合，有无数交点" << endl;
        } else {
            cout << "两条直线平行，无交点" << endl;
        }
    } else {
        // 使用克莱默法则求解交点
        double x = (B1 * C2 - B2 * C1) / det;
        double y = (A2 * C1 - A1 * C2) / det;

        cout << fixed << setprecision(6);
        cout << "两条直线的交点坐标为: (" << x << ", " << y << ")" << endl;
    }
    std::cin.get();
    return 0;
}
