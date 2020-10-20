#include "logger.hpp"

#include <nucleus/nucleus.h>

using namespace nuvk;

void Logger::Info(std::string_view msg)
{
    nu_info(("[NUVK] " + std::string(msg) + "\n").c_str());
}
void Logger::Warning(std::string_view msg)
{
    nu_warning(("[NUVK] " + std::string(msg) + "\n").c_str());
}
void Logger::Fatal(std::string_view msg)
{
    nu_fatal(("[NUVK] " + std::string(msg) + "\n").c_str());
}

void Logger::Info(std::string_view section, std::string_view msg)
{
    nu_info(("[NUVK][" + std::string(section) + "]" + std::string(msg) + "\n").c_str());
}
void Logger::Warning(std::string_view section, std::string_view msg)
{
    nu_warning(("[NUVK][" + std::string(section) + "]" + std::string(msg) + "\n").c_str());
}
void Logger::Fatal(std::string_view section, std::string_view msg)
{
    nu_fatal(("[NUVK][" + std::string(section) + "]" + std::string(msg) + "\n").c_str());
}