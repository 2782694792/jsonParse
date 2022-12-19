#include "Json.h"
// #include "10Parser.h"

int main(int argc, char const *argv[])
{
    // // 构造
    // Json j10 = 50;
    // decltype(j10) j100 = 50;
    // Json j11 = 50.00;
    // Json j12 = true;
    // Json j13 = "falseAndtrue";
    // // std::cout << j13 << std::endl;

    // // 重构
    // int i1 = j10;
    // double i2 = j11;
    // // 列表添加
    // Json j2;
    // // j2[0] = true;
    // // j2[1] = 123;
    // // 添加 json 元素
    // j2.append(j13);
    // j2.append(12.21);

    // double i4 = j2[1];
    // std::string i5 = j2[0];

    // // 明确 json 成员类型
    // Json j21(Json::ArrayValue);
    // // j21[0] = 123;
    // // j21[1] = "456";
    // // 获取 json 成员值
    // // j21.copy(j2);

    // // 容量或者大小
    // j2.size();

    // // 序列化
    // std::string str21 = j2.serial();

    // std::string j212 = j2[0];

    // Json j31(Json::ObjectValue);
    // j31["name"] = "bin";
    // j31["age"] = 23;
    // j31["gende"] = "man";

    // j31.remove(0); // fail
    // j31.remove("gende");

    // std::cout << std::boolalpha << j31.hasInObject("name") << std::endl;
    // std::cout << j31.getInObject("name") << std::endl;

    Json js;
    const std::string &str = "{\"name\":[123,\"asd\",NULL,True,false,12.34]}";
    // const std::string &str = "[123,\"asd\",NULL,True,false,12.34]";

    std::ifstream in("E:\\C++\\VScode-workspace\\source\\2212\\test.json");
    js.parse(in);

    // js.parse(str);
    std::cout << js["cmake_variants"][1]["variables"][1][0] << std::endl;

    return 0;
}