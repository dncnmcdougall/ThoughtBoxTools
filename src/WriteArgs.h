#pragma once

#include "CliArgs.h"

class WriteArgs: public CliArgs {

public:

    WriteArgs();

    Arg& tags() {return get(m_tags);};
    Arg& links() {return get(m_links);};

    Arg& database() {return get(m_database);};

private:
    unsigned long int m_links;
    unsigned long int m_tags;

    unsigned long int m_database;
};
