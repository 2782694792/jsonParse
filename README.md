# jsonParse
Json 文件解析 —— 浅拷贝

## 1、文件解析
json::parse -》parser::load -》 parser::parse
## 2、Value
null -> int\double -> bool -> string -> array(list) -> object(map)
## 3、添加
operator[] -> append(list、append)
## 4、遍历、序列化
iterator(Json) -> iterator_begin(Json) -> iterator_end(Json)
serial() -> Value 判断

# 补充
1. number 数据缺少自然常数的成分，如： 1.23e-10
2. 浅拷贝进行的解析存在的利弊
3. 深拷贝解析的实现
4. 缺少格式化
