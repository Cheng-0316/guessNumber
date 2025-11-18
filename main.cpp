/*********************************************
 * 项目说明：
 * 设计人：程创客
 * 创建时间：2025年9月
 * 内容：一个猜数字游戏
 * 注意：本项目需要使用C++17及以上版本编译运行
 *********************************************/

#include <iostream>
#include <string>

#include "nlohmann/json.hpp"
#include "guessNumber.hpp"

using std::cin;
using std::cout;
using std::endl;

using std::stoi;
using std::string;

guessNumber myGame;

int main()
{
    cout << "该项目使用了 nlohmann 的 json 项目" << endl;
    cout << "链接：https://github.com/nlohmann/json" << endl;
    cout << "版本：v";
    cout << NLOHMANN_JSON_VERSION_MAJOR;cout << ".";
    cout << NLOHMANN_JSON_VERSION_MINOR;cout << ".";
    cout << NLOHMANN_JSON_VERSION_PATCH << endl;
    cout << "协议：MIT" << endl;
    
    myGame.RepeatedlyPlay();
    return 0;
}
