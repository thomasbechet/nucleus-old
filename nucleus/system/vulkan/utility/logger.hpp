#pragma once

#include <string>

namespace nuvk
{
    class Logger
    {
    public:
        static void Info(std::string msg);
        static void Warning(std::string msg);
        static void Fatal(std::string msg);
    };
}