#ifndef GUESS_NUMBER_HPP
#define GUESS_NUMBER_HPP

#include <iostream>
#include <string>
#include <random>

#define CHENG_GUESS_NUMBER_VERSION_MAJOR 4
#define CHENG_GUESS_NUMBER_VERSION_MINOR 0
#define CHENG_GUESS_NUMBER_VERSION_PATCH 0

enum GameStatus
{
    GAME_CONTINUE,
    GAME_END,
    GAME_RESTART,
    INPUT_UNAVAILABLE
};

constexpr int EASY_MIN_NUMBER = 1;
constexpr int NORMAL_MIN_NUMBER = -63;
constexpr int HARD_MIN_NUMBER = -511;

constexpr int EASY_MAX_NUMBER = 64;
constexpr int NORMAL_MAX_NUMBER = 64;
constexpr int HARD_MAX_NUMBER = 512;

constexpr int EASY_LIMITED_TIMES = -1;
constexpr int NORMAL_LIMITED_TIMES = 7;
constexpr int HARD_LIMITED_TIMES = 8;

//多人模式（网络对战）（玩家AI对战）
//单次猜测限时、单次游戏整体限时
//动态难度、范围、复合判定
//体力值系统、商店系统、成就系统
//动态反馈、主题切换
//配置文件、支持多语言
//排行榜：本地、网络
//数据统计：成功率、平均猜测次数
//电脑二分法查找演示（）

//输出
inline void Print(const std::string &output)
{
    std::cout << output << std::flush;
}

//带回车的输出
inline void Println(const std::string &output)
{
    Print(output + "\n");
}

//输入
inline std::string ReadLine()
{
    std::string input;
    std::getline(std::cin, input);
    return input;
}

// struct playStates
// {
//     int playCount; //游玩次数
//     int wins;      //赢的次数
// };

struct guessNumberData
{
	int wealth;            //财富值
    // playStates total;      //总游玩数据
    // playStates easyMode;   //简单模式数据
    // playStates normalMode; //中等模式数据
    // playStates hardMode;   //困难模式数据
    // playStates customMode; //自定义模式数据
};


class guessNumber
{
    std::random_device rd;
    std::mt19937 gen;
    //游戏状态
    GameStatus gameStatus = GAME_CONTINUE;
    //难度数据（用于保持相同难度继续游玩）
    int recoveryDifficulty = 0, recoveryLimitedTimes = 0, recoveryMin = 0, recoveryMax = 0;
    //当前难度数据
    int difficulty = 0, guessTimes = 0, random = 0;
    //当前猜测数据
    //minGuess和maxGuess代表当前猜测范围为 (minGuess，maxGuess)（开区间）
    int guess = 0, minGuess = 0, maxGuess = 0;
    guessNumberData saveData{0};
    private:
        //判断特殊输入
        void SpecialInput(std::string input);
        //输入string转int
        int StringtoInt(std::string input);
        //游戏开始界面
        // void StartPage();
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
    guessNumber() : gen(rd()) {}
        //重复游玩直至玩家自主退出
        void Play();
        guessNumberData GetSaveData()
        {
            return saveData;
        }
        void SetSaveData(guessNumberData newSaveData)
        {
            saveData = newSaveData;
        }
};

#endif