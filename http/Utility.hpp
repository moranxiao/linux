#pragma once

#include <string>
#include <vector>


void CutString(const std::string& s, const std::string &sep, std::vector<std::string> *out)
{
    if(out == nullptr)
        return;
    size_t start = 0;
    while(start < s.size())
    {
        size_t pos = s.find(sep,start);
        if(pos == std::string::npos)
            break;
        out->push_back(s.substr(start,pos-start));
        start = pos + sep.size();
    
    }
    if(start < s.size())
    {
        out->push_back(s.substr(start));
    }
}