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

void Logger::Info(const Loggable *loggable, std::string msg)
{
    nu_info(("[NUVK][" + loggable->getLogSection() + "]" + msg + "\n").c_str());
}
void Logger::Warning(const Loggable *loggable, std::string msg)
{
    nu_warning(("[NUVK][" + loggable->getLogSection() + "]" + msg + "\n").c_str());
}
void Logger::Fatal(const Loggable *loggable, std::string msg)
{
    nu_fatal(("[NUVK][" + loggable->getLogSection() + "]" + msg + "\n").c_str());
}