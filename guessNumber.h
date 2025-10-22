#ifndef GUESS_NUMBER_H
#define GUESS_NUMBER_H

#define GAME_VERSION "2.1.2"

#define INPUT_UNAVAILABLE 1
#define GAME_ERROR 2
#define PROGRAM_ERROR 4
#define WHOLE_GAME_END_NORMALLY 8
#define SINGLE_GAME_END_NORMALLY 16
#define GAME_CONTINUE 32
#define GAME_RESTART 64

#define EASY_MAX_NUMBER 64
#define NORMAL_MAX_NUMBER 128
#define HARD_MAX_NUMBER 1024

#define EASY_LIMITED_TIMES -1
#define NORMAL_LIMITED_TIMES 7
#define HARD_LIMITED_TIMES 8

#include <string>

class guessNumber
{
    int gameStatus;
    int recoveryGuessLimit, recoveryMax;
    int difficulty, limitedGuessTimes, randomNumber;
    int guessNumber, minGuessLimit, maxGuessLimit;
    int wealth;
    private:
        //提出特殊输入
        void SpecialInputJudgement(std::string input);
        //输入string转int
        int InputStringtoInt(std::string input);
        //选择难度
        void ChooseDifficulty();
        //内置难度填充
        void InbuiltDifficultyFilling();
        //用户自定义最大随机值
        void SetUserDefinedMaxRandom();
        //用户自定义最大猜测次数
        void SetUserDefinedMaxGuessLimit();
        //重置难度
        void ResetDifficulty();
        //设置难度
        void SetDifficulty();
        //生成随机数
        void GenerateRandom();
        //判断用户的输入值是否在规定范围内
        bool RangeJudgement(int inputNumber);
        //判断用户的猜测值相对被猜数的大小
        bool SizeJudgement();
        //处理用户猜测值
        void DealPlayerGuess();
        //
        void WealthIncrease();
        //进行一次游戏
        void SinglePlay();
        //单次游戏解释处理
        void SingleEndDealing();
    public:
        //机器二分法搜索演示
        void ComputerGuessDemo();
        //重复游玩直至玩家自主退出
        void RepeatedlyPlay();
};

#endif