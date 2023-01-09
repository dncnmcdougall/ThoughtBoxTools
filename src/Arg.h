#pragma once

#include <iostream>
#include <vector>

class Arg
{

    public: 
    enum status {
    CONTINUE,
    COMPLETE,
    ABORT
    };

    Arg& setCount(long unsigned int params) {m_params = params; return *this;} ;
    Arg& setLong(std::string name) {m_longName = name; return *this;} ;
    Arg& setShort(std::string name) {m_shortName = name; return *this;} ;
    Arg& setDescription(std::string desc) {m_desc = desc; return *this;} ;
    Arg& setUnique() {m_unique = true; return *this;} ;
    Arg& setMultiple() {m_unique = false; return *this;} ;

    bool shouldActivate(const char* arg) const;
    status consume(const char* value);

    unsigned long int occurences() const;

    const std::vector<std::vector<std::string>>& params() const { return m_collectedParams;};
    std::cats::string longName() const {return m_longName;};

    std::string description() const;
    std::string rawDescription() const { return m_desc; };

    private:

    long unsigned int m_params = 0;
    std::string m_longName = "";
    std::string m_shortName = "";
    std::string m_desc = "";
    bool m_unique = false;

    bool m_begin = true;
    std::vector<std::vector<std::string>> m_collectedParams;

};
