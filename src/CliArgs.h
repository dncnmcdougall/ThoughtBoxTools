#pragma once

#include <vector>
#include <string>

#include "Arg.h"


class CliArgs 
{

public:
    CliArgs();

    int parse(int argc, char** argv);

    const std::string& description() const  {return m_description; }
    bool isHelp(const char* param) const;
    std::string help() const;

    std::vector<Arg>::iterator begin() noexcept { return m_args.begin(); };
    std::vector<Arg>::const_iterator begin() const noexcept { return m_args.begin(); };

    std::vector<Arg>::iterator end() noexcept { return m_args.end(); };
    std::vector<Arg>::const_iterator end() const noexcept { return m_args.end(); };

    const std::vector<std::string>& positionalParams() const {return m_positionalParams;};

protected:
    unsigned long int add(Arg arg);
    Arg& get(unsigned long int param);

    void setDescription(const char* description);

private:

    std::vector<Arg> m_args;

    std::vector<std::string> m_positionalParams;

    std::string m_description;
    Arg m_help;
};

