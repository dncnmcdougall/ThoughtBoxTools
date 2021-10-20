#include "CliArgs.h"

#include <sstream>

#include "logging.h"


CliArgs::CliArgs() 
{
    m_help = Arg().setLong("--help").setShort("-h").setCount(0);
}

int CliArgs::parse(int argc, char ** argv)
{
    Arg* activeArg  = nullptr;

    for (int i = 1; i < argc; ++i)
    {
        log::debug<<"Parsing("<<(activeArg == nullptr? 0: 1)<<"): "<<argv[i]<<std::endl;
        if (isHelp(argv[i]))
        {
            log::message<< help();
            return 0;
        } 
        else if (activeArg != nullptr ) 
        {
            switch( activeArg->consume(argv[i]) ) 
            {
                case Arg::CONTINUE:
                    log::debug<<"Consuming with: "<<activeArg->longName()<<std::endl;
                    continue;
                case Arg::COMPLETE:
                    log::debug<<"Completing: "<<activeArg->longName()<<std::endl;
                    activeArg = nullptr;
                    continue;
                case Arg::ABORT:
                    log::debug<< "Abort while processing arg: " << activeArg->longName() << std::endl;
                    return 1;
            }
        } 
        else 
        {
            bool foundArg = false;
            for( Arg& arg: m_args) {
                if ( arg.shouldActivate(argv[i]) )
                {
                    log::debug<<"Activating: "<<arg.longName()<<std::endl;
                    activeArg = &arg;
                    switch( activeArg->consume(argv[i]) ) 
                    {
                        case Arg::CONTINUE:
                            log::debug<<"Consuming with: "<<activeArg->longName()<<std::endl;
                            break;
                        case Arg::COMPLETE:
                            log::debug<<"Completing: "<<activeArg->longName()<<std::endl;
                            activeArg = nullptr;
                            break;
                        case Arg::ABORT:
                            log::debug<< "Abort while processing arg: " << activeArg->longName() << std::endl;
                            return 1;
                    }
                    foundArg = true;
                    break;
                }
            }
            if ( activeArg == nullptr && !foundArg ) 
            {
                log::debug<<"push extra: "<<argv[i]<<std::endl;
                m_positionalParams.push_back(argv[i]);
            }
        }
    }
    if (activeArg != nullptr ) 
    {
        log::error<< "Incorrect number of parameters parsed to "<<activeArg->longName()<<std::endl;
        log::message << std::endl;
        log::message << help()<<std::endl;
        return 1;
    }
    return 0;
}

bool CliArgs::isHelp(const char* param) const
{
    return m_help.shouldActivate(param);
}

std::string CliArgs::help() const
{
    std::stringstream out;
    out<<m_description<<std::endl;
    out<<std::endl;
    for( const Arg& arg: m_args) 
    {
        out<<arg.description()<<std::endl;
    }
    return out.str();
}

unsigned long int CliArgs::add(Arg arg) 
{ 
    m_args.push_back(arg); return m_args.size()-1; 
};

Arg& CliArgs::get(unsigned long int param)
{
    return m_args[param];
}

void CliArgs::setDescription(const char* description)
{ 
    m_description = std::string(description);
}
