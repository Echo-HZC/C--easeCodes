#include <bits/stdc++.h>
#include <windows.h>

const std::string RED = "\033[31m";       // 红色
const std::string RESET = "\033[0m";

bool isNumber(std::string str) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            continue;
        }
        return false;
    }
    return true;
}

void playBoom() {
    for (int f = 2000; f >= 800; f -= 400) {
        Beep(f, 50);
    }
    Sleep(80);

    Beep(1200, 60);
    Beep(800, 70);
    Beep(500, 90);
    Beep(250, 120);
    Beep(120, 500);
}

int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    int bomb = dis(gen);
    std::string bomb_str = std::to_string(bomb);
    int numberOfAttempts = 0;
    std::string input_str;
    int low = 1,high = 100,last_guess = -1;

    std::cout << "Digital bomb" <<std::endl;
    std::cout << "Press q to exit" <<std::endl;

    while (true) {
        std::cin >> input_str;

        if (input_str == "q") {
            std::cout << "Exit." << std::endl;
            break;
        }

        if (!isNumber(input_str)) {
            std::cout << "Invalid input." << std::endl;
            continue;
        }

        int input = std::stoi(input_str);

        if (input < 1 || input > 100) {
            std::cout << "Please enter a number between 1 and 100." << std::endl;
            continue;
        }

        numberOfAttempts++;

        if (input == bomb) {
            playBoom();
            std::cout << RED << "boom!!!" << RESET << std::endl;
            break;
        }

        if (numberOfAttempts > 1 && input == last_guess) {
            std::cout << "Why are you entering the same wrong number?" << std::endl;
            continue;
        }

        if (input < bomb) {
            low = std::max(low, input);
        }
        else {
            high = std::min(high, input);
        }


        std::cout <<
            "The bomb is between " << low << " and " << high << ", guess again!"
        << std::endl;

        last_guess = input;
    }
    std::cout << "You tried：" << numberOfAttempts << "times." << std::endl;

    std::cin.get();
    return 0;
}