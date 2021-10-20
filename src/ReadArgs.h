#pragma once


#include "CliArgs.h"

class ReadArgs: public CliArgs {

public:

    ReadArgs();

    Arg& tag() { return get(m_tag); };
    Arg& tags() { return get(m_tags); };

    Arg& memory() { return get(m_memory); };
    Arg& memories() { return get(m_memories); };

    Arg& detailed_memories() { return get(m_detailed_memories); };

    Arg& database() {return get(m_database);};

private:
    unsigned long int m_tag;
    unsigned long int m_tags;

    unsigned long int m_memory;
    unsigned long int m_memories;

    unsigned long int m_detailed_memories;

    unsigned long int m_database;
};
