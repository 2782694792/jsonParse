#pragma once

#include "Json.h"

using std::string;

class Parser
{
public:
    Parser();
    ~Parser();

    Json parseNull();
    Json parseNumber();
    Json parseBool();
    string parseString();
    Json parseArray();
    Json parseObject();

private:
    string m_json;
    size_t m_index;

public:
    void skip_whitespace();

    char nextChar();

    Json parse();

    void load(const string &jsonTxt);
};