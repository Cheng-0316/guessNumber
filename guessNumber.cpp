#include <iostream>
#include <sstream>
#include <random>
#include <string>
#include <cmath>
#include <limits>

#include "guessNumber.hpp"


//判断特殊输入
void guessNumber::SpecialInput(std::string input)
{
    // input 转换为全小写，以便于比较
    std::string lowerInput;
    for (char c : input)
    {
        lowerInput += std::tolower(c);
    }
    if (lowerInput == "end")
    {
        gameStatus = GAME_END;
    }
    if (lowerInput == "res" || lowerInput == "restart")
    {
        gameStatus = GAME_RESTART;
    }
}

//输入string转int
int guessNumber::StringtoInt(std::string input)
{
    int number = 0;
    std::size_t pos = 0;
    try
    {
        number = std::stoi(input, &pos);
    }
    catch (const std::out_of_range &e)
    {
        Println(e.what());
        Println("输入的数字超出了游戏所能储存的最大值，");
        gameStatus = INPUT_UNAVAILABLE;
        return 0; // 返回一个默认值，虽然这个值不太可能在正常游戏中出现，但可以防止未定义行为
    }
    catch (const std::invalid_argument &e)
    {
        Println(e.what());
        Println("输入的不是一个有效的整数，");
        gameStatus = INPUT_UNAVAILABLE;
        return 0;
    }
    catch (const std::exception &e) //以防万一
    {
        Println(e.what());
        Println("输入不可识别，");
        gameStatus = INPUT_UNAVAILABLE;
        return 0;
    }
    if (pos != input.size())
    {
        Println("输入包含非法字符，");
        gameStatus = INPUT_UNAVAILABLE;
    }
    return number;
}


//用户设置难度
void guessNumber::SetDifficulty()
{
    std::string input;
    do
    {
        gameStatus = GAME_CONTINUE;
        Println("请选择难度:");
        Println("1：简单（[" + std::to_string(EASY_MIN_NUMBER) + "," + std::to_string(EASY_MAX_NUMBER) + "]，无猜测次数限制）");
        Println("2：中等（[" + std::to_string(NORMAL_MIN_NUMBER) + "," + std::to_string(NORMAL_MAX_NUMBER) + "]，限制猜测次数" + std::to_string(NORMAL_LIMITED_TIMES) + "）");
        Println("3：困难（[" + std::to_string(HARD_MIN_NUMBER) + "," + std::to_string(HARD_MAX_NUMBER) + "]，限制猜测次数" + std::to_string(HARD_LIMITED_TIMES) + "）");
        Println("其他数字：自定义\n选择难度：");
        recoveryDifficulty = StringtoInt(ReadLine());
    } while (gameStatus != GAME_CONTINUE);
    //所有值为先保存至恢复数据中，再填充到游戏数据中
    if (recoveryDifficulty < 1 || recoveryDifficulty > 3)
    {
        //如果用户自定义难度
        //让用户自己输入随机数范围和限制猜测次数
        do
        {
            gameStatus = GAME_CONTINUE;
            Println("请输入自定义猜测范围（请同时输入最小值和最大值，两者用空格分隔，后续数字省略）：");
            input = ReadLine();
            std::istringstream iss(input);
            int tempMin = 0, tempMax = 0;
            if (iss >> tempMin >> tempMax)
            {
                recoveryMin = tempMin;
                recoveryMax = tempMax;
                if (recoveryMax == recoveryMin)
                {
                    Println("最大值和最小值不能相等。");
                    gameStatus = INPUT_UNAVAILABLE;
                    continue;
                }
                if (recoveryMin == INT_MIN && recoveryMax == INT_MAX)
                {
                    Println("范围过大可能会导致游戏无法正常进行，请重新输入。");
                    gameStatus = INPUT_UNAVAILABLE;
                    continue;
                }
                if (recoveryMax < recoveryMin)
                {
                    // 交换最大值和最小值
                    int temp;
                    temp = recoveryMax;
                    recoveryMax = recoveryMin;
                    recoveryMin = temp;
                }
            }
            else
            {
                Println("输入格式错误，请重新输入。");
                gameStatus = INPUT_UNAVAILABLE;
                continue;
            }
        } while (gameStatus != GAME_CONTINUE);
        do
        {
            gameStatus = GAME_CONTINUE;
            Println("请输入自定义最大猜测次数（输入负数代表无限制）：");
            input = ReadLine();
            recoveryLimitedTimes = StringtoInt(input);
            // 如果是输入非法字符串导致错误，recoveryLimitedTimes 虽然为 0，但不需要输出错误提示两次
            if (recoveryLimitedTimes == 0 && gameStatus != INPUT_UNAVAILABLE)
            {
                Println("没得玩啦！重来重来！");
                gameStatus = INPUT_UNAVAILABLE;
                continue;
            }
        } while (gameStatus != GAME_CONTINUE);
    }
    else
    {
        //自动填充难度相关恢复数据，使用内置数据
        switch (recoveryDifficulty)
        {
        case 1:
            recoveryMin = EASY_MIN_NUMBER;
            recoveryMax = EASY_MAX_NUMBER;
            recoveryLimitedTimes = EASY_LIMITED_TIMES;
            break;
        case 2:
            recoveryMin = NORMAL_MIN_NUMBER;
            recoveryMax = NORMAL_MAX_NUMBER;
            recoveryLimitedTimes = NORMAL_LIMITED_TIMES;
            break;
        case 3:
            recoveryMin = HARD_MIN_NUMBER;
            recoveryMax = HARD_MAX_NUMBER;
            recoveryLimitedTimes = HARD_LIMITED_TIMES;
            break;
        default:
            break;
        }
    }
    //此处的重置难度是为了自动填充
    ResetDifficulty();
}

void guessNumber::ResetDifficulty()
{
    minGuess = recoveryMin - 1;
    maxGuess = recoveryMax + 1;
    guessTimes = 0;
    difficulty = recoveryDifficulty;
}

inline void guessNumber::GenerateRandom()
{
    // 减一确保生成的随机数在最大值以内，防止其等于最大值，导致用户无法猜到
    // 生成的随机数范围为闭区间
    std::uniform_int_distribution<> dis(recoveryMin, recoveryMax);
    random = dis(gen);

    // Println("调试信息：随机数为" + std::to_string(random));
}


void guessNumber::HandlePlayerGuess()
{
    // 判断用户输入的值是否在范围内
    if (guess <= minGuess || guess >= maxGuess)
    {
        Println("输入的数字不在范围内，");
        gameStatus = GAME_CONTINUE; // 以防万一
        return;
    }
    guessTimes++;
    if (guess == random)
    {
        Println("猜对了！");
        WealthIncrease();
        gameStatus = GAME_END;
        return;
    }
    if (guess < random)
    {
        Println("猜小了！");
        minGuess = guess;
        gameStatus = GAME_CONTINUE;
    }
    if (guess > random)
    {
        Println("猜大了！");
        maxGuess = guess;
        gameStatus = GAME_CONTINUE;
    }

    if (recoveryLimitedTimes >= 0 && guessTimes >= recoveryLimitedTimes)
    {
        Println("没机会啦，正确答案是：" + std::to_string(random));
        gameStatus = GAME_END;
        return; //防止同时发生 minGuess + 2 == maxGuess，导致提示重复输出
    }
    if (minGuess + 2 == maxGuess)
    {
        Println("哎呀呀，就差一点儿，正确答案是：" + std::to_string(random));
        gameStatus = GAME_END;
    }
}

void guessNumber::WealthIncrease()
{
    const double COEFFICIENT = 1.20; // 系数，用于调整财富增加的数值
    int baseIncrease = 0;
    int fortuneIncrease = 0;
    // 范围
    int64_t range = static_cast<int64_t>(recoveryMax) - static_cast<int64_t>(recoveryMin) + 1; // 超出 int 范围的值会导致未定义行为，有待处理
    if (range < 2)
    {
        // 几乎不可能发生
        Println("游戏范围错误。");
        return;
    }
    double log2Range = std::log2(range);
    Println("财富增加清单：");
    baseIncrease = static_cast<int>(std::round(log2Range * COEFFICIENT));

    if (baseIncrease < 1)
    {
        baseIncrease = 1; //至少增加1点财富
    }
    Println("   来自基础奖励：" + std::to_string(baseIncrease) + "。");
    // 基于猜测次数的额外奖励
    //基于 log2Range 计算，不使用 recoveryLimitedTimes，否则会导致在用户自定义模式下刷分
    //用户可以设置一个非常大的范围和一个非常小的猜测次数限制，导致 log2Range 很大但 guessTimes 很小，从而获得大量财富增加
    int standardGuessTimes = static_cast<int>(std::ceil(log2Range)); // 理论上最优的猜测次数
    int remainingGuesses = standardGuessTimes - guessTimes;
    if (remainingGuesses > 0)
    {
        double guessEfficiency = static_cast<double>(remainingGuesses) / standardGuessTimes;
        fortuneIncrease = static_cast<int>(std::round(guessEfficiency * COEFFICIENT));
        Println("   来自运气奖励：" + std::to_string(fortuneIncrease) + "。");
    }

    Print("目前财富由" + std::to_string(saveData.wealth));
    saveData.wealth += baseIncrease + fortuneIncrease;
    Print("增加到了" + std::to_string(saveData.wealth));
    Println("（+" + std::to_string(baseIncrease + fortuneIncrease) + "）。");
}

// 进行一次游戏
void guessNumber::PlayOnce()
{
    Println("财富为：" + std::to_string(saveData.wealth));
    GenerateRandom();
    std::string input;
    do
    {
        gameStatus = GAME_CONTINUE;

        // 打印猜测范围和剩余猜测次数等提醒
        Println("（输入end终止此次游戏，输入restart或res新开一局）");
        if (recoveryLimitedTimes < 0)
        {
            Println("无限制猜测次数。");
        }
        else
        {
            Println("还剩" + std::to_string(recoveryLimitedTimes - guessTimes) + "猜测次数。");
        }
        Print("输入一个大于" + std::to_string(minGuess) + "且小于" + std::to_string(maxGuess) + "的整数：");

        input = ReadLine();
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
            Println("是否开始新一轮的游戏？（是/否）");
            choice = ReadLine();
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
            choice = ReadLine();
            if (choice == "y" || choice == "Y" || choice == "是")
            {
                SetDifficulty();
            }
            else
            {
                ResetDifficulty();
            }
            gameStatus = GAME_CONTINUE;
        }
    }
}
