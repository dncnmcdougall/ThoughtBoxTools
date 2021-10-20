#include <iostream>
#include <filesystem>
#include <map>
#include <stdexcept>
#include <algorithm>

#include <sqlite3.h>

#include "DB.h"
#include "Query.h"
#include "ReadArgs.h"
#include "logging.h"
#include "Utils.h"

namespace fs = std::filesystem;

struct Result
{
    Result(): code(0), result("") {};
    Result(int code, std::string result) : code(code), result(result) {};
    int code;
    std::string result;
};

Result displayTag(DB& db, const std::string& tag)
{
    Result res(0, tag);
    res.result+=": \n";

    std::string tag_number;
    auto processTagNumber = [&](int count, char** values, char** titles) 
        {
            tag_number = std::string(values[0]);
            return 0;
        };
    Query::query(db, "SELECT number FROM tags WHERE title='"+utils::sqlescape(tag)+"'").then(processTagNumber);
    if ( tag_number.size() == 0 ) 
    {
        log::error<<"No memories found with tag: "<<tag<<std::endl;
        return Result(1,"");
    }

    auto processMemory = [&](int count, char** values, char** titles) 
        {
            res.result += std::string(values[0])+": "+values[1]+"\n";
            return 0;
        };
    Query::query(db, "SELECT memories.number, memories.title FROM tag_links,memories WHERE tag_links.memory=memories.number AND tag_links.tag="+tag_number+" ORDER BY memories.number").then(processMemory);

    return res;
}

Result displayTags(DB& db)
{
    Result res;

    auto processTag = [&](int count, char** values, char** titles) 
        {
            auto parts = utils::split(values[2], ",");
            // TODO: impliment custom sorter.
            std::sort(parts.begin(), parts.end());

            res.result+=values[1];
            res.result+=": ";
            for( std::size_t i=0;i<(parts.size()-1);++i)
            {
                res.result+=parts[i]+", ";
            }
            res.result+=parts.back()+"\n";
            return 0;
        };
    Query::query(db, "SELECT tag_links.tag, tags.title, group_concat(tag_links.memory) FROM tag_links,tags WHERE tag_links.tag=tags.number GROUP BY tags.number").then(processTag);

    return res;
}

Result displayMemory(DB& db, const std::string& memory)
{
    Result res;
    auto processMemory = [&](int count, char** values, char** titles) 
        {
            res.result = memory+": "+values[0]+"\n";
            res.result+="tags: ";

            if ( values[1] != nullptr) {
            auto parts = utils::split(values[1], ",");
            // TODO: impliment custom sorter.
            std::sort(parts.begin(), parts.end());
            for( std::size_t i=0;i<(parts.size()-1);++i)
            {
                res.result+=parts[i]+", ";
            }
            res.result+=parts.back()+"\n";
                }
            else
            {
            res.result+="\n";
                }

            res.result+="links: ";

            if ( values[2] != nullptr) {
            auto parts = utils::split(values[2], ",");
            // TODO: impliment custom sorter.
            std::sort(parts.begin(), parts.end());
            for( std::size_t i=0;i<(parts.size()-1);++i)
            {
                res.result+=parts[i]+", ";
            }
            res.result+=parts.back()+"\n";
                }
            else
            {
            res.result+="\n";
                }
            return 0;
        };
        //TODO: This can be improved so that the uniques above are not necessary.
    Query::query(db, "SELECT title, tags, group_concat(links.target) as links "
    "FROM (SELECT memories.title as title, memories.number as number, group_concat(tags.title) as tags "
    "FROM memories "
    "LEFT JOIN tag_links ON tag_links.memory=memories.number "
    "LEFT JOIN tags ON tags.number=tag_links.tag "
    "WHERE memories.number='"+memory+"' "
    "GROUP BY memories.number) "
    "LEFT JOIN links ON links.source=number "
    "WHERE number='"+memory+"' "
    "GROUP BY number").then(processMemory);

    return res;
}
Result displayMemories(DB& db)
{
    Result res;

    auto processMemory = [&](int count, char** values, char** titles) 
        {
            res.result += std::string(values[0])+": "+values[1]+"\n";
            return 0;
        };
    Query::query(db, "SELECT number, title FROM memories ORDER BY number").then(processMemory);

    return res;
}

Result displayDetailedMemories(DB& db)
{
    
    if (false) {
    Result res;
    std::vector<std::string> memories;

    auto processMemory = [&](int count, char** values, char** titles) 
        {
            memories.push_back(values[0]);
            return 0;
        };
    Query::query(db, "SELECT number FROM memories ORDER BY number").then(processMemory);
    for( const auto& num : memories)
    {
        auto inner_res = displayMemory(db, num);
        if ( inner_res.code != 0 ) 
        {
            return inner_res;
        }
        res.result += inner_res.result;
    }
    return res;
    }

    Result res;
    auto processMemory = [&](int count, char** values, char** titles) 
        {
            res.result += std::string(values[0])+": "+values[1]+"\n";
            res.result+="tags: ";

            if ( values[2] != nullptr) {
            auto parts = utils::split(values[2], ",");
            // TODO: impliment custom sorter.
            std::sort(parts.begin(), parts.end());
            for( std::size_t i=0;i<(parts.size()-1);++i)
            {
                res.result+=parts[i]+", ";
            }
            res.result+=parts.back()+"\n";
                }
            else
            {
            res.result+="\n";
                }

            res.result+="links: ";

            if ( values[3] != nullptr) {
            auto parts = utils::split(values[3], ",");
            // TODO: impliment custom sorter.
            std::sort(parts.begin(), parts.end());
            for( std::size_t i=0;i<(parts.size()-1);++i)
            {
                res.result+=parts[i]+", ";
            }
            res.result+=parts.back()+"\n";
                }
            else
            {
            res.result+="\n";
                }
            return 0;
        };
        //TODO: This can be improved so that the uniques above are not necessary.
    Query::query(db, "SELECT number, title, tags, group_concat(links.target) as links "
    "FROM (SELECT memories.title as title, memories.number as number, group_concat(tags.title) as tags "
    "FROM memories "
    "LEFT JOIN tag_links ON tag_links.memory=memories.number "
    "LEFT JOIN tags ON tags.number=tag_links.tag "
    "GROUP BY memories.number) "
    "LEFT JOIN links ON links.source=number "
    "GROUP BY number").then(processMemory);

    return res;
}

int main(int argc, char ** argv) 
{
    Logging::setLevel(Logging::MESSAGE);

    ReadArgs args;
    int result = args.parse(argc, argv);
    if ( result != 0 )
    {
        return result;
    }
    const std::vector<std::string>& extra_params = args.positionalParams();

    std::string memory;
    std::string title;

    if (extra_params.size() > 0) 
    {
        log::error<<"Extra parameters provided:"<<std::endl;
        for( auto param : extra_params)
        {
            log::error<<"'"<<param<<"'"<<std::endl;
        }
        log::message << std::endl;
        log::message << args.help()<<std::endl;
        return 1;
    }

    int occurences = args.tag().occurences() 
        + args.tags().occurences() 
        + args.memory().occurences()
        + args.memories().occurences()  
        + args.detailed_memories().occurences();

    switch ( occurences )
    {
        case 0:
            log::error<<"Select at least one option for output."<<std::endl;
            log::message << std::endl;
            log::message << args.help()<<std::endl;
            return 1;
        case 1:
            break;
        default:
            log::error<<"Only one of flag can be used at a time."<<std::endl;
            log::message << std::endl;
            log::message << args.help()<<std::endl;
            return 1;
    }

    for( Arg& arg: args) {
        log::debug<<arg.longName()<<": "<<arg.params().size()<<std::endl;
    }

    std::string name = "test.db";
    if (args.database().occurences() > 0) 
    {
        name = args.database().params().back()[0];
    }

    if ( !fs::exists(name) ) 
    {
        log::error << "Could not find database : '"<< name<<"'"<<std::endl;
        return 1;
    } 

    DB db(name);

    if ( args.tags().occurences() > 0 ) 
    {
        auto res = displayTags(db);
        log::message<<res.result;
        return res.code;
    }
    else if ( args.tag().occurences() > 0 ) 
    {
        std::string tag = args.tag().params().back()[0];
        auto res = displayTag(db, tag);
        log::message<<res.result;
        return res.code;
    }
    else if ( args.memories().occurences() > 0 ) 
    {
        auto res = displayMemories(db);
        log::message<<res.result;
        return res.code;
    }
    else if ( args.memory().occurences() > 0 ) 
    {
        std::string memory = args.memory().params().back()[0];
        auto res = displayMemory(db, memory);
        log::message<<res.result;
        return res.code;
    } 
    else if ( args.detailed_memories().occurences() > 0 ) 
    {
        auto res = displayDetailedMemories(db);
        log::message<<res.result;
        return res.code;
    }
    else
    {
        log::error<<"This should be imposible"<<std::endl;
        return 1;
    }

    return 0;
}
