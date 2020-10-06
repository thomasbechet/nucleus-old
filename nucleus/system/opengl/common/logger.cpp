#include "logger.hpp"

#include "../../../core/nucleus.h"

void nugl::log_warning(std::string message)
{
    std::string msg = "[OPENGL] " + message;
    nu_warning(msg.c_str());
}
void nugl::log_info(std::string message)
{
    std::string msg = "[OPENGL] " + message;
    nu_info(msg.c_str());
}
void nugl::fatal(std::string message)
{
    std::string msg = "[OPENGL] " + message;
    nu_fatal(msg.c_str());
}