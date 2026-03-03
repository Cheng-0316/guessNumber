#ifndef GUESS_NUMBER_HPP
#define GUESS_NUMBER_HPP

#include <string>

#define CHENG_GUESS_NUMBER_VERSION_MAJOR 3
#define CHENG_GUESS_NUMBER_VERSION_MINOR 0
#define CHENG_GUESS_NUMBER_VERSION_PATCH 0

#define INPUT_UNAVAILABLE 1
#define GAME_END 2
#define GAME_CONTINUE 3
#define GAME_RESTART 4

#define EASY_MAX_NUMBER 64
#define NORMAL_MAX_NUMBER 128
#define HARD_MAX_NUMBER 1024

#define EASY_LIMITED_TIMES -1
#define NORMAL_LIMITED_TIMES 7
#define HARD_LIMITED_TIMES 8

//多人模式（网络对战）（玩家AI对战）
//单次猜测限时、单次游戏整体限时
//动态难度、范围、复合判定
//体力值系统、商店系统、成就系统
//动态反馈、主题切换
//配置文件、支持多语言
//排行榜：本地、网络
//数据统计：成功率、平均猜测次数
//电脑二分法查找演示（）

//打印信息
void Print(std::string output);
//打印信息（加入换行符）
void Println(std::string output);
//读取输入
std::string ReadString();

struct playTimes
{
    int playTimes;
    int winTimes;
};

struct guessNumberData
{
	int wealth;
    // playTimes total;
    // playTimes easyMode;
    // playTimes normalMode;
    // playTimes hardMode;
    // playTimes 
};

class guessNumber
{
    //游戏状态
    int gameStatus;
    //难度数据（用于保持相同难度继续游玩）
    int recoveryDifficulty, recoveryLimitedTimes, recoveryMax;
    //当前难度数据
    int difficulty, limitedTimes, random;
    //当前猜测数据
    int guess, minGuess, maxGuess;
    private:
        //判断特殊输入
        void SpecialInput(std::string input);
        //输入string转int
        int StringtoInt(std::string input);
        //设置难度
        void SetDifficulty();
        //重置难度数据
        void ResetDifficulty();
        //生成随机数
        void GenerateRandom();
        //处理猜测值
        void HandlePlayerGuess();
        //增加财富
        void WealthIncrease();
        //进行一次游戏
        void PlayOnce();
    public:
        guessNumberData saveData;
        //重复游玩直至玩家自主退出
        void Play();
};

#endif