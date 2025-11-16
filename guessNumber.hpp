#ifndef GUESS_NUMBER_HPP
#define GUESS_NUMBER_HPP

#include <string>
#include <vector>

#include "nlohmann/json.hpp"

#define GAME_VERSION "2.3.0"

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

//多人模式（网络对战）（玩家AI对战）
//单次猜测限时、单次游戏整体限时
//动态难度、范围、复合判定
//体力值系统、商店系统、成就系统
//动态反馈、主题切换
//配置文件、支持多语言
//排行榜：本地、网络
//数据统计：成功率、平均猜测次数

//电脑二分法查找演示（）

//进行存档

// //特殊输入列表
// const std::vector<std::string> specialInput_exit =
// {"exit", "quit",};
// //特殊输入列表
// const std::vector<std::string> specialInput_restart =
// {"res", "restart",};

struct guessNumber_config
{
    //游戏结束时，决定是否保存
    bool configSaving;
    //配置数据
    std::string languageFilePath;
    //判断配置文件是否存在
    bool ConfigFileExists();
    //读取配置文件
    void LoadConfigFile();
    //保存配置
    void SaveConfig();
};

struct guessNumber_archive
{
    //游戏结束时，决定是否保存
    bool archiveSaving;
    //存档的财富值
    int wealth;
    //判断存档文件是否存在
    bool ArchiveFileExists();
    //读取存档文件
    void LoadArchiveFile();
    //保存存档
    void SaveArchive();
};

class guessNumber
{
    //保存游戏状态
    int gameStatus;
    //保存难度数据（用于恢复）
    int recoveryDifficulty, recoveryLimitedTimes, recoveryMax;
    //保存当前难度数据
    int difficulty, limitedTimes, random;
    //保存当前猜测数据
    int guess, minGuess, maxGuess;
    guessNumber_config ganeConfig;
    guessNumber_archive gameArchive;
    private:
        void SaveAll();
        //判断特殊输入
        void SpecialInput(std::string input);
        //输入string转int
        int StringtoInt(std::string input);
        //选择难度
        void ChooseDifficulty();
        //内置难度填充
        void InbuiltDifficultyFilling();
        //自定义最大随机值
        void SetRandomLimit_UserDefined();
        //自定义最大猜测次数
        void SetLimitedTimes_UserDefined();
        //设置难度
        void SetDifficulty();
        //重置难度相关数据
        void ResetDifficulty();
        //生成随机数
        void GenerateRandom();
        //判断输入值是否在规定范围内
        bool OutOfRange(int inputNumber);
        //判断猜测值相对被猜数的大小
        bool SizeJudgement();
        //处理猜测值
        void DealPlayerGuess();
        //增加财富
        void WealthIncrease();
        //机器二分法搜索演示
        void ComputerGuessDemo();
        //打印猜测线索
        void PrintClue();
        //进行一次游戏
        void SinglePlay();
        //单次游戏解释处理
        void SingleEnd();
    public:
        //重复游玩直至玩家自主退出
        void RepeatedlyPlay();
};

//创建一个文件不管其是否存在
bool FileCreate(std::string fileName);
//打印信息
void Print(std::string message);
//打印信息（加入换行符）
void Println(std::string message);

#endif