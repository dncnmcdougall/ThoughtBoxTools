#include "DB.h"

#include <iostream>

// ----- CONSTRUCTOR -----

DB::DB(const std::string& name) 
: m_sqlite(nullptr)
{
    int error_code = sqlite3_open(name.c_str(), &m_sqlite);
    if ( error_code != SQLITE_OK ) 
    {
        std::cerr<< sqlite3_errmsg(m_sqlite) << std::endl;
        sqlite3_close(m_sqlite);
        m_sqlite = nullptr;
    }
}

DB::~DB() 
{
    if ( m_sqlite != nullptr ) {
        sqlite3_close(m_sqlite);
    }
}

// ----- PUBLIC -----

sqlite3* DB::getPointer() 
{
    return m_sqlite;
}

sqlite3* DB::operator&() 
{
    return getPointer();
}
