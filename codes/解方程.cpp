#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cctype>

using namespace std;

// 表达式节点类型
enum NodeType { NUMBER, VARIABLE, ADD, SUB, MUL, DIV, POW, NEG };

struct ExprNode {
    NodeType type;
    double value;           // 对于NUMBER
    char var;               // 对于VARIABLE
    ExprNode* left;
    ExprNode* right;

    ExprNode(NodeType t) : type(t), value(0), var(0), left(nullptr), right(nullptr) {}
    ~ExprNode() {
        delete left;
        delete right;
    }
};

// 解析器类
class Parser {
    string s;
    size_t pos;

public:
    Parser(const string& str) : s(str), pos(0) {}

    // 跳过空白
    void skipSpace() {
        while (pos < s.size() && isspace(s[pos])) pos++;
    }

    // 检查是否为字母（支持所有字母作为变量）
    bool isVarChar(char c) const {
        return isalpha(c);
    }

    // 解析数字（支持负数、小数）
    ExprNode* parseNumber() {
        skipSpace();
        size_t start = pos;
        bool hasDigits = false;
        bool hasDot = false;

        // 处理符号（仅在表达式开头或左括号后或运算符后）
        if (pos < s.size() && (s[pos] == '-' || s[pos] == '+')) {
            // 检查前一个字符，确定这是符号还是运算符
            if (pos == 0) {
                // 表达式开头，是符号
                pos++;
            } else {
                size_t prev = pos - 1;
                while (prev > 0 && isspace(s[prev])) prev--;
                char prevChar = s[prev];
                if (prevChar == '(' || prevChar == ',' || prevChar == '=' ||
                    prevChar == '+' || prevChar == '-' || prevChar == '*' || prevChar == '/') {
                    pos++; // 是符号
                } else {
                    return nullptr; // 是运算符，不是数字的一部分
                }
            }
        }

        // 解析整数部分
        while (pos < s.size() && isdigit(s[pos])) {
            pos++;
            hasDigits = true;
        }

        // 解析小数部分
        if (pos < s.size() && s[pos] == '.') {
            hasDot = true;
            pos++;
            while (pos < s.size() && isdigit(s[pos])) {
                pos++;
                hasDigits = true;
            }
        }

        if (!hasDigits) {
            // 回退，比如只有"-"但没有数字
            pos = start;
            return nullptr;
        }

        string numStr = s.substr(start, pos - start);
        ExprNode* node = new ExprNode(NUMBER);
        node->value = stod(numStr);
        return node;
    }

    // 解析变量或带系数的变量
    ExprNode* parseVariableOrTerm() {
        skipSpace();

        // 尝试解析数字（系数），包括负数系数
        size_t savedPos = pos;
        ExprNode* coef = parseNumber();

        skipSpace();

        // 检查是否是变量（支持所有字母）
        if (pos < s.size() && isVarChar(s[pos])) {
            char v = s[pos++];

            ExprNode* varNode = new ExprNode(VARIABLE);
            varNode->var = v;

            // 检查是否有指数
            skipSpace();
            if (pos < s.size() && s[pos] == '^') {
                pos++; // 跳过'^'
                skipSpace();
                ExprNode* exp = parseNumber();
                if (!exp) {
                    // 如果没有解析到数字指数，可能是变量指数，不支持
                    exp = new ExprNode(NUMBER);
                    exp->value = 1;
                }

                // 创建幂运算节点
                ExprNode* powNode = new ExprNode(POW);
                powNode->left = varNode;
                powNode->right = exp;

                // 如果有系数，创建乘法节点
                if (coef) {
                    ExprNode* mulNode = new ExprNode(MUL);
                    mulNode->left = coef;
                    mulNode->right = powNode;
                    return mulNode;
                }
                return powNode;
            }

            // 没有指数，创建 x^1
            ExprNode* powNode = new ExprNode(POW);
            powNode->left = varNode;
            ExprNode* one = new ExprNode(NUMBER);
            one->value = 1;
            powNode->right = one;

            if (coef) {
                ExprNode* mulNode = new ExprNode(MUL);
                mulNode->left = coef;
                mulNode->right = powNode;
                return mulNode;
            } else {
                // 隐含系数为1
                ExprNode* one = new ExprNode(NUMBER);
                one->value = 1;
                ExprNode* mulNode = new ExprNode(MUL);
                mulNode->left = one;
                mulNode->right = powNode;
                return mulNode;
            }
        }

        // 不是变量，返回之前解析的数字
        if (coef) return coef;

        // 可能是括号表达式
        if (pos < s.size() && s[pos] == '(') {
            pos++;
            ExprNode* expr = parseExpression();
            skipSpace();
            if (pos < s.size() && s[pos] == ')') pos++;
            return expr;
        }

        return nullptr;
    }

    // 解析因子（处理乘法、除法）
    ExprNode* parseFactor() {
        skipSpace();
        ExprNode* left = parseVariableOrTerm();
        if (!left) return nullptr;

        while (true) {
            skipSpace();
            if (pos >= s.size()) break;

            if (s[pos] == '*') {
                pos++;
                ExprNode* right = parseVariableOrTerm();
                if (!right) break;
                ExprNode* node = new ExprNode(MUL);
                node->left = left;
                node->right = right;
                left = node;
            } else if (s[pos] == '/') {
                pos++;
                ExprNode* right = parseVariableOrTerm();
                if (!right) break;
                ExprNode* node = new ExprNode(DIV);
                node->left = left;
                node->right = right;
                left = node;
            } else {
                break;
            }
        }
        return left;
    }

    // 解析表达式（处理加法、减法）
    ExprNode* parseExpression() {
        skipSpace();
        ExprNode* left = parseFactor();
        if (!left) return nullptr;

        while (true) {
            skipSpace();
            if (pos >= s.size()) break;

            if (s[pos] == '+') {
                pos++;
                ExprNode* right = parseFactor();
                if (!right) break;
                ExprNode* node = new ExprNode(ADD);
                node->left = left;
                node->right = right;
                left = node;
            } else if (s[pos] == '-') {
                // 减号：检查是否是二元运算符
                size_t nextPos = pos + 1;
                // 跳过空白看下一个字符
                while (nextPos < s.size() && isspace(s[nextPos])) nextPos++;

                if (nextPos < s.size() && (isdigit(s[nextPos]) || isVarChar(s[nextPos]) || s[nextPos] == '(' || s[nextPos] == '.')) {
                    pos++;
                    ExprNode* right = parseFactor();
                    if (!right) break;
                    ExprNode* node = new ExprNode(SUB);
                    node->left = left;
                    node->right = right;
                    left = node;
                } else {
                    break; // 不是有效的减法
                }
            } else {
                break;
            }
        }
        return left;
    }
};

// 计算表达式值
double evaluate(ExprNode* node, const map<char, double>& vars) {
    if (!node) return 0;

    switch (node->type) {
        case NUMBER: return node->value;
        case VARIABLE: {
            auto it = vars.find(node->var);
            if (it != vars.end()) return it->second;
            return 0; // 变量未定义
        }
        case ADD: return evaluate(node->left, vars) + evaluate(node->right, vars);
        case SUB: return evaluate(node->left, vars) - evaluate(node->right, vars);
        case MUL: return evaluate(node->left, vars) * evaluate(node->right, vars);
        case DIV: {
            double denom = evaluate(node->right, vars);
            if (abs(denom) < 1e-15) return 1e308; // 防止除零
            return evaluate(node->left, vars) / denom;
        }
        case POW: return pow(evaluate(node->left, vars), evaluate(node->right, vars));
        case NEG: return -evaluate(node->left, vars);
    }
    return 0;
}

// 收集表达式中的变量
void collectVars(ExprNode* node, vector<char>& vars) {
    if (!node) return;
    if (node->type == VARIABLE) {
        if (find(vars.begin(), vars.end(), node->var) == vars.end()) {
            vars.push_back(node->var);
        }
    }
    collectVars(node->left, vars);
    collectVars(node->right, vars);
}

// 提取多项式系数（针对特定变量）
void extractPolynomialCoeffs(ExprNode* node, char targetVar,
                             map<int, double>& coeffs, double multiplier = 1) {
    if (!node) return;

    switch (node->type) {
        case NUMBER:
            coeffs[0] += multiplier * node->value;
            break;

        case VARIABLE:
            if (node->var == targetVar) {
                coeffs[1] += multiplier;
            } else {
                coeffs[0] += multiplier; // 其他变量视为常数
            }
            break;

        case ADD:
            extractPolynomialCoeffs(node->left, targetVar, coeffs, multiplier);
            extractPolynomialCoeffs(node->right, targetVar, coeffs, multiplier);
            break;

        case SUB:
            extractPolynomialCoeffs(node->left, targetVar, coeffs, multiplier);
            extractPolynomialCoeffs(node->right, targetVar, coeffs, -multiplier);
            break;

        case MUL: {
            // 尝试识别系数*变量^n 的形式
            double leftConst = 0, rightConst = 0;
            bool leftIsConst = false, rightIsConst = false;

            if (node->left && node->left->type == NUMBER) {
                leftConst = node->left->value;
                leftIsConst = true;
            }
            if (node->right && node->right->type == NUMBER) {
                rightConst = node->right->value;
                rightIsConst = true;
            }

            if (leftIsConst && !rightIsConst) {
                extractPolynomialCoeffs(node->right, targetVar, coeffs, multiplier * leftConst);
            } else if (!leftIsConst && rightIsConst) {
                extractPolynomialCoeffs(node->left, targetVar, coeffs, multiplier * rightConst);
            } else if (leftIsConst && rightIsConst) {
                coeffs[0] += multiplier * leftConst * rightConst;
            } else {
                extractPolynomialCoeffs(node->left, targetVar, coeffs, multiplier);
                extractPolynomialCoeffs(node->right, targetVar, coeffs, multiplier);
            }
            break;
        }

        case POW: {
            if (node->left && node->left->type == VARIABLE &&
                node->left->var == targetVar &&
                node->right && node->right->type == NUMBER) {
                int exp = (int)round(node->right->value);
                coeffs[exp] += multiplier;
            } else if (node->left && node->left->type == VARIABLE &&
                       node->left->var == targetVar) {
                coeffs[1] += multiplier;
            } else {
                extractPolynomialCoeffs(node->left, targetVar, coeffs, multiplier);
                extractPolynomialCoeffs(node->right, targetVar, coeffs, multiplier);
            }
            break;
        }

        case DIV:
            if (node->right && node->right->type == NUMBER) {
                extractPolynomialCoeffs(node->left, targetVar, coeffs, multiplier / node->right->value);
            }
            break;

        case NEG:
            extractPolynomialCoeffs(node->left, targetVar, coeffs, -multiplier);
            break;
    }
}

// 求解一元一次方程 ax + b = c
bool solveLinear(double a, double b, double c, double& result) {
    if (abs(a) < 1e-10) {
        if (abs(b - c) < 1e-10) {
            result = 0;
            return true;
        }
        return false;
    }
    result = (c - b) / a;
    return true;
}

// 求解一元二次方程 ax^2 + bx + c = d
vector<double> solveQuadratic(double a, double b, double c, double d) {
    vector<double> roots;
    c = c - d;

    if (abs(a) < 1e-10) {
        if (abs(b) > 1e-10) {
            roots.push_back(-c / b);
        }
        return roots;
    }

    double discriminant = b*b - 4*a*c;
    if (discriminant < -1e-10) {
        return roots;
    }

    if (abs(discriminant) < 1e-10) {
        roots.push_back(-b / (2*a));
    } else {
        double sqrtD = sqrt(discriminant);
        roots.push_back((-b + sqrtD) / (2*a));
        roots.push_back((-b - sqrtD) / (2*a));
    }
    return roots;
}

// 求解一元三次方程 ax^3 + bx^2 + cx + d = e
vector<double> solveCubic(double a, double b, double c, double d, double e) {
    vector<double> roots;
    d = d - e;

    if (abs(a) < 1e-10) {
        return solveQuadratic(b, c, d, 0);
    }

    auto f = [&](double x) {
        return a*x*x*x + b*x*x + c*x + d;
    };

    // 在较大范围内搜索符号变化
    double step = 0.5;
    double prevX = -100;
    double prevY = f(prevX);

    for (double x = -100; x <= 100; x += step) {
        double y = f(x);
        if (prevY * y < 0 || abs(y) < 1e-6) {
            double left = prevX, right = x;
            if (abs(y) < 1e-6) {
                if (roots.empty() || abs(x - roots.back()) > 1e-4)
                    roots.push_back(x);
            } else {
                for (int iter = 0; iter < 50; iter++) {
                    double mid = (left + right) / 2;
                    double midY = f(mid);
                    if (abs(midY) < 1e-12) {
                        if (roots.empty() || abs(mid - roots.back()) > 1e-4)
                            roots.push_back(mid);
                        break;
                    }
                    if (prevY * midY < 0) {
                        right = mid;
                        y = midY;
                    } else {
                        left = mid;
                        prevY = midY;
                    }
                }
                double mid = (left + right) / 2;
                if (roots.empty() || abs(mid - roots.back()) > 1e-4)
                    roots.push_back(mid);
            }
        }
        prevX = x;
        prevY = y;
    }

    sort(roots.begin(), roots.end());
    vector<double> uniqueRoots;
    for (double r : roots) {
        if (uniqueRoots.empty() || abs(r - uniqueRoots.back()) > 1e-4) {
            uniqueRoots.push_back(r);
        }
    }

    return uniqueRoots;
}

// 求解线性方程组（高斯消元）
vector<double> solveLinearSystem(vector<vector<double>>& A, vector<double>& B) {
    int n = A.size();
    if (n == 0) return {};

    for (int i = 0; i < n; i++) {
        A[i].push_back(B[i]);
    }

    for (int i = 0; i < n; i++) {
        int maxRow = i;
        for (int k = i+1; k < n; k++) {
            if (abs(A[k][i]) > abs(A[maxRow][i])) {
                maxRow = k;
            }
        }
        swap(A[i], A[maxRow]);

        if (abs(A[i][i]) < 1e-10) continue;

        for (int k = i+1; k < n; k++) {
            double factor = A[k][i] / A[i][i];
            for (int j = i; j <= n; j++) {
                A[k][j] -= factor * A[i][j];
            }
        }
    }

    vector<double> x(n, 0);
    for (int i = n-1; i >= 0; i--) {
        if (abs(A[i][i]) < 1e-10) continue;
        x[i] = A[i][n];
        for (int j = i+1; j < n; j++) {
            x[i] -= A[i][j] * x[j];
        }
        x[i] /= A[i][i];
    }

    return x;
}

// 方程求解器类
class EquationSolver {
public:
    struct Equation {
        ExprNode* left;
        ExprNode* right;
        string raw;
    };

    vector<Equation> equations;
    vector<char> allVars;

    ~EquationSolver() {
        for (auto& eq : equations) {
            delete eq.left;
            delete eq.right;
        }
    }

    void parseInput(const string& input) {
        vector<string> eqStrs;
        string current;
        int parenCount = 0;

        for (char c : input) {
            if (c == '(') parenCount++;
            else if (c == ')') parenCount--;

            if (c == ',' && parenCount == 0) {
                if (!current.empty()) {
                    eqStrs.push_back(current);
                    current.clear();
                }
            } else {
                current += c;
            }
        }
        if (!current.empty()) {
            eqStrs.push_back(current);
        }

        for (string& eqStr : eqStrs) {
            size_t eqPos = eqStr.find('=');
            if (eqPos == string::npos) continue;

            string leftStr = eqStr.substr(0, eqPos);
            string rightStr = eqStr.substr(eqPos + 1);

            Parser leftParser(leftStr);
            Parser rightParser(rightStr);

            Equation eq;
            eq.left = leftParser.parseExpression();
            eq.right = rightParser.parseExpression();
            eq.raw = eqStr;

            if (eq.left && eq.right) {
                equations.push_back(eq);

                vector<char> vars;
                collectVars(eq.left, vars);
                collectVars(eq.right, vars);
                for (char v : vars) {
                    if (find(allVars.begin(), allVars.end(), v) == allVars.end()) {
                        allVars.push_back(v);
                    }
                }
            }
        }

        sort(allVars.begin(), allVars.end());
    }

    void solve() {
        if (equations.empty()) {
            cout << "没有有效的方程" << endl;
            return;
        }

        if (allVars.empty()) {
            bool allTrue = true;
            for (auto& eq : equations) {
                double l = evaluate(eq.left, {});
                double r = evaluate(eq.right, {});
                if (abs(l - r) > 1e-6) {
                    allTrue = false;
                    break;
                }
            }
            if (allTrue) {
                cout << "恒等式（任意解）" << endl;
            } else {
                cout << "无解" << endl;
            }
            return;
        }

        if (allVars.size() == 1) {
            solveSingleVariable();
            return;
        }

        if (allVars.size() >= 2) {
            solveMultiVariable();
            return;
        }
    }

    void solveSingleVariable() {
        char var = allVars[0];
        vector<double> allSolutions;
        bool hasSolution = false;

        for (auto& eq : equations) {
            map<int, double> coeffs;
            extractPolynomialCoeffs(eq.left, var, coeffs, 1);
            extractPolynomialCoeffs(eq.right, var, coeffs, -1);

            int maxDegree = 0;
            for (auto& p : coeffs) {
                if (abs(p.second) > 1e-10) {
                    maxDegree = max(maxDegree, p.first);
                }
            }

            double a = 0, b = 0, c = 0, d = 0;
            if (coeffs.count(3)) a = coeffs[3];
            if (coeffs.count(2)) b = coeffs[2];
            if (coeffs.count(1)) c = coeffs[1];
            if (coeffs.count(0)) d = coeffs[0];

            vector<double> roots;

            if (maxDegree == 1) {
                double x;
                if (solveLinear(c, d, 0, x)) {
                    roots.push_back(x);
                }
            } else if (maxDegree == 2) {
                roots = solveQuadratic(b, c, d, 0);
            } else if (maxDegree == 3) {
                roots = solveCubic(a, b, c, d, 0);
            } else if (maxDegree == 0) {
                if (abs(d) < 1e-10) {
                    continue;
                } else {
                    cout << "该方程无解" << endl;
                    return;
                }
            }

            for (double r : roots) {
                map<char, double> testVars;
                testVars[var] = r;
                double l = evaluate(eq.left, testVars);
                double rt = evaluate(eq.right, testVars);

                if (abs(l - rt) < 1e-4) {
                    allSolutions.push_back(r);
                    hasSolution = true;
                }
            }
        }

        if (!hasSolution) {
            cout << "无解" << endl;
            return;
        }

        sort(allSolutions.begin(), allSolutions.end());
        vector<double> uniqueSol;
        for (double s : allSolutions) {
            if (uniqueSol.empty() || abs(s - uniqueSol.back()) > 1e-4) {
                uniqueSol.push_back(s);
            }
        }

        if (equations.size() > 1) {
            vector<double> common;
            for (double s : uniqueSol) {
                bool valid = true;
                for (auto& eq : equations) {
                    map<char, double> testVars;
                    testVars[var] = s;
                    double l = evaluate(eq.left, testVars);
                    double r = evaluate(eq.right, testVars);
                    if (abs(l - r) > 1e-4) {
                        valid = false;
                        break;
                    }
                }
                if (valid) common.push_back(s);
            }
            uniqueSol = common;
        }

        if (uniqueSol.empty()) {
            cout << "无共同解" << endl;
            return;
        }

        cout << fixed << setprecision(6);
        for (double s : uniqueSol) {
            if (abs(s - round(s)) < 1e-6) {
                cout << var << "=" << (int)round(s) << endl;
            } else {
                cout << var << "=" << s << endl;
            }
        }
    }

    void solveMultiVariable() {
        int n = allVars.size();
        int m = equations.size();

        if (m < n) {
            cout << "方程不足，有无穷多解" << endl;
            return;
        }

        vector<vector<double>> A(m, vector<double>(n, 0));
        vector<double> B(m, 0);

        for (int i = 0; i < m; i++) {
            auto& eq = equations[i];

            for (int j = 0; j < n; j++) {
                char var = allVars[j];
                map<char, double> zeroVars;
                for (char v : allVars) zeroVars[v] = 0;

                map<char, double> testVars = zeroVars;
                testVars[var] = 1;
                double val1 = evaluate(eq.left, testVars) - evaluate(eq.right, testVars);
                testVars[var] = 0;
                double val0 = evaluate(eq.left, testVars) - evaluate(eq.right, testVars);
                A[i][j] = val1 - val0;
                B[i] = -val0;
            }
        }

        vector<double> sol = solveLinearSystem(A, B);

        bool valid = true;
        for (auto& eq : equations) {
            map<char, double> vars;
            for (int j = 0; j < n; j++) {
                vars[allVars[j]] = sol[j];
            }
            double l = evaluate(eq.left, vars);
            double r = evaluate(eq.right, vars);
            if (abs(l - r) > 1e-4) {
                valid = false;
                break;
            }
        }

        if (!valid) {
            cout << "无非线性方程组的数值解（或需要更复杂的解法）" << endl;
            return;
        }

        cout << fixed << setprecision(6);
        for (int i = 0; i < n; i++) {
            double s = sol[i];
            if (abs(s - round(s)) < 1e-6) {
                cout << allVars[i] << "=" << (int)round(s);
            } else {
                cout << allVars[i] << "=" << s;
            }
            if (i < n-1) cout << ",";
        }
        cout << endl;
    }
};

int main() {
    cout << "=== 方程求解器 ===" << endl;
    cout << "支持：3元以下，3次以下的方程" << endl;
    cout << "支持所有字母作为未知数，支持负数运算" << endl;
    cout << "输入格式示例：" << endl;
    cout << "  2a+3b=100,a-b=10" << endl;
    cout << "  3m+(-2)=0" << endl;
    cout << "  -2p^2+3p-5=0" << endl;
    cout << "  3x^2+2x+3=50" << endl;
    cout << "输入q退出" << endl << endl;

    while (true) {
        cout << ">> ";
        string input;
        getline(cin, input);

        if (input == "q" || input == "Q") break;
        if (input.empty()) continue;

        EquationSolver solver;
        solver.parseInput(input);
        solver.solve();
        cout << endl;
    }
    std::cin.get();
    return 0;
}