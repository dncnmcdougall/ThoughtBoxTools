#pragma once

#include <vector>
#include <iostream>

namespace utils
{

std::string sqlescape(std::string str)
{
    size_t pos = str.find("'");
    while( pos != std::string::npos)
    {
        str = str.replace(pos, 1, "''");
        pos = str.find("'", pos+2);
    }

    return str;
}

template <typename CharT>
std::vector<std::string> split(const std::string& str, CharT sep)
{
    std::vector<std::string> parts;
    int ppos = 0;
    int pos = str.find(sep);
    while( pos != std::string::npos ) 
    {
        parts.push_back(str.substr(ppos, pos-ppos));
        ppos = pos+1;
        pos = str.find(sep, ppos);
    }
    if ( ppos < str.size() )
    {
        parts.push_back(str.substr(ppos));
    }
    return parts;
}

}
