/****************************************************************
 * 项目说明：
 * 设计人：程创客
 * 开始时间：2025年9月
 * 内容：
****************************************************************/

#include <iostream>
#include <random>
#include <string>

#define PROGRAM_END 0
#define GAME_END 1
#define GAME_CONTINUE 2
#define GAME_RESTART 3

#define EASY_MAX_NUMBER 64
#define NORMAL_MAX_NUMBER 128
#define HARD_MAX_NUMBER 1024

#define NORMAL_GUESS_TIMES 7
#define HARD_GUESS_TIMES 8

using std::cin;
using std::cout;
using std::endl;

using std::string;
using std::stoi;

unsigned int ChooseMaxNumber();
unsigned int GenerateRandom(unsigned int maxNumber);
int DealGuessNumber(unsigned int randomNumber, unsigned int guessNumber, unsigned int &minNumber, unsigned int &maxNumber);
int GuessNumber(unsigned int maxNumber, unsigned int maxGuessTimes);
int DealGameReturn(int gameReturn);

int main()
{
    unsigned int maxNumber, maxGuessTimes;
    int gameReturn, dealedReturn;
    char choice;
    cout << "游戏开始！" << endl;
    maxNumber = ChooseMaxNumber();
    while(true)
    {
        gameReturn = GuessNumber(maxNumber);
        if(gameReturn == PROGRAM_END)
        {
            break;
        }
        dealedReturn = DealGameReturn(gameReturn);
        if(dealedReturn == -1)
        {
            cout << "程序错误地结束。" << endl;
            cout << "按回车健退出……" << endl;
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return -1;
        }
        if(dealedReturn == PROGRAM_END) //用户选择退出
        {
            break;
        }
        if(dealedReturn == GAME_RESTART)
        {
            cout << "游戏重新开始！" << endl;
            cout << "是否重新选择难度？(y/n)" << endl;
            cin >> choice;
            if(choice == 'y' || choice == 'Y')
            {
                maxNumber = ChooseMaxNumber();
            }
        }
    }
    cout << "感谢参与游戏！" << endl;
    cout << "按回车健退出……" << endl;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.get();
    return 0;
}

unsigned int ChooseMaxNumber()
{
    int difficulty = 0;
    unsigned int maxNumber;
    while (true)
    {
        cout << "请选择难度:" << endl;
        cout << "1.简单（1-" << EASY_MAX_NUMBER << "，无猜测次数限制）" << endl;
        cout << "2.中等（1-" << NORMAL_MAX_NUMBER << "，限制猜测次数" << NORMAL_GUESS_TIMES << "）" << endl;
        cout << "3.困难（1-" << HARD_MAX_NUMBER << "，限制猜测次数" << HARD_GUESS_TIMES << "）" << endl;
        cout << "4.自定义" << endl;
        cin >> difficulty;
        if (difficulty >= 1 && difficulty <= 3)
        {
            break;
        }
        else if(difficulty == 4)
        {
            cout << "请输入最大猜测值：" << endl;
            //需加入输入侦测，确保输入为数字且大于4
            cin >> maxNumber;
            break;
        }
        else
        {
            cout << "无效的选择，请重新输入。" << endl;
        }
    }
    switch(difficulty)
    {
        case 1: maxNumber = EASY_MAX_NUMBER; break;
        case 2: maxNumber = NORMAL_MAX_NUMBER; break;
        case 3: maxNumber = HARD_MAX_NUMBER; break;
        default: break;
    }
    return maxNumber;
}

unsigned int GenerateRandom(unsigned int maxNumber)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(2, maxNumber - 1);
    unsigned int randomNumber = dis(gen);
    return randomNumber;
}



//函数作用：处理猜测的数字并更改游戏的猜测范围
int DealGuessNumber(unsigned int randomNumber, unsigned int guessNumber, unsigned int &minNumber, unsigned int &maxNumber)
{
    if (guessNumber <= minNumber || guessNumber >= maxNumber)
    {
        cout << "输入的数字不在范围内，" << endl;
        return GAME_CONTINUE;
    }
    if (guessNumber < randomNumber)
    {
        cout << "太小了！" << endl;
        minNumber = guessNumber;
        return GAME_CONTINUE;
    }
    if (guessNumber > randomNumber)
    {
        cout << "太大了！" << endl;
        maxNumber = guessNumber;
        return GAME_CONTINUE;
    }
    if (minNumber + 2 == maxNumber)
    {
        cout << "你已经没有机会了，正确答案是：" << randomNumber << endl;
        return GAME_END;
    }
    if (guessNumber == randomNumber)
    {
        cout << "恭喜你，猜对了！" << endl;
        return GAME_END;
    }
    return -1;
}

//函数作用：猜一个数字（自动生成随机数）
int GuessNumber(unsigned int maxNumber, unsigned int maxGuessTimes)
{
    unsigned int randomNumber = GenerateRandom(maxNumber);
    unsigned int guessNumber, guessTime;
    unsigned int minNumber = 1;
    string input;
    while (true)
    {
        cout << "输入一个从" << minNumber << "到" << maxNumber << "之内的整数：" << endl;
        cout << "（输入exit或quit退出程序，输入restart或res重新开始游戏）" << endl;
        cin >> input;
        if (input == "exit" || input == "quit")
        {
            return PROGRAM_END;
        }
        if(input == "res" ||input == "restart")
        {
            return GAME_RESTART;
        }
        try
        {
            guessNumber = stoi(input);
        }
        catch (std::invalid_argument &e)
        {
            cout << "输入不可识别，" << endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        //判断输入的大小（仅数字）
        int inputConclusion = DealGuessNumber(randomNumber, guessNumber, minNumber, maxNumber);
        //如果返回值为结束信号，中断循环
        if(inputConclusion == GAME_END)
        {
            break;
        }
        // switch(inputConclusion)
        // {
        //     case CONTINUE: break;
        //     case GAME_END: break;
        //     default: break;
        // }
    }
    return GAME_END;
}

int DealGameReturn(int gameReturn)
{
    char choice;
    switch(gameReturn)
    {
        case GAME_END:
        cout << "是否继续游戏？(y/n)" << endl;
        cin >> choice;
        if(choice == 'y' || choice == 'Y')
        {
            return GAME_CONTINUE;
        }
        else
        {
            return PROGRAM_END;
        }
        break;
        case GAME_RESTART:
        return GAME_CONTINUE;
        break;
        case GAME_CONTINUE:
        return GAME_CONTINUE;
        break;
        default:
        return -1;
        break;
    }
    return -1;
}
