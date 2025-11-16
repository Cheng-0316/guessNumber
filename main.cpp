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
    myGame.RepeatedlyPlay();
    return 0;
}
