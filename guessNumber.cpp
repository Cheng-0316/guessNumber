#include <iostream>
#include <random>
#include <string>

#include "guessNumber.hpp"

//输出
inline void Print(std::string output)
{
    std::cout << output << std::flush;
}

//带回车的输出
inline void Println(std::string output)
{
    Print(output + "\n");
}

//输入
inline std::string ReadString()
{
    std::string input;
    std::cin >> input;
    return input;
}


//判断特殊输入
void guessNumber::SpecialInput(std::string input)
{
    if (input == "end")
    {
        gameStatus = GAME_END;
    }
    if (input == "res" || input == "restart")
    {
        gameStatus = GAME_RESTART;
    }
}

//输入string转int
int guessNumber::StringtoInt(std::string input)
{
    int number = 0;
    try
    {
        number = std::stoi(input);
    }
    catch (const std::exception &e)
    {
        Println(e.what());
        Println("输入不可识别，");
        gameStatus = INPUT_UNAVAILABLE;
    }
    return number;
}


//用户设置难度
void guessNumber::SetDifficulty()
{
    std::string input;
    std::ostringstream oss;
    do
    {
        gameStatus = GAME_CONTINUE;
        oss.str("");
        oss << "请选择难度:\n";
        oss << "1：简单（1-" << EASY_MAX_NUMBER << "，无猜测次数限制）\n";
        oss << "2：中等（1-" << NORMAL_MAX_NUMBER << "，限制猜测次数" << NORMAL_LIMITED_TIMES << "）\n";
        oss << "3：困难（1-" << HARD_MAX_NUMBER << "，限制猜测次数" << HARD_LIMITED_TIMES << "）\n";
        oss << "其他数字：自定义\n选择难度：";
        Println(oss.str());
        recoveryDifficulty = StringtoInt(ReadString());
    } while (gameStatus != GAME_CONTINUE);
    //所有值为先保存至恢复数据中，再填充到游戏数据中
    if (recoveryDifficulty < 1 || recoveryDifficulty > 3)
    {
        //如果用户自定义难度
        //让用户自己输入随机数范围和限制猜测次数
        do
        {
            gameStatus = GAME_CONTINUE;
            Println("请输入自定义最大猜测值：");
            input = ReadString();
            recoveryMax = StringtoInt(input);
            if (gameStatus == INPUT_UNAVAILABLE)
            {
                continue;
            }
            if (recoveryMax < 2)
            {
                Println("请确保输入值大于1。");
                continue;
            }
        } while (gameStatus != GAME_CONTINUE);
        do
        {
            gameStatus = GAME_CONTINUE;
            Println("请输入自定义最大猜测次数（输入负数代表无限制）：");
            input = ReadString();
            recoveryLimitedTimes = StringtoInt(input);
        } while (gameStatus != GAME_CONTINUE);
    }
    else
    {
        //自动填充难度相关恢复数据，使用内置数据
        switch (recoveryDifficulty)
        {
        case 1:
            recoveryMax = EASY_MAX_NUMBER;
            recoveryLimitedTimes = EASY_LIMITED_TIMES;
            break;
        case 2:
            recoveryMax = NORMAL_MAX_NUMBER;
            recoveryLimitedTimes = NORMAL_LIMITED_TIMES;
            break;
        case 3:
            recoveryMax = HARD_MAX_NUMBER;
            recoveryLimitedTimes = HARD_LIMITED_TIMES;
            break;
        default:
            break;
        }
    }
    recoveryMax++;
    //此处的重置难度是为了自动填充
    ResetDifficulty();
}

void guessNumber::ResetDifficulty()
{
    minGuess = 0;
    maxGuess = recoveryMax;
    limitedTimes = recoveryLimitedTimes;
    difficulty = recoveryDifficulty;
}

inline void guessNumber::GenerateRandom()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    // 减一确保生成的随机数在最大值以内，防止其等于最大值，导致用户无法猜到
    std::uniform_int_distribution<> dis(1, maxGuess - 1);
    random = dis(gen);

    // Println("调试信息：随机数为" + std::to_string(random));
}

    

void guessNumber::HandlePlayerGuess()
{// 判断用户输入的值是否在范围内
    if (guess <= minGuess || guess >= maxGuess)
    {
        Println("输入的数字不在范围内，");
    }
    limitedTimes--;
    if (guess == random)
    {
        Println("猜对了！");
        WealthIncrease();
        gameStatus = GAME_END;
        return;
    }
    if (guess < random)
    {
        Println("猜的太小了！");
        minGuess = guess;
        gameStatus = GAME_CONTINUE;
    }
    if (guess > random)
    {
        Println("猜的太大了！");
        maxGuess = guess;
        gameStatus = GAME_CONTINUE;
    }

    if (limitedTimes == 0)
    {
        Println("没机会啦，正确答案是：" + std::to_string(random));
        gameStatus = GAME_END;
    }
    if (minGuess + 2 == maxGuess)
    {
        Println("哎呀呀，就差一点儿，正确答案是：" + std::to_string(random));
        gameStatus = GAME_END;
    }
}

void guessNumber::WealthIncrease()
{
    int wealthIncrease = 0;
    if (difficulty == 1)
    {
        wealthIncrease += 5;
    }
    else if (difficulty == 2)
    {
        wealthIncrease += 10;
        wealthIncrease += limitedTimes * 2;
    }
    else if (difficulty == 3)
    {
        wealthIncrease += 20;
        wealthIncrease += limitedTimes * 2;
    }
    if (wealthIncrease <= 0)
    {
        // Println("财富无变化。");
    }
    else
    {
        Println("财富增加了" + std::to_string(wealthIncrease) + "。");
        saveData.wealth += wealthIncrease;
    }
    Println("目前财富值为：" + std::to_string(saveData.wealth) + "。");
}


void guessNumber::PlayOnce()
{
    Println("财富为：" + std::to_string(saveData.wealth));
    GenerateRandom();
    std::string input;
    do
    {
        gameStatus = GAME_CONTINUE;

        // 打印猜测范围和剩余猜测次数等提醒
        Println("（输入end终止此次游戏，输入restart或res开始新一轮）");
        if (limitedTimes < 0)
        {
            Println("无限制猜测次数。");
        }
        else
        {
            Println("还剩" + std::to_string(limitedTimes) + "猜测次数。");
        }
        Print("输入一个大于" + std::to_string(minGuess) + "且小于" + std::to_string(maxGuess) + "的整数：");

        input = ReadString();
        SpecialInput(input);
        if (gameStatus == GAME_END || gameStatus == GAME_RESTART)
        {
            break;
        }

        guess = StringtoInt(input);
        if (gameStatus == INPUT_UNAVAILABLE)
        {
            continue;
        }
        HandlePlayerGuess();
        if (gameStatus == INPUT_UNAVAILABLE)
        {
            continue;
        }
    } while (gameStatus != GAME_END);
    std::string choice;
}

void guessNumber::Play()
{
    // ShowStartPage();
    SetDifficulty();
    std::string choice;
    while (true)
    {
        PlayOnce();
        //处理单次游戏结束
        if (gameStatus == GAME_END)
        {
            Println("是否继续游戏？（是/否）");
            choice = ReadString();
            if (choice == "y" || choice == "Y" || choice == "是")
            {
                gameStatus = GAME_RESTART;
            }
            else
            {
                break;
            }
        }
        if (gameStatus == GAME_RESTART)
        {
            Println("是否重新选择难度？（是/否）");
            choice = ReadString();
            if (choice == "y" || choice == "Y" || choice == "是")
            {
                SetDifficulty();
            }
            else
            {
                ResetDifficulty();
            }
        }
    }
}
