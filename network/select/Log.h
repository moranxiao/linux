#pragma once
#include <iostream>
#include <string>

enum {
    INFO,
    DEBUG,
    WARNING,
    ERROR,
    FATAL
};

static std::ostream& Log(const std::string& level,const std::string& file,int line)
{
    std::string buffer;
    buffer += '[';
    buffer += level;
    buffer += ']';

    buffer += '[';
    buffer += file;
    buffer += ']';

    buffer += '[';
    buffer += std::to_string(line);
    buffer += ']';

    std::cout << buffer << ':';
    return std::cout;
}

#define LOG(level) Log(#level,__FILE__,__LINE__)