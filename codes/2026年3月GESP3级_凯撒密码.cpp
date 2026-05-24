/*3.2.1 题目描述
凯撒密码是一种替换加密技术，明文中的所有字母都在字母表上向后（或向前）按照一个固定数目进行偏移后被替
换成密文。例如，当偏移量是3的时候，所有的字母A将被替换成D，B被替换成E，C被替换成 F以此类推，
W被替换成Z，X被替换成A ，Y被替换成 B，Z 被替换成 C。这个加密方法是以罗马共和时期凯撒的名字命名
的，据称当年凯撒曾用此方法与其将军们进行联系。
但是和所有的利用字母表进行替换的加密技术一样，凯撒密码非常容易被破解，而且在实际应用中也无法保证通信
安全。
现在给你一个已破解的凯撒密码明文与密文，与一个有相同偏移量的未破解凯撒密码密文，请你帮忙破解它。
3.2.2 输入格式
输入共三行：
第一行包含一个字符串，表示已破解的凯撒密码明文；
第二行包含一个字符串，表示已破解的凯撒密码密文；
第三行包含一个字符串，表示待破解的凯撒密码密文。
3.2.3 输出格式
输出一行，包含一个字符串，表示待破解的凯撒密码对应的明文。
3.2.4 样例
3.2.4.1 输入样例
ABCDEFGVWXYZ
DEFGHIJYZABC
WKHTXLFNEURZQIRAMXPSVRYHUWKHODCBGRJ
3.2.4.2 输出样例
THEQUICKBROWNFOXJUMPSOVERTHELAZYDOG
3.2.5 样例解释
样例 1 中，通过已破解的密码得出偏移量为 'D' - 'A' = 3 ，因此，对未破解部分进行逆向偏移：密文中的 W 对
应明文中的 T （ 'W' - 3 = 'T' ），密文中的 K 对应明文中的 H （ 'K' - 3 = 'H' ），以此类推。
3.2.6 数据范围
保证密码长度均不超过 1000，所有字符串由大写字母组成。
*/
#include<bits/stdc++.h>


int main()
{
    std::string plaintext1,ciphertext1,ciphertext2;
    std::cin >> plaintext1 >> ciphertext1 >> ciphertext2;
    int move = ((ciphertext1[0] - plaintext1[0]) % 26 + 26) % 26;;
    for(int i = 0;i < ciphertext2.length();i++) {
        if (ciphertext2[i] - move < 65) {
            std::cout << (char)(ciphertext2[i] - move + 26);
            continue;
        }
        std::cout << (char)(ciphertext2[i] - move);
    }
    return 0;
}