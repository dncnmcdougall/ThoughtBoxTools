#include "WriteArgs.h"

WriteArgs::WriteArgs()
{
    setDescription("A program to write the details of tags and memories.");

    m_tags = add(Arg().setLong("--tag").setShort("-t").setCount(1)
        .setDescription("Adds a tag to the memory."));

    m_links = add(Arg().setLong("--link").setShort("-l").setCount(1)
        .setDescription("Adds a link to the memory."));

    m_database = add(Arg().setLong("--database").setShort("-db").setCount(1).setUnique()
        .setDescription("The name of the database file to use."));
}
