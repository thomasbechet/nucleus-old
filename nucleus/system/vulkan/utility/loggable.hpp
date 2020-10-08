#pragma once

#include "logger.hpp"

#include <string>

namespace nuvk
{    
    class Loggable
    {
    public:
        Loggable(const std::string &section);
        std::string getLogSection() const;
    private:
        std::string m_section;
    };
}