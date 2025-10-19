/****************************************************************
 * 项目说明：
 * 设计人：程创客
 * 创建时间：2025年9月
 * 内容：一个猜数字游戏
 ****************************************************************/

//二分法查找演示
//双人模式（网络对战）
//电脑猜数模式
//限时
//动态难度
//商店系统
//成就系统
//主题切换
//支持多语言
//保存记录、金钱等
//配置文件系统
//排行榜：本地、网络
//数据统计：成功率、平均猜测次数

#include <iostream>
#include <random>
#include <string>

#include "guessNumber.h"

using std::cin;
using std::cout;
using std::endl;

using std::stoi;
using std::string;

guessNumber myGame;

int main()
{
    myGame.RepeatedlyPlay();
    // myGame.ComputerGuessDemo();
    // std::cout << "按回车健退出……" << std::endl;
    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    // std::cin.get();
    return 0;
}
