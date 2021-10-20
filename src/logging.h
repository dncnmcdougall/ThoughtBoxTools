#pragma once
#include <iostream>

class Logging
{
public:

    enum Level {
    ERROR=0,
    WARNING=1,
    MESSAGE=2,
    DEBUG=3
    };

    Logging(Level level) : m_level(level) {};

    typedef std::basic_ostream<char, std::char_traits<char>> CoutType;
    Logging& operator<<(CoutType& (*value)(CoutType&) ) { 
        if ( m_level <= s_level) {
            switch(m_level) {
            case ERROR: [[fallthrough]];
            case DEBUG:
                std::cerr<<value;
                break;
            case MESSAGE: [[fallthrough]];
            case WARNING:
                std::cout<<value;
                break;
            default:
                std::cout<<value;
                break;
            }
        }
        return *this;
    };

    template<typename T>
    Logging& operator<<(const T& value) { 
        if ( m_level <= s_level) {
            switch(m_level) {
            case ERROR: [[fallthrough]];
            case DEBUG:
                std::cerr<<value;
                break;
            case MESSAGE: [[fallthrough]];
            case WARNING:
                std::cout<<value;
                break;
            default:
                std::cout<<value;
                break;
            }
        }
        return *this;
    }

    static void setLevel(Level level) { s_level=level; };

private:
    const Level m_level;

    static Level s_level;

};

namespace log
{

extern Logging error;
extern Logging warn;
extern Logging message;
extern Logging debug;

}
