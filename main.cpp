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
#include <string>

#include "nlohmann/json.hpp"
#include "guessNumber.hpp"

using std::cin;
using std::getline;
using std::cout;
using std::endl;
using std::flush;

using std::stoi;
using std::string;


guessNumber myGame;
bool saveAvailable;
nlohmann::ordered_json saveData_json;
guessNumberData saveData;
//使用相对路径，在编译时会将文件放在编译器的路径下，可能会导致无法找到文件或文件被放在不方便的位置
std::string saveDataPath = "saveData.json";
std::string reasonOfNoSave = "";

int main()
{
    //未来添加到 “关于当中”
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
                saveAvailable = true;
            }
            catch (const std::exception &e)
            {
                // 遇到错误的格式取消读档，但游戏结束后会覆盖原内容
                cout << "存档格式错误，取消读档。" << endl;
                std::cerr << e.what() << endl;
                reasonOfNoSave = "由于原存档文件格式错误，为不覆盖原存档文件";
                saveAvailable = false;
            }
            //关闭文件
            saveDataFile.close();
        }
		else
        {
            std::cerr << "程序找到了存档文件，但无法将其打开，因此游戏将不保存进度，且从0开始。" << endl;
            reasonOfNoSave = "虽然存档文件存在，但无法打开存档文件";
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
            reasonOfNoSave = "因为无法创建存档文件";
            saveAvailable = false;
        }
	}
    
    cout <<"游戏版本：" << CHENG_GUESS_NUMBER_VERSION_MAJOR;
    cout <<"." << CHENG_GUESS_NUMBER_VERSION_MINOR;
    cout <<"." << CHENG_GUESS_NUMBER_VERSION_PATCH << endl;
    cout << "游戏开始！" << endl;
    
    cout << "您的财富：" << saveData.wealth << endl;
	
    myGame.SetSaveData(saveData);
    myGame.Play();
    saveData = myGame.GetSaveData();
    
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
        cout << reasonOfNoSave  << "，所以不存档。" << endl;
        // 不执行文件备份
        cout << "如果您想保存游戏进度，请将以下输出复制到文件 " << saveDataPath << " 中。" << endl;
        cout << saveData_json.dump(4) << std::endl;
    }
    
    // cout << "欢迎" << endl;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清空输入缓冲区，防止之前的输入干扰后续输入
    cout << "按下回车键退出。";
    cin.get();
    return 0;
}
