#include <stdexcept>

#include "logging.h"

Logging log::error(Logging::ERROR);
Logging log::warn(Logging::WARNING);
Logging log::message(Logging::MESSAGE);
Logging log::debug(Logging::DEBUG);

Logging::Level Logging::s_level = MESSAGE;

std::ostream& operator<<(std::ostream& out, const Logging::Level& level)
{
    switch(level)
        {
            case Logging::ERROR:
                return out<<"ERROR";
            case Logging::DEBUG:
                return out<<"DEBUG";
            case Logging::MESSAGE:
                return out<<"MESSAGE";
            case Logging::WARNING:
                return out<<"WARNING";
        }

    throw std::runtime_error("Invalid level.");
}
