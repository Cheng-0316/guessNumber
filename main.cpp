/*********************************************
 * 项目说明：
 * 设计人：程创客
 * 创建时间：2025年9月
 * 内容：一个猜数字游戏
 * 注意：本项目需要使用C++17及以上版本编译运行
 *********************************************/

#include <iostream>
#include <filesystem>
#include <fstream>
#include <thread>
#include <string>
#include <random>

#include "nlohmann/json.hpp"
#include "guessNumber.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::flush;

using std::stoi;
using std::string;


guessNumber myGame;
bool saveAvailable;
nlohmann::ordered_json saveData_json;
guessNumberData saveData;
//使用该相对路径，在编译时会将文件放在编译器的当前路径下，可能会导致无法找到文件或文件被放在不方便的位置
std::string saveDataPath = "saveData.json";
// std::string saveDataPath = "D:/Programs/C++/guessNumber VS Code/saveData.json";


int main()
{
//    for (int i = 0;i < 1;i++)
//    {
//        cout << "-" << flush;
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//        cout << "\b" << flush;
//        cout << "\\" << flush;
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//        cout << "\b" << flush;
//        cout << "|" << flush;
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//        cout << "\b" << flush;
//        cout << "/" << flush;
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//        cout << "\b" << flush;
//    }
    // cout << "该项目使用了 nlohmann 的 json 项目" << endl;
    // cout << "链接：https://github.com/nlohmann/json" << endl;
    // cout << "版本：v";
    // cout << NLOHMANN_JSON_VERSION_MAJOR << ".";
    // cout << NLOHMANN_JSON_VERSION_MINOR << ".";
    // cout << NLOHMANN_JSON_VERSION_PATCH << endl;
    // cout << "协议：MIT" << endl;

    if (std::filesystem::exists(saveDataPath))
    {
    	// 存档文件存在，读取存档
    	cout << "加载存档中……" << endl;
        std::ifstream saveDataFile(saveDataPath);
        // 无法打开存档就取消读档，且游戏结束后不会保存
        if (saveDataFile.is_open())
        {
            nlohmann::ordered_json parsed_json;
            try
            {
                parsed_json = nlohmann::ordered_json::parse(saveDataFile);
                // 如果存档格式错误，抛出异常后，以下段落将会被跳过
                
                /*
                // ===== 版本检查与兼容处理 =====
                int sv_major = 0, sv_minor = 0, sv_patch = 0;
                if (parsed_json.contains("version") && parsed_json["version"].is_object())
                {
                    auto &v = parsed_json["version"];
                    if (v.contains("major") && v["major"].is_number()) sv_major = v["major"].get<int>();
                    if (v.contains("minor") && v["minor"].is_number()) sv_minor = v["minor"].get<int>();
                    if (v.contains("patch") && v["patch"].is_number()) sv_patch = v["patch"].get<int>();
                }

                // 完全相同版本
                if (sv_major == CHENG_GUESS_NUMBER_VERSION_MAJOR &&
                    sv_minor == CHENG_GUESS_NUMBER_VERSION_MINOR &&
                    sv_patch == CHENG_GUESS_NUMBER_VERSION_PATCH)
                {
                    // nothing special
                }
                // 存档主版本较旧：不可兼容 -> 备份旧存档并初始化新存档（不加载旧数据）
                else if (sv_major < CHENG_GUESS_NUMBER_VERSION_MAJOR)
                {
                    std::filesystem::path orig(saveDataPath);
                    std::filesystem::path bak = orig;
                    bak += ".bak";
                    std::error_code ec;
                    std::filesystem::copy_file(orig, bak, std::filesystem::copy_options::overwrite_existing, ec);
                    if (!ec)
                        cout << "已备份旧存档到 " << bak << " 。" << endl;
                    else
                        cout << "备份旧存档失败: " << ec.message() << endl;

                    cout << "存档主版本与程序不兼容，已初始化新存档（不会加载旧数据）。" << endl;
                    parsed_json.clear();
                }
                // 存档主版本较新：程序版本无法完全识别 -> 只读取可识别字段且不覆盖原存档
                else if (sv_major > CHENG_GUESS_NUMBER_VERSION_MAJOR)
                {
                    cout << "警告：存档版本高于程序版本，程序将只读取可识别字段且不会覆盖原存档。" << endl;
                    saveAvailable = false; // 禁止覆盖原存档
                }
                // 相同主版本但次版本较旧：尝试迁移（当前策略为兼容读取已知字段）
                else if (sv_major == CHENG_GUESS_NUMBER_VERSION_MAJOR && sv_minor < CHENG_GUESS_NUMBER_VERSION_MINOR)
                {
                    cout << "存档次版本较旧，尝试迁移已知字段。" << endl;
                    // 这里可以添加字段迁移逻辑（目前使用默认值补全缺失字段）
                }
                */

                // 读取并验证财富值（存在且为数字时使用，否则使用默认值 0）
                if (parsed_json.contains("wealth") && parsed_json["wealth"].is_number())
                {
                    saveData.wealth = parsed_json["wealth"].get<decltype(saveData.wealth)>();
                }
                else
                {
                    cout << "存档中未找到有效的 wealth 字段，使用默认值：0。" << endl;
                }

                cout << "加载存档完成。" << endl;
            }
            catch (const std::exception &e)
            {
                // 遇到错误的格式取消读档，但游戏结束后会覆盖原内容
                cout << "存档格式错误，取消读档。\n注意，游戏结束后原内容将被覆盖。" << endl;
                std::cerr << e.what() << endl;
            }
            //关闭文件
            saveDataFile.close();
            saveAvailable = true;
        }
		else
        {
            std::cerr << "存档文件存在但似乎无法打开，游戏将从0开始，且不进行保存。" << std::endl;
            saveAvailable = false;
        }
	}
	else
    {
    	// 文件不存在就创建
        cout << "文件" << saveDataPath << "不存在，正在试图创建……" << endl;
        std::ofstream saveDataFile(saveDataPath);
        if (saveDataFile)
        {
            cout << "文件" << saveDataPath << "创建成功！" << endl;
            cout << "正在初始化存档……" << endl;
            saveData_json["version"]["major"] = CHENG_GUESS_NUMBER_VERSION_MAJOR;
            saveData_json["version"]["minor"] = CHENG_GUESS_NUMBER_VERSION_MINOR;
            saveData_json["version"]["patch"] = CHENG_GUESS_NUMBER_VERSION_PATCH;
            saveData_json["wealth"] = 0;
            saveDataFile << saveData_json.dump(4) << std::endl;
            cout << "存档初始化完成。" << endl;
            saveDataFile.close();
            saveAvailable = true;
        }
        else
        {
        	cout << "文件" << saveDataPath << "创建失败！" << endl;
            cout << "由于无法创建存档文件" << saveDataPath << "，游戏将从0开始且不会存档。" << endl;
            saveAvailable = false;
        }
	}
    
    cout <<"游戏版本：" << CHENG_GUESS_NUMBER_VERSION_MAJOR;
    cout <<"." << CHENG_GUESS_NUMBER_VERSION_MINOR;
    cout <<"." << CHENG_GUESS_NUMBER_VERSION_PATCH << endl;
    cout << "游戏开始！" << endl;
    
    cout << "您的财富：" << saveData.wealth << endl;
	
    myGame.saveData = saveData;
    myGame.Play();
    saveData = myGame.saveData;
    
    saveData_json["version"]["major"] = CHENG_GUESS_NUMBER_VERSION_MAJOR;
    saveData_json["version"]["minor"] = CHENG_GUESS_NUMBER_VERSION_MINOR;
    saveData_json["version"]["patch"] = CHENG_GUESS_NUMBER_VERSION_PATCH;
    saveData_json["wealth"] = saveData.wealth;

    if (saveAvailable == true)
    {
        cout << "正在保存存档……" << endl;
        std::ofstream saveDataFile(saveDataPath);
        saveDataFile << saveData_json.dump(4) << std::endl;
        saveDataFile.close();
        cout << "存档保存完成。" << endl;
    }
    else
    {
        cout << "基于读档时的错误，存档不进行保存。" << endl;
        cout << "如果您想保存游戏进度，请将以下输出复制到文件：saveDataPath 中。" << endl;
        cout << saveData_json.dump(4) << std::endl;
        cout << "输出到此为止。" << endl;
    }
    
    cout << "按下回车键推退出。";
    cin.ignore();
    cin.get();
    return 0;
}
