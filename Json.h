#pragma once

#include <string>
#include <list>
#include <map>
#include <regex>
#include <fstream>
#include <iostream>

class Json
{
public:
    // 数据类型
    enum TYPE
    {
        NullValue = 0,
        IntValue,
        DoubleValue,
        // JSON_NUMBER,
        BoolValue,
        StringValue,
        ArrayValue,
        ObjectValue
    };

    Json();
    Json(TYPE type);
    Json(bool value);
    Json(int value);
    Json(double value);
    Json(const char *value);
    Json(const std::string &value);
    Json(const Json &otherJson);
    ~Json();

    TYPE type() const;
    // template <typename T>
    // T value() const;

    void append(const Json &otherJson);
    void remove(int index);
    void remove(const char *key);
    void remove(const std::string &key);

    operator bool();
    operator int();
    operator double();
    operator std::string();

    Json &operator[](const int index);
    Json &operator[](const char *key);
    Json &operator[](const std::string key);
    Json &operator=(const Json &otherJson);
    bool operator==(const Json &otherJson);
    bool operator!=(const Json &otherJson);

    bool isNull() const;
    bool isBool() const;
    bool isInt() const;
    bool isDouble() const;
    bool isString() const;
    bool isArray() const;
    bool isObject() const;

    bool asBool() const;
    int asInt() const;
    double asDouble() const;
    std::string asString() const;

    bool hasInArray(int index) const;
    bool hasInObject(const char *key) const;
    bool hasInObject(const std::string &key) const;

    Json getInArray(int index) const;
    Json getInObject(const char *key) const;
    Json getInObject(const std::string &key) const;

private:
    union Value
    {
        bool m_bool;
        int m_int;
        double m_double;
        // char m_number[256];
        std::string *m_string;
        std::list<Json> *m_array;
        std::map<std::string, Json> *m_object;
    };

    TYPE m_Type;
    Value m_value;

public:
    void copy(const Json &otherJson);

    void clear();

    std::string serial() const;

    int size() const;

    bool empty() const;

    friend std::ostream &operator<<(std::ostream &os, const Json &json)
    {
        os << json.serial();
        return os;
    }

    typedef std::list<Json>::iterator iterator;
    iterator begin()
    {
        return (m_value.m_array)->begin();
    }
    iterator end()
    {
        return (m_value.m_array)->end();
    }

    void parse(const std::string &jsonTxt);
    void parse(const std::ifstream &in);
};
