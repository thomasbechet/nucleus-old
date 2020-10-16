#include "logger.hpp"

#include "loggable.hpp"

#include <nucleus/nucleus.h>

using namespace nuvk;

void Logger::Info(std::string msg)
{
    nu_info(("[NUVK] " + msg + "\n").c_str());
}
void Logger::Warning(std::string msg)
{
    nu_warning(("[NUVK] " + msg + "\n").c_str());
}
void Logger::Fatal(std::string msg)
{
    nu_fatal(("[NUVK] " + msg + "\n").c_str());
}

void Logger::Info(std::string section, std::string msg)
{
    nu_info(("[NUVK][" + section + "]" + msg + "\n").c_str());
}
void Logger::Warning(std::string section, std::string msg)
{
    nu_warning(("[NUVK][" + section + "]" + msg + "\n").c_str());
}
void Logger::Fatal(std::string section, std::string msg)
{
    nu_fatal(("[NUVK][" + section + "]" + msg + "\n").c_str());
}