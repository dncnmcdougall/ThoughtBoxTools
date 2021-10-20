#pragma once

#include <string>

#include <sqlite3.h>

class DB {

    public:

    DB(const std::string& name);
    ~DB();

    sqlite3* getPointer();
    sqlite3* operator&();

    private:

    sqlite3 * m_sqlite;
};
