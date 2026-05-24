#include<bits/stdc++.h>

#define PI 3.14
// 正方形 菱形 三角形（ah/2 && 海伦公式） 平行四边形 梯形 圆 扇形(已知圆心角（角度制）&& 已知圆心角（弧度制）&& 已知弧长 l) 椭圆
//正方形
double square (double a) {
    return a * a;
}
//菱形
double rhombic (double d1, double d2) {
    return d1 * d2 / 2;
}
//三角形（ah/2）
double triangle1 (double a,double h) {
    return a * h /2;
}
//三角形（海伦公式）
double triangle2 (double a,double b,double c) {
    double p = (a + b + c) / 2;
    return sqrt(p*(p-a)*(p-b)*(p-c));
}
//平行四边形
double parallelogram (double a, double h) {
    return a * h ;
}
//梯形
double trapezoid (double a, double b,double h) {
    return (a+b)*h/2;
}
//圆
double circle (double r) {
    return r * r * PI;
}
//扇形(已知圆心角（角度制）)
double flabellate1 (double r,double n) {
    return (n/360)*circle(r);
}
//扇形(已知圆心角（弧度制）)
double flabellate2 (double r,double θ) {
    return r*r/2*θ;
}
//扇形(已知弧长)
double flabellate3 (double r,double l) {
    return l*r/2;
}
//椭圆
double elliptic (double a,double b) {
    return a*b*PI;
}

int main()
{
    int choose;
    std::cout << "This is an area calculator." << std::endl;
    std::cout << "Please enter:" << std::endl;
    std::cout <<
"1.Square 2.Rhombus 3.Triangle (ah/2) 4.Triangle (Helen's formula) 5.Parallelogram 6.Trapezoid 7.Circle 8.Fan (known center angle (angle)) 9.Fan (known center angle (radian system)) 10.Fan (known arc length l) 11.Ellipse"    << std::endl;
    std::cin >> choose;
    switch (choose) {
        case 1: {
            double a;
            std::cout << "Please enter the side length: ";
            std::cin >> a;
            std::cout << "Area=" << std::fixed << std::setprecision(3) << square(a) << std::endl;
            break;
        }
        case 2: {
            double d1, d2;
            std::cout << "Please enter the two diagonals: ";
            std::cin >> d1 >> d2;
            std::cout << "Area=" << std::fixed << std::setprecision(3) << rhombic(d1, d2) << std::endl;
            break;
        }
        case 3: {
            double a, h;
            std::cout << "Please enter the base and height: ";
            std::cin >> a >> h;
            std::cout << "Area=" << std::fixed << std::setprecision(3) << triangle1(a, h) << std::endl;
            break;
        }
        case 4: {
            double a, b, c;
            std::cout << "Please enter the three sides: ";
            std::cin >> a >> b >> c;
            if (a + b > c && a + c > b && b + c > a) {
                std::cout << "Area=" << std::fixed << std::setprecision(3) << triangle2(a, b, c) << std::endl;
            } else {
                std::cout << "Error: These sides cannot form a triangle!" << std::endl;
            }
            break;
        }
        case 5: {
            double a, h;
            std::cout << "Please enter the base and height: ";
            std::cin >> a >> h;
            std::cout << "Area=" << std::fixed << std::setprecision(3) << parallelogram(a, h) << std::endl;
            break;
        }
        case 6: {
            double a, b, h;
            std::cout << "Please enter the upper base, lower base and height: ";
            std::cin >> a >> b >> h;
            std::cout << "Area=" << std::fixed << std::setprecision(3) << trapezoid(a, b, h) << std::endl;
            break;
        }
        case 7: {
            double r;
            std::cout << "Please enter the radius: ";
            std::cin >> r;
            std::cout << "Area=" << std::fixed << std::setprecision(3) << circle(r) << std::endl;
            break;
        }
        case 8: {
            double r, n;
            std::cout << "Please enter the radius and central angle (degrees): ";
            std::cin >> r >> n;
            std::cout << "Area=" << std::fixed << std::setprecision(3) << flabellate1(r, n) << std::endl;
            break;
        }
        case 9: {
            double r, theta;
            std::cout << "Please enter the radius and central angle (radians): ";
            std::cin >> r >> theta;
            std::cout << "Area=" << std::fixed << std::setprecision(3) << flabellate2(r, theta) << std::endl;
            break;
        }
        case 10: {
            double r, l;
            std::cout << "Please enter the radius and arc length: ";
            std::cin >> r >> l;
            std::cout << "Area=" << std::fixed << std::setprecision(3) << flabellate3(r, l) << std::endl;
            break;
        }
        case 11: {
            double a, b;
            std::cout << "Please enter the semi-major axis and semi-minor axis: ";
            std::cin >> a >> b;
            std::cout << "Area=" << std::fixed << std::setprecision(3) << elliptic(a, b) << std::endl;
            break;
        }
        default: {
            std::cout << "Invalid choice!" << std::endl;
            break;
        }
    }
    std::cin.get();
    return 0;
}