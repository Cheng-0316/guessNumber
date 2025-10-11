/****************************************************************
 * 项目说明：
 * 设计人：程创客
 * 开始时间：2025年9月
 * 内容：
****************************************************************/

#include <iostream>
#include <random>
#include <string>

#define END 0
#define RESTART 1
#define CONTINUE 2

#define EASY_MAX_NUMBER 64
#define NORMAL_MAX_NUMBER 128
#define HARD_MAX_NUMBER 1024

using std::cin;
using std::cout;
using std::endl;

using std::string;
using std::stoi;

unsigned int ChooseMaxNumber();
unsigned int GenerateRandom(unsigned int maxNumber);
int GuessNumber(unsigned int maxNumber);
void DealGameReturn(int gameReturn);

int main()
{
    unsigned int maxNumber;
    int gameReturn = 0;
    cout << "游戏开始！" << endl;
    maxNumber = ChooseMaxNumber();
    cout << "1" << endl;
    while(true)
    {
        gameReturn = GuessNumber(maxNumber);
        DealGameReturn(gameReturn);
        if(gameReturn == -1) //用户选择退出
        {
            break;
        }
    }
    return 0;
}

unsigned int ChooseMaxNumber()
{
    int difficulty = 0;
    unsigned int maxNumber;
    while (true)
    {
        cout << "请选择难度:" << endl;
        cout << "1.简单（1-" << EASY_MAX_NUMBER << "）" << endl;
        cout << "2.中等（1-" << NORMAL_MAX_NUMBER << "）" << endl;
        cout << "3.困难（1-" << HARD_MAX_NUMBER << "）" << endl;
        cout << "4.自定义" << endl;
        cin >> difficulty;
        if (difficulty >= 1 && difficulty <= 3)
        {
            break;
        }
        else if(difficulty == 4)
        {
            cout << "请输入最大猜测值：" << endl;
            //需加入正确输入判别
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
    std::uniform_int_distribution<> dis(1, maxNumber);
    unsigned int randomNumber = dis(gen);
    return randomNumber;
}

int JudgePosition(unsigned int randomNumber, unsigned int guessNumber, unsigned int &minNumber, unsigned int &maxNumber)
{
    if (guessNumber <= minNumber || guessNumber >= maxNumber)
    {
        cout << "输入的数字不在范围内，" << endl;
        return CONTINUE;
    }
    if (guessNumber < randomNumber)
    {
        cout << "太小了！" << endl;
        minNumber = guessNumber;
        return CONTINUE;
    }
    if (guessNumber > randomNumber)
    {
        cout << "太大了！" << endl;
        maxNumber = guessNumber;
        return CONTINUE;
    }
    if (minNumber + 2 == maxNumber)
    {
        cout << "你已经没有机会了，正确答案是：" << randomNumber << endl;
        return END;
    }
    if (guessNumber == randomNumber)
    {
        cout << "恭喜你，猜对了！" << endl;
        return END;
    }
    return -1;
}

int GuessNumber(unsigned int maxNumber)
{
    unsigned int randomNumber = GenerateRandom(maxNumber);
    unsigned int guessNumber;
    unsigned int minNumber = 1;
    string input;
    while (true)
    {
        cout << "请输入一个从" << minNumber << "到" << maxNumber << "之内的整数：" << endl;
        cout << "（输入exit或quit退出游戏）" << endl;
        cout << "（输入restart或res重新开始游戏）" << endl;
        cin >> input;
        if (input == "exit" || input == "quit")
        {
            cout << "感谢参与游戏！" << endl;
            cout << "按回车健退出……" << endl;
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cin.get();
            return -1;
        }
        if(input == "res" ||input == "restart")
        {
            return RESTART;
        }
        try
        {
            guessNumber = stoi(input);
        }
        catch (std::invalid_argument &e)
        {
            cout << "检测输入不可识别，" << endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        //判断输入的大小（仅数字）
        int inputConclusion = JudgePosition(randomNumber, guessNumber, minNumber, maxNumber);
        //如果返回值为结束信号，中断循环
        if(inputConclusion == END)
        {
            break;
        }
        // switch(inputConclusion)
        // {
        //     case CONTINUE: break;
        //     case END: break;
        //     default: break;
        // }
    }
    return END;
}

void DealGameReturn(int gameReturn)
{
    char choice;
    unsigned maxNumber;
    switch(gameReturn)
    {
        case END:
        cout << "是否继续游戏？(y/n)" << endl;
        cin >> choice;
        if(choice == 'y' || choice == 'Y')
        {
            cout << "游戏重新开始！" << endl;
            cout << "是否重新选择难度？(y/n)" << endl;
            cin >> choice;
            if(choice == 'y' || choice == 'Y')
            {
                maxNumber = ChooseMaxNumber();
            }
        }
        else
        {
            cout << "感谢参与游戏！" << endl;
            cout << "按回车健退出……" << endl;
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cin.get();
        }
        break;
        case RESTART:
        cout << "游戏重新开始！" << endl;
        cout << "是否重新选择难度？(y/n)" << endl;
        cin >> choice;
        if(choice == 'y' || choice == 'Y')
        {
            maxNumber = ChooseMaxNumber();
        }
        break;
        case CONTINUE:
        break;
        default: break;
    }
}