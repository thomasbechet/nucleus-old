#pragma once

#include <string>

namespace nugl
{
    void log_warning(std::string message);
    void log_info(std::string message);
    void fatal(std::string message);
}