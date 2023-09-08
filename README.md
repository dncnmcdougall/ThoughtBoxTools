# Thoughtbox Tools

ThoughtBox is my name for a
[zettelkasten](https://zettelkasten.de/introduction/). I have used this method
successfully to aid in research towards a PhD. But I like CLI and NVIM. 

This set of tools are some simple CLI programs designed to be used by a vim
plugin to read and write to a sqlite database that stores the metadata for my
thoughts. This greatly improves the speed of searching thought the thoughts as
they do not need to be reparsed on loading vim, or worse, on each search.

Once built there is the read tool that can be used for searching the database.
And the write tool that can be used for updating the metadata of a thought. The
parse tool is not implemented.

I suppose that the most interesting parts of this code are the argument parsing
library and the logging library. The argument parsing library is sufficient for
this code, but would need work to be generally applicable. The lined in
[argparse](https://github.com/p-ranav/argparse) library is a better library for
this, but it is fun to try writing one oneself (if your not in a hurry and it is
a personal project). The logging library is rudimentary.


