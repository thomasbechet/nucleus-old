#pragma once

#include <string>

namespace nuvk
{
    class Loggable;

    class Logger
    {
    public:
        static void Info(std::string msg);        
        static void Warning(std::string msg);
        static void Fatal(std::string msg);
    
        static void Info(const Loggable *loggable, std::string msg);
        static void Warning(const Loggable *loggable, std::string msg);
        static void Fatal(const Loggable *loggable, std::string msg);
    };
}