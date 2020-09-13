#include "cursor.hpp"

using namespace nudebug;

cursor_t::cursor_t()
{
    m_rect.top = 0;
    m_rect.left = 0;
    m_rect.width = 5;
    m_rect.height = 1;
}
cursor_t::~cursor_t()
{

}