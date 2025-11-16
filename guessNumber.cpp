#include <filesystem>
#include <iostream>
#include <fstream>
#include <random>
#include <string>

#include "guessNumber.hpp"
#include "nlohmann/json.hpp"

bool guessNumber_config::ConfigFileExists()
{
    //版本限制
    //直接返回，表示该版本无配置
    return true;
    if (!std::filesystem::exists("config.json"))
    {
        // 文件不存在就创建
        Println("文件config.json不存在，正在创建中……");
        if (FileCreate("config.json"))
        {
            Println("文件config.json创建成功！");
            Println("正在写入默认配置……");
            nlohmann::json defaultConfig;
            defaultConfig["version"] = GAME_VERSION;
            std::ofstream configFile("config.json");
            configFile << defaultConfig.dump(4) << std::endl;
            Println("默认配置写入完成。");
        }
        else
        {
            //没有且无法创建，返回错误
            Println("无法创建文件config.json！");
            return false;
        }
    }
    //存在或创建成功，返回正常
    return true;
}

void guessNumber_config::LoadConfigFile()
{
    if (!ConfigFileExists())
    {
        // 配置文件无法创建且不存在
        // 退出游戏时不进行保存
        Println("游戏将使用默认配置，并不进行保存。");
        configSaving = false;
        return;
    }
    // 目前无可读取配置
    Println("版本限制，配置不进行加载。");
    configSaving = false;
    return;
    // 读取配置
    Println("加载游戏配置中……");
    ;
    Println("加载配置完成。");
}

void guessNumber_config::SaveConfig()
{
    nlohmann::ordered_json config;
    config["version"] = GAME_VERSION;
    std::ofstream configFile("config.json");
    configFile << config.dump(4) << std::endl;
}

bool guessNumber_archive::ArchiveFileExists()
{
    if (!std::filesystem::exists("archiveFile.json"))
    {
        // 文件不存在就创建
        Println("文件archiveFile.json不存在，正在创建中……");
        if (FileCreate("archiveFile.json"))
        {
            Println("文件archiveFile.json创建成功！");
            Println("正在初始化存档中……");
            nlohmann::ordered_json archive;
            archive["version"] = GAME_VERSION;
            archive["wealth"] = 0;
            std::ofstream archiveFile("archiveFile.json");
            archiveFile << archive.dump(4) << std::endl;
            Println("存档初始化完成。");
        }
        else
        {
            Println("无法创建文件archiveFile.json！");
            return false;
        }
    }
    return true;
}

void guessNumber_archive::LoadArchiveFile()
{
    // 如果存档文件不存在且无法创建
    if (!ArchiveFileExists())
    {
        // 退出游戏时不进行保存
        Println("游戏将使用空存档，且不进行保存。");
        archiveSaving = false;
        return;
    }
    // 读取存档
    Println("加载存档中……");
    std::ifstream archiveFile("archiveFile.json");
    // 无法打开存档就取消读档，且游戏结束后不会保存
    if (!archiveFile.is_open())
    {
        std::cerr << "文件archiveFile.json未找到或无法打开！" << std::endl;
        archiveSaving = false;
        return;
    }
    nlohmann::ordered_json archive;
    try
    {
        archive = nlohmann::ordered_json::parse(archiveFile);
    }
    catch (const std::exception &e)
    {
        // 遇到错误的格式也取消读档，但游戏结束后会保存
        Println("存档格式错误。");
        std::cerr << e.what() << std::endl;
        Println("取消读档。");
        archiveSaving = true;
        return;
    }
    archiveFile.close();
    // 读取财富值
    // 还需判断值是否存在！！！
    wealth = archive["wealth"];
    Println("加载存档完成。");
    archiveSaving = true;
}

void guessNumber_archive::SaveArchive()
{
    nlohmann::ordered_json archive;
    archive["version"] = GAME_VERSION;
    archive["wealth"] = wealth;
    std::ofstream archiveFile("archiveFile.json");
    archiveFile << archive.dump(4) << std::endl;
}


void guessNumber::SaveAll()
{
    if (ganeConfig.configSaving == true)
    {
        Println("保存配置中……");
        ganeConfig.SaveConfig();
        Println("配置保存完毕。");
    }
    else
    {
        Println("版本限制，配置不进行保存。");
    }
    if (gameArchive.archiveSaving == true)
    {
        Println("保存存档中……");
        gameArchive.SaveArchive();
        Println("存档保存完毕。");
    }
    else
    {
        Println("存档无法保存。");
    }
}

void guessNumber::SpecialInput(std::string input)
{
    if (input == "exit" || input == "quit")
    {
        gameStatus = WHOLE_GAME_END_NORMALLY;
    }
    if (input == "res" || input == "restart")
    {
        gameStatus = GAME_RESTART;
    }

    // 测试代码
    if (input == "wealth_increase")
    {
        gameArchive.wealth += 100;
        gameStatus = 3;
        Println("你的财富：" + std::to_string(gameArchive.wealth));
    }
}

int guessNumber::StringtoInt(std::string input)
{
    int number = 0;
    try
    {
        number = std::stoi(input);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        Println("输入不可识别，");
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
        recoveryDifficulty = StringtoInt(input);
    } while (gameStatus != GAME_CONTINUE);
}

void guessNumber::InbuiltDifficultyFilling()
{
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

void guessNumber::SetRandomLimit_UserDefined()
{
    std::string input;
    do
    {
        gameStatus = GAME_CONTINUE;
        std::cout << "请输入自定义最大猜测值：" << std::flush;
        std::cin >> input;
        recoveryMax = StringtoInt(input);
        if (recoveryMax < 2)
        {
            std::cout << "请确保输入值大于1，" << std::endl;
            continue;
        }
    } while (gameStatus != GAME_CONTINUE);
}

void guessNumber::SetLimitedTimes_UserDefined()
{
    std::string input;
    do
    {
        gameStatus = GAME_CONTINUE;
        std::cout << "（输入负数代表无限制）" << std::endl;
        std::cout << "请输入自定义最大猜测次数：" << std::flush;
        std::cin >> input;
        recoveryLimitedTimes = StringtoInt(input);
    } while (gameStatus != GAME_CONTINUE);
}

void guessNumber::SetDifficulty()
{
    std::string input;
    ChooseDifficulty();
    //所有值为先保存至恢复数据中，再填充到游戏数据中
    //如果用户自定义难度
    if (recoveryDifficulty < 1 || recoveryDifficulty > 3)
    {
        //让用户自己输入随机数范围和限制猜测次数
        SetRandomLimit_UserDefined();
        SetLimitedTimes_UserDefined();
    }
    else
    {
        //否则自动填充难度相关恢复数据
        InbuiltDifficultyFilling();
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

void guessNumber::GenerateRandom()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    // 减一确保生成的随机数在最大值以内，防止其等于最大值，导致用户无法猜到
    std::uniform_int_distribution<> dis(1, maxGuess - 1);
    random = dis(gen);
    // std::cout << "（调试信息：随机数为 " << randomNumber << "）" << std::endl;
}

bool guessNumber::OutOfRange(int inputNumber)
{
    // 判断用户输入的值是否在范围内
    if (inputNumber <= minGuess || inputNumber >= maxGuess)
    {
        std::cout << "输入的数字不属于限制范围，" << std::endl;
        return true;
    }
    return false;
}

bool guessNumber::SizeJudgement()
{
    if (guess == random)
    {
        return true;
    }
    if (guess < random)
    {
        std::cout << "太小了！" << std::endl;
        minGuess = guess;
        gameStatus = GAME_CONTINUE;
    }
    if (guess > random)
    {
        std::cout << "太大了！" << std::endl;
        maxGuess = guess;
        gameStatus = GAME_CONTINUE;
    }
    return false;
}

void guessNumber::DealPlayerGuess()
{
    //如果输入的数字在范围内
    if (!OutOfRange(guess))
    {
        if (limitedTimes > 0)
        {
            limitedTimes--;
        }
        if (SizeJudgement())
        {
            std::cout << "恭喜你，猜对了！" << std::endl;
            WealthIncrease();
            gameStatus = SINGLE_GAME_END_NORMALLY;
            return;
        }
        if (limitedTimes == 0 || minGuess + 2 == maxGuess)
        {
            std::cout << "你已经没有机会了，正确答案是：" << random << std::endl;
            gameStatus = SINGLE_GAME_END_NORMALLY;
        }
    }
}

void guessNumber::WealthIncrease()
{
    gameArchive.wealth += 10;
    if (limitedTimes > 0)
    {
        gameArchive.wealth += limitedTimes * 2;
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
        std::cout << "当前范围：(" << minGuess << "," << maxGuess << ")" << std::endl;
        guess = (minGuess + maxGuess) / 2;
        std::cout << "电脑二分搜索：" << std::endl;
        std::cout << "(" << minGuess << " + " << maxGuess << ") ÷ 2 = " << guess << std::endl;
        SizeJudgement();
    } while (guess != random);
    std::cout << "电脑二分法搜索演示完毕，随机数为：" << guess << std::endl;
}

void guessNumber::PrintClue()
{
    Println("（输入exit或quit退出程序，输入restart或res重新开始游戏）");
    if (limitedTimes < 0)
    {
        Println("无限制猜测次数。");
    }
    else
    {
        Println("还剩" + std::to_string(limitedTimes) + "猜测次数。");
    }
    Print("输入一个大于" + std::to_string(minGuess));
    Print("且小于" + std::to_string(maxGuess) + "的整数：");
}

void guessNumber::SinglePlay()
{
    std::cout << "你的财富：" << gameArchive.wealth << std::endl;
    GenerateRandom();
    std::string input;
    do
    {
        gameStatus = GAME_CONTINUE;
        PrintClue();
        std::cin >> input;
        SpecialInput(input);
        if (gameStatus == WHOLE_GAME_END_NORMALLY || gameStatus == GAME_RESTART)
        {
            break;
        }

        // 测试代码
        if (gameStatus == 3)
        {
            continue;
        }

        guess = StringtoInt(input);
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
    SingleEnd();
}

void guessNumber::SingleEnd()
{
    char choice;
    //处理单次游戏正常结束
    if (gameStatus == SINGLE_GAME_END_NORMALLY)
    {
        Println("是否继续游戏？(y/n)");
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y')
        {
            gameStatus = GAME_RESTART;
        }
        else
        {
            gameStatus = WHOLE_GAME_END_NORMALLY;
        }
    }
}

void guessNumber::RepeatedlyPlay()
{
    Print("游戏版本：");
    Println(GAME_VERSION);
    ganeConfig.LoadConfigFile();
    gameArchive.LoadArchiveFile();
    Println("游戏开始！\n你的财富：" + std::to_string(gameArchive.wealth));
    SetDifficulty();
    char choice;
    while (gameStatus != WHOLE_GAME_END_NORMALLY)
    {
        Println("是否观看演示？(y/n)");
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y')
        {
            ComputerGuessDemo();
            ResetDifficulty();
        }
        SinglePlay();
        /*
        if (gameStatus == -1)
        {
            std::cout << "游戏错误地结束。" << std::endl;
            std::cout << "按回车健退出……" << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }
        */
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
    SaveAll();
    Println("感谢参与游戏！\n回车健退出……");
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

bool FileCreate(std::string fileName)
{
    std::ofstream file(fileName);
    if (file)
    {
        file.close();
        return true;
    }
    return false;
}

void Print(std::string message)
{
    std::cout << message << std::flush;
}

void Println(std::string message)
{
    std::cout << message << std::endl;
}
