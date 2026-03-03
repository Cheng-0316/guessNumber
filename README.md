# 一个猜数字游戏

规则如下：

1.当只剩下最后一个数字，算作玩家失败。

## 该项目的使用方法

该项目使用 json.hpp 将游戏进度转换为 json 格式，游戏本体不依赖 json.hpp。

如果你自己实现游戏进度的保存，可以只引入 guessNumber.hpp 和 guessNumber.cpp 两个文件。
