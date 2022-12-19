#include "10Parser.h"

using std::logic_error;

#define m_jsonIndex m_json[m_index]

Parser::Parser() : m_json(""), m_index(0)
{
}

Parser::~Parser()
{
}

void Parser::load(const string &jsonTxt)
{
    m_json = jsonTxt;
    m_index = 0;
}

void Parser::skip_whitespace()
{
    while (m_json[m_index] == ' ' || m_json[m_index] == '\r' || m_json[m_index] == '\n' || m_json[m_index] == '\t')
    {
        m_index++;
    }
}

char Parser::nextChar()
{
    skip_whitespace();
    if (m_index == m_json.size())
    {
        throw logic_error("end of string, output next char exception!");
    }
    return m_json[m_index++];
}

Json Parser::parseNull()
{
    if (m_json.compare(m_index, 4, "null") == 0 || m_json.compare(m_index, 4, "NULL") == 0)
    {
        m_index += 4;
        return Json();
    }
    else
    {
        throw logic_error("data is not \"null\" , or \"NULL\"");
    }
}

Json Parser::parseNumber()
{
    size_t pos = m_index;

    if (m_jsonIndex == '-')
    {
        m_index++;
    }

    if (m_jsonIndex >= '1' && m_jsonIndex <= '9')
    {
        m_index++;
        while (m_jsonIndex >= '0' && m_jsonIndex <= '9')
        {
            m_index++;
        }
    }
    else if (m_jsonIndex == '0')
    {
        m_index++;
        char ch = nextChar();
        if (ch == '.')
        {
            m_index--;
        }
        else
        {
            throw logic_error("number type exception");
        }
    }
    else
    {
        throw logic_error("number exception, it is not a number");
    }

    if (m_jsonIndex == '.')
    {
        m_index++;

        while (1)
        {
            if (m_jsonIndex >= '0' && m_jsonIndex <= '9')
            {
                m_index++;
            }
            else
            {
                break;
                // throw logic_error("data type is not double, end of the point!");
            }
        }

        return Json(stof(m_json.substr(pos, m_index)));
        // return Json(atof(m_json.c_str() + pos));
    }
    else
    {
        return Json(stoi(m_json.substr(pos, m_index)));
        // return Json(atoi(m_json.c_str() + pos));
    }
}

Json Parser::parseBool()
{
    if (m_json.compare(m_index, 5, "false") == 0 || m_json.compare(m_index, 5, "False") == 0)
    {
        m_index += 5;
        return Json(false);
    }
    else if (m_json.compare(m_index, 4, "true") == 0 || m_json.compare(m_index, 4, "True") == 0)
    {
        m_index += 4;
        return Json(true);
    }
    else
    {
        throw logic_error("not found the \'bool\' type, data exception!");
    }
}

string Parser::parseString()
{
    size_t pos = m_index;

    while (1)
    {
        if (m_index == m_json.size())
        {
            throw logic_error("end of string already");
        }

        char ch = m_json[m_index++];
        if (ch == '\"')
        {
            break;
        }

        if (ch == '\\')
        {
            ch = m_json[m_index++];
            switch (ch)
            {
            case 'b':
            case 'f':
            case 'n':
            case 'r':
            case 't':
            case '\"':
            case '\\':
            case '/':
                break;

            case 'u':
                m_index += 4;

                break;
            default:
                break;
            }
        }
    }
    return m_json.substr(pos, m_index - pos - 1);
}

Json Parser::parseArray()
{
    if (m_index == m_json.size())
    {
        throw logic_error("end of data array already!");
    }
    else
    {
        Json arrayJson = Json(Json::ArrayValue);
        char ch = nextChar();
        if (m_jsonIndex == ']')
        {
            return arrayJson;
        }
        m_index--;
        while (true)
        {
            arrayJson.append(parse());
            ch = nextChar();
            if (ch == ']')
            {
                break;
            }

            if (ch != ',')
            {
                throw logic_error("not found the symbol of \',\' in array");
            }
        }
        return arrayJson;
    }
}

Json Parser::parseObject()
{
    if (m_index == m_json.size())
    {
        throw logic_error("end of object data already!");
    }
    else
    {
        Json objectJson = Json(Json::ObjectValue);
        char ch = nextChar();
        if (m_jsonIndex == '}')
        {
            return objectJson;
        }
        m_index--;
        while (true)
        {
            ch = nextChar();
            if (ch != '\"')
            {
                throw logic_error("not found the key in object!");
            }
            std::string key = parseString();

            ch = nextChar();
            if (ch != ':')
            {
                throw logic_error("not found the symbol of \':\' in object!");
            }
            objectJson[key] = parse();

            ch = nextChar();
            if (ch == '}')
            {
                break;
            }
            if (ch != ',')
            {
                throw logic_error("not found the symbol of \',\' in objectJson！");
            }
        }
        return objectJson;
    }
}

Json Parser::parse()
{
    char ch = nextChar();
    switch (ch)
    {
    case 'n':
    case 'N':
        m_index--;
        return parseNull();

    case '-':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '0':
        m_index--;
        return parseNumber();

    case '\"':
        return parseString();

    case 'f':
    case 'F':
    case 't':
    case 'T':
        m_index--;
        return parseBool();

    case '[':
        return parseArray();

    case '{':
        return parseObject();

    default:
        break;
    }
    throw logic_error("parse char exception! in " + std::to_string(m_index));
}