#include "Json.h"
#include "10Parser.h"

#include <sstream>
#include <stdlib.h>
#include <stdexcept>
#include <cmath>
#include <algorithm>

// static std::string DoubleToString(const double value)
// {
//     std::string s = std::to_string(value);
//     std::string::reverse_iterator  re;

//     for (re = s.rbegin();re != s.rend(); ++re)
//     {
//         if (*re == '0')
//         {
//             s.erase(*re);
//         }
//         break;
//     }

//     return s;
// }

Json::Json() : m_Type(NullValue)
{
}

Json::~Json()
{
}

Json::Json(TYPE type) : m_Type(type)
{
    switch (m_Type)
    {
    case NullValue:
        break;

    case IntValue:
        m_value.m_int = 0;
        break;

    case DoubleValue:
        m_value.m_double = 0.0;
        break;

    case BoolValue:
        m_value.m_bool = false;
        break;

    case StringValue:
        m_value.m_string = new std::string("");
        break;

    case ArrayValue:
        m_value.m_array = new std::list<Json>();
        break;

    case ObjectValue:
        m_value.m_object = new std::map<std::string, Json>();
        break;

    default:
        break;
    }
}

Json::Json(bool value) : m_Type(BoolValue)
{
    m_value.m_bool = value;
}

Json::Json(int value) : m_Type(IntValue)
{
    m_value.m_int = value;
}

Json::Json(double value) : m_Type(DoubleValue)
{
    m_value.m_double = value;
}

// Json::Json(char value[]) : m_Type(JSON_NUMBER)
// {
//     m_value.m_number = value;
// }

Json::Json(const char *value) : m_Type(StringValue)
{
    if (value == nullptr)
    {
        throw std::logic_error("char value is a nullptr!");
    }

    m_value.m_string = new std::string(value);
}

Json::Json(const std::string &value) : m_Type(StringValue)
{
    if (value == "")
    {
        throw std::logic_error("string value is a nullptr!");
    }

    m_value.m_string = new std::string(value);
}

Json::Json(const Json &otherJson)
{
    copy(otherJson);
}

void Json::copy(const Json &otherJson)
{
    m_value = otherJson.m_value;
    m_Type = otherJson.type();
}

void Json::clear()
{
    switch (m_Type)
    {
    case NullValue:
    case BoolValue:
    case IntValue:
    case DoubleValue:
        break;

    case StringValue:
        delete m_value.m_string;
        break;

    case ArrayValue:
        for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); it++)
        {
            it->clear();
        }
        delete m_value.m_array;
        break;

    case ObjectValue:
        for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); it++)
        {
            it->second.clear();
        }
        delete m_value.m_object;
        break;

    default:
        break;
    }
    m_Type = NullValue;
}

Json::operator bool()
{
    if (m_Type != BoolValue)
    {
        throw std::logic_error("type exception, not found bool value!");
    }
    return m_value.m_bool;
}

Json::operator int()
{
    if (m_Type != IntValue)
    {
        throw std::logic_error("type exception, not found numerical value!");
    }
    return m_value.m_int;
}

Json::operator double()
{
    if (m_Type != DoubleValue)
    {
        throw std::logic_error("type exception, not found numerical value!");
    }
    return m_value.m_double;
}

Json::operator std::string()
{
    if (m_Type != StringValue)
    {
        throw std::logic_error("type exception, not found string value!");
    }
    return *m_value.m_string;
}

/**
 * @brief 赋值基本类型
 * @param index 列表数组索引
 * @return 返回对应的列表索引对应值
 */
Json &Json::operator[](const int index)
{
    if (m_Type != ArrayValue)
    {
        m_Type = ArrayValue;
        m_value.m_array = new std::list<Json>();
    }
    if (index < 0)
    {
        throw std::logic_error("traversal exception, not found array index");
    }
    int len = m_value.m_array->size();
    if (index >= len)
    {
        // for (int i = len; len <= index; i++)
        // {
        //     m_value.m_array->push_back(Json());
        // }

        throw std::logic_error("out of range, not found the index in array!");
    }

    auto it = m_value.m_array->begin();
    for (int i = 0; i < index; i++)
    {
        it++;
    }

    return *it;
}

Json &Json::operator[](const char *key)
{
    std::string keyStr = key;
    return (*this)[keyStr];
}

Json &Json::operator[](const std::string key)
{
    if (m_Type != ObjectValue)
    {
        clear();
        m_Type = ObjectValue;
        m_value.m_object = new std::map<std::string, Json>();
    }
    return (*(m_value.m_object))[key];
}

void Json::append(const Json &otherJson)
{
    if (m_Type != ArrayValue)
    {
        m_Type = ArrayValue;
        m_value.m_array = new std::list<Json>();
        // throw std::logic_error("type exception, not found array data!");
    }

    m_value.m_array->push_back(otherJson);
}

void Json::remove(int index)
{
    if (m_Type != ArrayValue)
    {
        return;
    }
    if (index < 0)
    {
        throw std::logic_error("traversal exception, not found array index");
    }
    int len = m_value.m_array->size();
    if (index >= len)
    {
        throw std::logic_error("out of range, not found the index in array!");
    }

    auto it = m_value.m_array->begin();
    for (int i = 0; i < index; i++)
    {
        it++;
    }

    it->clear();
    m_value.m_array->erase(it);
}

void Json::remove(const char *key)
{
    remove(std::string(key));
}

void Json::remove(const std::string &key)
{
    if (m_Type != ObjectValue)
    {
        return;
    }

    auto it = m_value.m_object->find(key);
    if (it != m_value.m_object->end())
    {
        it->second.clear();
        m_value.m_object->erase(it);
    }
}

/**
 * @brief const 声明修饰 *this
 * @attention const fun 修饰返回值，fun const 修饰函数成员本身
 * @return 返回成员类型枚举值
 */
Json::TYPE Json::type() const
{
    return m_Type;
}

// template <typename T>
// T Json::value() const
// {

// }

Json &Json::operator=(const Json &otherJson)
{
    clear();
    copy(otherJson);
    return *this;
}

bool Json::operator==(const Json &otherJson)
{
    if (m_Type != otherJson.type())
    {
        return false;
    }
    switch (m_Type)
    {
    case BoolValue:
        return (m_value.m_bool == otherJson.m_value.m_bool);
    case IntValue:
        return (m_value.m_int == otherJson.m_value.m_int);
    case DoubleValue:
        return (m_value.m_double == otherJson.m_value.m_double);
    case StringValue:
        return *(m_value.m_string) == *(otherJson.m_value.m_string);
    case ArrayValue:
        return m_value.m_array == otherJson.m_value.m_array;
    case ObjectValue:
        return m_value.m_object == otherJson.m_value.m_object;
    case NullValue:
    default:
        return false;
        break;
    }
}

bool Json::operator!=(const Json &otherJson)
{
    return !(*this == otherJson);
}

bool Json::isNull() const
{
    return m_Type == NullValue;
}

bool Json::isBool() const
{
    return m_Type == BoolValue;
}

bool Json::isInt() const
{
    return m_Type == IntValue;
}

bool Json::isDouble() const
{
    return m_Type == DoubleValue;
}

bool Json::isString() const
{
    return m_Type == StringValue;
}

bool Json::isArray() const
{
    return m_Type == ArrayValue;
}

bool Json::isObject() const
{
    return m_Type == ObjectValue;
}

bool Json::asBool() const
{
    switch (type())
    {
    case BoolValue:
        return m_value.m_bool;
    case NullValue:
        return false;
    case IntValue:
        return m_value.m_int != 0;
    case DoubleValue:
    {
        // 0 或 nan 视为 false
        const auto value_classification = std::fpclassify(m_value.m_double);
        return value_classification != FP_ZERO && value_classification != FP_NAN;
    }
    default:
        break;
    }
    throw std::logic_error("failed to convert the bool type!");
}

int Json::asInt() const
{
    switch (type())
    {
    case IntValue:
        return m_value.m_int;
    case DoubleValue:
        return static_cast<int>(m_value.m_double);
    case NullValue:
        return 0;
    case BoolValue:
        return m_value.m_bool ? 1 : 0;
    default:
        break;
    }
    throw std::logic_error("failed to convert the double type!");
}

double Json::asDouble() const
{
    switch (type())
    {
    case IntValue:
        return static_cast<double>(m_value.m_int);
    case DoubleValue:
        return m_value.m_double;
    case NullValue:
        return 0.0;
    case BoolValue:
        return m_value.m_bool ? 1.0 : 0.0;
    default:
        break;
    }
    throw std::logic_error("failed to convert the double type!");
}

std::string Json::asString() const
{
    switch (type())
    {
    case NullValue:
        return "";

    case StringValue:
    {
        if (m_value.m_string == nullptr)
            return "";
        return *m_value.m_string;
    }
    case BoolValue:
        return m_value.m_bool ? "true" : "false";

    case IntValue:
        return std::to_string(m_value.m_int);

    case DoubleValue:
        return std::to_string(m_value.m_double);

    default:
        break;
    }
    throw std::logic_error("failed to convert the string type!");
}

bool Json::hasInArray(int index) const
{
    if (m_Type != ArrayValue)
    {
        return false;
    }
    int size = m_value.m_array->size();
    return (index >= 0) && (index < size);
}

bool Json::hasInObject(const char *key) const
{
    return hasInObject(std::string(key));
}

bool Json::hasInObject(const std::string &key) const
{
    if (m_Type != ObjectValue)
    {
        return false;
    }
    return (m_value.m_object)->find(key) != (m_value.m_object)->end();
}

Json Json::getInArray(int index) const
{
    if (!hasInArray(index))
    {
        return Json();
    }
    auto it = (m_value.m_array)->begin();
    for (int i = 0; i < index; i++)
    {
        it++;
    }
    return *it;
}

Json Json::getInObject(const char *key) const
{
    return getInObject(std::string(key));
}

Json Json::getInObject(const std::string &key) const
{
    if (!hasInObject(key))
    {
        return Json();
    }

    return (*m_value.m_object)[key];
}

int Json::size() const
{
    switch (m_Type)
    {
    case ArrayValue:
        return m_value.m_array->size();

    case ObjectValue:
        return m_value.m_object->size();

        // case StringValue:
        //     return (*m_value.m_string).size();

    default:
        break;
    }
    throw std::logic_error("data type is abnormal, non container!");
}

bool Json::empty() const
{
    switch (m_Type)
    {
    case NullValue:
        return true;

    case ArrayValue:
        return m_value.m_array->empty();

    case ObjectValue:
        return m_value.m_object->empty();

    default:
        break;
    }
    return false;
}

std::string Json::serial() const
{
    std::stringstream ss;

    switch (m_Type)
    {
    case NullValue:
        ss << "null";
        break;

    case IntValue:
        ss << m_value.m_int;
        break;

    case DoubleValue:
        ss << m_value.m_double;
        break;

    case BoolValue:
        if (m_value.m_bool)
        {
            ss << "true";
        }
        else
        {
            ss << "false";
        }
        break;

    case StringValue:
        ss << '\"';
        ss << *m_value.m_string;
        ss << '\"';
        break;

    /**
     * @brief json 属于数组，对 json 对象的数组包含，一般一个 json 一个数据包，属于一个大对象
     */
    case ArrayValue:
    {
        ss << '[' << '\n';
        for (auto li = m_value.m_array->begin(); li != m_value.m_array->end(); li++)
        {
            if (li != m_value.m_array->begin())
            {
                ss << "," << '\n';
            }
            ss << (*li).serial();
        }
        ss << '\n'
           << ']';
        break;
    }

    /**
     * @brief { "age":123,"gende":"man" }
     */
    case ObjectValue:
    {
        ss << '{' << '\n';
        for (auto li = m_value.m_object->begin(); li != m_value.m_object->end(); li++)
        {
            if (li != m_value.m_object->begin())
            {
                ss << "," << '\n';
            }
            ss << '\"' << (*li).first << '\"' << ':' << (*li).second.serial();
        }
        ss << '\n'
           << '}';
        break;
    }

    default:
        break;
    }

    return ss.str();
}

void Json::parse(const std::string &jsonTxt)
{
    Parser parser;
    parser.load(jsonTxt);
    *this = parser.parse();
}

void Json::parse(const std::ifstream &in)
{
    std::stringstream ss;
    ss << in.rdbuf();
    const string &data = ss.str();
    parse(data);
}