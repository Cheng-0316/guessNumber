# 猜数字游戏 (Guess Number)

一个基于命令行的猜数字游戏，支持多种难度、自定义范围、猜测次数限制、财富系统和本地存档。

## 版本

v4.0.0

## 功能特性

- 三种预设难度（简单 / 中等 / 困难）及完全自定义模式
- 动态范围提示
- 财富系统：根据范围和剩余猜测次数计算奖励
- 本地 JSON 存档，自动保存和读取游戏进度
- 支持游戏内重开、难度重选、中途退出

## 依赖

- **编译器**：支持 C++17 或更高版本
- **第三方库**：[nlohmann/json](https://github.com/nlohmann/json)（采用 MIT 协议）
  - 需要将 `json.hpp` 放置在项目路径中

## 编译与运行

使用支持 C++17 的编译器（如 g++、clang++ 或 MSVC）：

```bash
g++ -std=c++17 main.cpp guessNumber.hpp guessNumber.cpp nlohmann/json.hpp -o main.exe