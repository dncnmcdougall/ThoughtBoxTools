#include <algorithm>

#include <argparse/argparse.hpp>

#include "logging.h"

int main(int argc, char ** argv )
{
    argparse::ArgumentParser args("parse");

    args.add_argument("thought")
        .help("The id of the thought");

    int verbosity = 2;
    args.add_argument("-V","--verbose")
        .help("Increases the verbosity with which to print messages.")
        .action([&verbosity](const auto &) { ++verbosity;} )
        .default_value(false)
        .implicit_value(true)
        .append()
        .nargs(0);

    args.add_argument("-Q","--silent")
        .help("Decreases the verbosity with which to print messages.")
        .action([&verbosity](const auto &) { --verbosity;} )
        .default_value(false)
        .implicit_value(true)
        .append()
        .nargs(0);

    try
    {
        args.parse_args(argc, argv);
    } 
    catch (const std::runtime_error& err) 
    {
        std::cerr << err.what() << std::endl;
        std::cerr << args;
        std::exit(1);
    }
    int iLevel = std::clamp(verbosity, (int)Logging::ERROR, (int)Logging::DEBUG);
    Logging::Level level = (Logging::Level)std::clamp(verbosity, (int)Logging::ERROR, (int)Logging::DEBUG);
    Logging::setLevel(level);

    std::cout<<"Verbosity: "<<verbosity<<" ("<<iLevel<<"): "<<level<<" Logging: "<<Logging::level()<<std::endl;

    std::cout<< args.get("thought")<<std::endl;
}
