#include "logging.h"

Logging log::error(Logging::ERROR);
Logging log::warn(Logging::WARNING);
Logging log::message(Logging::MESSAGE);
Logging log::debug(Logging::DEBUG);

Logging::Level Logging::s_level = MESSAGE;
