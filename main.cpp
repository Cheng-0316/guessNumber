/****************************************************************
 * 项目说明：
 * 设计人：程创客
 * 创建时间：2025年9月
 * 内容：一个猜数字游戏
 ****************************************************************/

//多人模式（网络对战）（玩家AI对战）
//电脑二分法查找演示
//单次猜测限时、单次游戏整体限时
//动态难度、范围、复合判定
//体力值系统、商店系统、成就系统
//动态反馈、主题切换
//配置文件、支持多语言、保存记录、金钱等
//排行榜：本地、网络
//数据统计：成功率、平均猜测次数

#include <iostream>
#include <random>
#include <string>

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
