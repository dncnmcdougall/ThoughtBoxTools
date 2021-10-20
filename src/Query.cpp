#include "Query.h"

#include <iostream>
#include <sqlite3.h>

#include "DB.h"
#include "logging.h"

const Query Query::query(DB& db, const std::string& sql) 
{
    return Query(db, sql);
}

void Query::then(Query::Callback callback) const
{
    char * error = nullptr;
    log::debug<<"Begining: "<<std::endl;
    log::debug<<m_sql<<std::endl;
    int error_code = sqlite3_exec( m_db.getPointer(), m_sql.c_str(), 
        process, &callback, &error);
    if ( error_code != SQLITE_OK ) 
    {
        std::cerr<< "Error in "<<m_sql<<":" << std::endl;
        std::cerr<< error << std::endl;
    }
    sqlite3_free(error);
    log::debug<<"Done"<<std::endl;
}

int Query::process(void* callback, int columns, char** values, char** titles)
{
    if ( callback != nullptr ) {
        Callback& call = *static_cast<Callback*>(callback);
        return call(columns, values, titles);
    }
    return 0;
}
