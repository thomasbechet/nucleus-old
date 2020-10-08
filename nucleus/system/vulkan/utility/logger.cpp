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
    Info("[" + loggable->getLogSection() + "]" + msg);
}
void Logger::Warning(const Loggable *loggable, std::string msg)
{
    Warning("[" + loggable->getLogSection() + "]" + msg);
}
void Logger::Fatal(const Loggable *loggable, std::string msg)
{
    Fatal("[" + loggable->getLogSection() + "]" + msg);
}