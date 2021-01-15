#pragma once

#include <string>

namespace nuvk
{
    class Logger
    {
    public:
        static void Info(std::string_view msg);        
        static void Warning(std::string_view msg);
        static void Fatal(std::string_view msg);
    
        static void Info(std::string_view section, std::string_view msg);
        static void Warning(std::string_view section, std::string_view msg);
        static void Fatal(std::string_view section, std::string_view msg);
    };
}