#include "loggable.hpp"

using namespace nuvk;

Loggable::Loggable(const std::string &section) :
    m_section(section)
{

}
std::string Loggable::getLogSection() const
{
    return m_section;
}