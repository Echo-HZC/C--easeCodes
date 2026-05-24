#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

const std::string RESET = "\033[0m";
const std::string BLUE = "\033[34m";      // 蓝色
const std::string GREEN = "\033[32m";     // 绿色
const std::string RED = "\033[31m";       // 红色
const std::string YELLOW = "\033[33m";    // 黄色

std::string lesson[3][5] = {
    {
        "The quick brown fox jumps over the lazy dog. ",
        "Pack my box with five dozen liquor jugs.",
        "How vexingly quick daft zebras jump!",
        "A mad boxer shot a quick, gloved jab to the jaw of his dizzy opponent.",
        "The job requires extra pluck and zeal from every young wage earner."
    },
    {
        "Sphinx of black quartz, judge my vow.",
        "Two driven jocks help fax my big quiz.",
        "The five boxing wizards jump quickly.",
        "Crazy Fredrick bought many very exquisite opal jewels.",
        "We promptly judged antique ivory buckles to be worth five dozen yuan."
    },
    {
        "Mr. Jock, TV quiz PhD, bags few lynx.",
        "\"Quick zephyrs blow, vexing daft Jim.\"",
        "Amazingly few discotheques provide jukeboxes.",
        "The wizard quickly jinxed the gnomes before they vaporized his amazing, yet fragile, quartz lamp!",
        "\"Zelda,\" whispered the vexed phantom, \"quickly grab my jacked-up BMW before those five crazy dogs jump over it!\""
    }
};

void showError(const std::string& input, const std::string& target) {
    std::cout << RED << "Typing error!" << RESET << std::endl;
    std::cout << "Expected: " << BLUE << target << RESET << std::endl;
    std::cout << "Your input: ";

    for (size_t i = 0; i < input.size() || i < target.size(); ++i) {
        char c = (i < input.size()) ? input[i] : ' ';
        if (i >= target.size() || c != target[i]) {
            std::cout << RED << c << RESET;  // 错误字符标红
        } else {
            std::cout << GREEN << c << RESET; // 正确字符标绿
        }
    }
    std::cout << std::endl;
}


int main()
{

    std::cout << BLUE <<
        "Typing practice, press the s key to skip and the q key to exit"
    << RESET << std::endl;
    std::cout << std::endl;

    std::string input;
    int error = 0;
    bool alreadyFailed = false;
    for (int i = 0; i < 3; i++) {
        switch (i) {
            case 0: {
                std::cout << YELLOW << "Easy mode:" << RESET << std::endl;
                break;
            }
            case 1: {
                std::cout << YELLOW << "Mid mode:" << RESET << std::endl;
                break;
            }
            case 2: {
                std::cout << YELLOW << "Difficult mode:" << RESET << std::endl;
                break;
            }
            default: {
                break;
            }
        }
        std::cout << std::endl;
        for (int j = 0;j < 5;j++) {
            alreadyFailed = true;
            switch (j) {
                case 0: {
                    std::cout << "The first:" << std::endl;
                    break;
                }
                case 1: {
                    std::cout << "The second:" << std::endl;
                    break;
                }
                case 2: {
                    std::cout << "The third:" << std::endl;
                    break;
                }
                case 3: {
                    std::cout << "The fourth:" << std::endl;
                    break;
                }
                case 4: {
                    std::cout << "The fifth:" << std::endl;
                    break;
                }
                default: {
                    break;
                }

            }
            std::cout << BLUE << lesson[i][j] << RESET << std::endl;
            while (true) {
                std::cout << "Please enter: ";
                getline(std::cin,input);
                if (input.empty()) {
                    std::cout << "Input cannot be empty, please try again." << std::endl;
                    continue;
                }
                if (input == "q") {
                    std::cout << GREEN << "Exit." <<  RESET<< std::endl;
                    return 0;
                }
                if (input == "s") {
                    if (alreadyFailed) {
                        error++;
                        alreadyFailed = false;
                    }
                    std::cout << YELLOW << "Skipped." << RESET << std::endl << std::endl;
                    break;
                }
                if (input == lesson[i][j]){
                    std::cout << GREEN << "You are right." << RESET << std::endl << std::endl;
                    std::cout << std::endl;
                    break;
                }
                if (alreadyFailed) {
                    error++;
                    alreadyFailed = false;
                }
                showError(input,lesson[i][j]);
            }
        }
        std::cout << "Congratulations on passing the test!" << std::endl;
        std::cout << std::endl;
        if (i < 2) {
            std::cout << GREEN << "Level complete! Press Enter to continue..." << RESET;
            std::string dummy;
            std::getline(std::cin, dummy);
        }
    }
    if (error == 0) {
        std::cout << GREEN << "You have no mistakes." << std::endl;
        std::cout << "You win!" << RESET << std::endl;
        return 0;
    }
    std::cout << "You made a total of (" << error <<"/15) mistake(s)." << std::endl;
    std::cout << "Your correct rate is:" << std::fixed << std::setprecision(2) << (1.0 -(double)error / 15.0) * 100.0 << "%." << std::endl;
    std::cout << "Keep going!" << std::endl;
    return 0;
}