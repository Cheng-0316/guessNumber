#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>

#include "generalFunctions.hpp"

bool FileExists(std::string fileName)
{
    if (std::filesystem::exists(fileName))
    {
        std::cout << "文件" << fileName << "已存在，跳过创建。" << std::endl;
        return true;
    }
    std::cout << "文件" << fileName << "不存在，正在创建新文件。" << std::endl;
    return false;
}

bool FileCreat(std::string fileName)
{
    if (!FileExists(fileName))
    {
        std::ofstream file(fileName);
        if (file)
        {
            std::cout << "文件" << fileName << "创建成功！" << std::endl;
            file.close();
            return true;
        }
        else
        {
            std::cerr << "无法创建文件" << fileName << "！" << std::endl;
            return false;
        }
    }
    return true;
}

void Print(std::string message)
{
    std::cout << message << std::flush;
}

std::string GetInput()
{
    std::string input;
    std::cin >> input;
    return input;
}