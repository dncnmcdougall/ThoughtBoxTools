#include <iostream>
#include <filesystem>
#include <map>
#include <stdexcept>

#include <sqlite3.h>

#include "DB.h"
#include "Query.h"
#include "WriteArgs.h"
#include "logging.h"
#include "utils.h"

namespace fs = std::filesystem;

int main(int argc, char ** argv) {

    Logging::setLevel(Logging::MESSAGE);

    WriteArgs args;

    int result = args.parse(argc, argv);
    if ( result != 0 )
    {
        return result;
    }
    const std::vector<std::string>& extra_params = args.positionalParams();

    std::string memory;
    std::string title;

    if (extra_params.size() < 2) 
    {
        log::error<<"Please provide the memory number and title."<<std::endl;
        return 1;
    }

    bool first = true;
    for( const std::string& value : extra_params)
    {
        if ( first )
        {
            memory = value;
            first = false;
        }
        else
        {
            title += " "+value;
        }
    }
    title.erase(0,1);

    title = utils::sqlescape(title);

    for( Arg& arg: args) {
        log::debug<<arg.longName()<<": "<<arg.params().size()<<std::endl;
    }

    std::string name = "test.db";
    if (args.database().params().size() > 0) 
    {
        name = args.database().params().back()[0];
    }

    bool create_tables = false;

    if ( !fs::exists(name) ) 
    {
        log::debug << "Creating DB: "<< name<<std::endl;
        create_tables = true;
    } 
    else 
    {
        log::debug << "Opening DB: "<< name<<std::endl;
    }

    log::debug << "Details: "<< memory<<": "<<title<<std::endl;

    DB db(name);

    if ( create_tables ) 
    {
        Query::query(db, "CREATE TABLE memories (number TEXT PRIMARY KEY, title TEXT)").then(nullptr);
        Query::query(db, "CREATE TABLE tags (number INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT UNIQUE)").then(nullptr);
        Query::query(db, "CREATE TABLE links (source TEXT, target TEXT)").then(nullptr);
        Query::query(db, "CREATE TABLE tag_links (memory TEXT, tag INTEGER)").then(nullptr);
    }

    Query::query(db, "DELETE FROM memories WHERE number IS '"+memory+"'").then(nullptr);
    Query::query(db, "DELETE FROM links WHERE source IS '"+memory+"'").then(nullptr);
    Query::query(db, "DELETE FROM tag_links WHERE memory IS '"+memory+"'").then(nullptr);

    Query::query(db, "INSERT INTO memories (number, title) VALUES ('"+memory+"', '"+title+"')").then(nullptr);

    if ( args.links().params().size() > 0 )
    {
        std::string linkValues;
        for( const std::vector<std::string>& link: args.links().params())
        {
            linkValues += "('"+memory+"', '"+utils::sqlescape(link[0])+"'), ";
        }
        linkValues.erase(linkValues.size()-2,2);
        Query::query(db, "INSERT INTO links (source, target) VALUES "+linkValues).then(nullptr);
    }

    if ( args.tags().params().size() > 0 )
    {
        std::string tagValues;
        for( const std::vector<std::string>& tag: args.tags().params())
        {
            tagValues += " ('"+utils::sqlescape(tag[0])+"'), ";
        }
        if ( tagValues.size() > 0 ) 
        {
            tagValues.erase(tagValues.size()-2,2);
            Query::query(db, "INSERT INTO tags (title) VALUES "+tagValues+" ON CONFLICT DO NOTHING").then(nullptr);
        }

        std::string tagQuery;
        for( const std::vector<std::string>& tag: args.tags().params())
        {
            tagQuery += " (title='"+utils::sqlescape(tag[0])+"') OR ";
        }
        if ( tagQuery.size() > 0 ) 
        {
            tagQuery.erase(tagQuery.size()-4,4);

            tagValues = "";
            auto process = [&](int count, char** values, char** titles) 
                {
                    tagValues += std::string("('")+memory+"',"+values[1]+"), ";
                    return 0;
                };
            Query::query(db, "SELECT title, number FROM tags WHERE "+tagQuery).then(process);
            tagValues.erase(tagValues.size()-2,2);

            Query::query(db, "INSERT INTO tag_links (memory, tag) VALUES "+tagValues).then(nullptr);
        }
    }

    return 0;
}
