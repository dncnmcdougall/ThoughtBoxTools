#pragma once

#include <functional>


class DB;
class Query {

    public:

    typedef std::function<int(int, char**, char**)> Callback;

    ~Query() = default;

    static const Query query(DB& db, const std::string& sql);
    void then(Callback callback) const;

    private:
    Query(DB& db, const std::string& sql):m_db(db), m_sql(sql) {};

    static int process(void*, int columns, char** values, char** titles);

    DB& m_db;
    const std::string& m_sql;

};


