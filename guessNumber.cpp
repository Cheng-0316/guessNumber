#include <iostream>
#include <random>
#include <string>

#include "guessNumber.h"

void guessNumber::SpecialInputJudgement(std::string input)
{
    if (input == "exit" || input == "quit")
    {
        gameStatus = WHOLE_GAME_END_NORMALLY;
    }
    if (input == "res" || input == "restart")
    {
        gameStatus = GAME_RESTART;
    }
}

int guessNumber::InputStringtoInt(std::string input)
{
    int number = 0;
    try
    {
        number = std::stoi(input);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::cout << "输入不可识别，" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        gameStatus = INPUT_UNAVAILABLE;
    }
    return number;
}

void guessNumber::ChooseDifficulty()
{
    std::string input;
    do
    {   
        gameStatus = GAME_CONTINUE;
        std::cout << "请选择难度:" << std::endl;
        std::cout << "1：简单（1-" << EASY_MAX_NUMBER << "，无猜测次数限制）" << std::endl;
        std::cout << "2：中等（1-" << NORMAL_MAX_NUMBER << "，限制猜测次数" << NORMAL_LIMITED_TIMES << "）" << std::endl;
        std::cout << "3：困难（1-" << HARD_MAX_NUMBER << "，限制猜测次数" << HARD_LIMITED_TIMES << "）" << std::endl;
        std::cout << "其他数字：自定义" << std::endl;
        std::cout << "选择难度：" << std::flush;
        std::cin >> input;
        difficulty = InputStringtoInt(input);
    } while (gameStatus != GAME_CONTINUE);
}

void guessNumber::InbuiltDifficultyFilling()
{
    switch (difficulty)
    {
    case 1:
        recoveryMax = EASY_MAX_NUMBER;
        recoveryGuessLimit = EASY_LIMITED_TIMES;
        break;
    case 2:
        recoveryMax = NORMAL_MAX_NUMBER;
        recoveryGuessLimit = NORMAL_LIMITED_TIMES;
        break;
    case 3:
        recoveryMax = HARD_MAX_NUMBER;
        recoveryGuessLimit = HARD_LIMITED_TIMES;
        break;
    default:
        break;
    }
}

void guessNumber::SetUserDefinedMaxRandom()
{
    std::string input;
    do
    {
        gameStatus = GAME_CONTINUE;
        std::cout << "请输入自定义最大猜测值：" << std::flush;
        std::cin >> input;
        recoveryMax = InputStringtoInt(input);
        if (recoveryMax < 2)
        {
            std::cout << "请确保输入值大于1，" << std::endl;
            continue;
        }
    } while (gameStatus != GAME_CONTINUE);
}

void guessNumber::SetUserDefinedMaxGuessLimit()
{
    std::string input;
    do
    {
        gameStatus = GAME_CONTINUE;
        std::cout << "（输入负数代表无限制）" << std::endl;
        std::cout << "请输入自定义最大猜测次数：" << std::flush;
        std::cin >> input;
        recoveryGuessLimit = InputStringtoInt(input);
    } while (gameStatus != GAME_CONTINUE);
}

void guessNumber::ResetDifficulty()
{
    minGuessLimit = 0;
    maxGuessLimit = recoveryMax;
    limitedGuessTimes = recoveryGuessLimit;
}

void guessNumber::SetDifficulty()
{
    std::string input;
    ChooseDifficulty();
    if (difficulty < 1 || difficulty > 3)
    {
        SetUserDefinedMaxRandom();
        SetUserDefinedMaxGuessLimit();
    }
    InbuiltDifficultyFilling();
    recoveryMax++;
    //此处的重置难度是为了自动填充
    ResetDifficulty();
}

void guessNumber::GenerateRandom()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    //减一确保生成的随机数在最大值以内，防止其等于最大值，导致用户无法猜到
    std::uniform_int_distribution<> dis(1, maxGuessLimit - 1);
    randomNumber = dis(gen);
    // std::cout << "（调试信息：随机数为 " << randomNumber << "）" << std::endl;
}

bool guessNumber::RangeJudgement(int inputNumber)
{
    //判断用户输入的值是否在范围内
    if (inputNumber <= minGuessLimit || inputNumber >= maxGuessLimit)
    {
        std::cout << "输入的数字不属于限制范围，" << std::endl;
        return false;
    }
    return true;
}

bool guessNumber::SizeJudgement()
{
    if (guessNumber == randomNumber)
    {
        return true;
    }
    if (guessNumber < randomNumber)
    {
        std::cout << "太小了！" << std::endl;
        minGuessLimit = guessNumber;
        gameStatus = GAME_CONTINUE;
    }
    if (guessNumber > randomNumber)
    {
        std::cout << "太大了！" << std::endl;
        maxGuessLimit = guessNumber;
        gameStatus = GAME_CONTINUE;
    }
    return false;
}

void guessNumber::DealPlayerGuess()
{
    if (RangeJudgement(guessNumber))
    {
        if (limitedGuessTimes > 0)
        {
            limitedGuessTimes--;
        }
        if (SizeJudgement())
        {
            std::cout << "恭喜你，猜对了！" << std::endl;
            WealthIncrease();
            gameStatus = SINGLE_GAME_END_NORMALLY;
            return;
        }
        if (limitedGuessTimes == 0 || minGuessLimit + 2 == maxGuessLimit)
        {
            std::cout << "你已经没有机会了，正确答案是：" << randomNumber << std::endl;
            gameStatus = SINGLE_GAME_END_NORMALLY;
        }
    }
}

void guessNumber::WealthIncrease()
{
    wealth += 10;
    if (limitedGuessTimes > 0)
    {
        wealth += limitedGuessTimes * 2;
    }
}

void guessNumber::SinglePlay()
{
    GenerateRandom();
    std::string input;
    do
    {
        gameStatus = GAME_CONTINUE;
        std::cout << "（输入exit或quit退出程序，输入restart或res重新开始游戏）" << std::endl;
        if (limitedGuessTimes < 0)
        {
            std::cout << "无限制猜测次数，" << std::flush;
        }
        else
        {
            std::cout << "还剩" << limitedGuessTimes << "猜测次数，" << std::flush;
        }
        std::cout << "输入一个从" << minGuessLimit << "到" << maxGuessLimit << "之内的整数：" << std::flush;
        std::cin >> input;
        SpecialInputJudgement(input);
        if (gameStatus == WHOLE_GAME_END_NORMALLY || gameStatus == GAME_RESTART)
        {
            break;
        }
        guessNumber = InputStringtoInt(input);
        if (gameStatus == INPUT_UNAVAILABLE)
        {
            continue;
        }
        DealPlayerGuess();
        if (gameStatus == INPUT_UNAVAILABLE)
        {
            continue;
        }
    } while (gameStatus != SINGLE_GAME_END_NORMALLY);
}

void guessNumber::SingleEndDealing()
{
    char choice;
    switch (gameStatus)
    {
    case SINGLE_GAME_END_NORMALLY:
        std::cout << "是否继续游戏？(y/n)" << std::endl;
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y')
        {
            gameStatus = GAME_RESTART;
        }
        else
        {
            gameStatus = WHOLE_GAME_END_NORMALLY;
        }
        break;
    /*
    case GAME_RESTART:
        break;
    case GAME_CONTINUE:
        break;
    case WHOLE_GAME_END_NORMALLY:
        break;
    */
    default:
        break;
    }
}

void guessNumber::ComputerGuessDemo()
{
    std::cout << "电脑二分法搜索演示开始。" << std::endl;
    difficulty = 1;
    InbuiltDifficultyFilling();
    ResetDifficulty();
    GenerateRandom();
    do
    {
        std::cout << "当前范围：(" << minGuessLimit << "," << maxGuessLimit << ")" << std::endl;
        guessNumber = (minGuessLimit + maxGuessLimit) / 2;
        std::cout << "电脑二分搜索：" << std::endl;
        std::cout << "(" << minGuessLimit << " + " << maxGuessLimit << ") ÷ 2 = " << guessNumber << std::endl;
        SizeJudgement();
    } while (guessNumber != randomNumber);
    std::cout << "电脑二分法搜索演示完毕，随机数为：" << guessNumber << "。" << std::endl;
}

void guessNumber::RepeatedlyPlay()
{
    char choice;
    std::cout << "游戏开始！版本：" << GAME_VERSION << std::endl;
    SetDifficulty();
    while (gameStatus != WHOLE_GAME_END_NORMALLY)
    {
        std::cout << "是否观看演示？(y/n)" << std::endl;
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y')
        {
            ComputerGuessDemo();
            std::cout << "按回车健结束……" << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }
        std::cout << "你的金钱目前为：" << wealth << std::endl;
        SinglePlay();
        SingleEndDealing();
        if (gameStatus == -1)
        {
            std::cout << "游戏错误地结束。" << std::endl;
            std::cout << "按回车健退出……" << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }
        if (gameStatus == GAME_RESTART)
        {
            std::cout << "游戏重新开始！" << std::endl;
            std::cout << "是否重新选择难度？(y/n)" << std::endl;
            std::cin >> choice;
            if (choice == 'y' || choice == 'Y')
            {
                SetDifficulty();
            }
            else
            {
                ResetDifficulty();
            }
        }
    }
    std::cout << "感谢参与游戏！" << std::endl;
    std::cout << "按回车健退出……" << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}