#include "ReadArgs.h"

ReadArgs::ReadArgs() 
{
    setDescription("A program to read the details of tags and memories.");

    m_tag = add(Arg().setLong("--tag").setShort("-t").setCount(1).setUnique()
        .setDescription("Display the tag and the memories associated with it."));
    m_tags = add(Arg().setLong("--tags").setCount(0)
        .setDescription("Displays all the tags and the memories associated with them."));

    m_memory = add(Arg().setLong("--memory").setShort("-m").setCount(1).setUnique()
        .setDescription("Display the memory and its details."));
    m_memories = add(Arg().setLong("--memories").setCount(0)
        .setDescription("Display all the memories."));

    m_detailed_memories = add(Arg().setShort("-d").setLong("--detailed-memories").setCount(0)
        .setDescription("Display all details of all the memories."));

    m_database = add(Arg().setLong("--database").setShort("-db").setCount(1).setUnique()
        .setDescription("The name of the database file to use."));
}

