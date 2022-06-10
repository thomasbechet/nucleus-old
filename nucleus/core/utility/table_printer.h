#ifndef NU_TABLE_PRINTER_H
#define NU_TABLE_PRINTER_H

#include <nucleus/core/utility/string.h>

NU_DECLARE_HANDLE(nu_table_printer_t);

NU_API nu_table_printer_t nu_table_printer_allocate_(nu_allocator_t a, const char *file, uint32_t line);
NU_API void nu_table_printer_free(nu_table_printer_t t);
NU_API void nu_table_printer_separator(nu_table_printer_t t);
NU_API void nu_table_printer_row(nu_table_printer_t t, const char *format, ...);
NU_API void nu_table_printer_row_center(nu_table_printer_t t, const char *format, ...);
NU_API const char *nu_table_printer_render(nu_table_printer_t t);

#define nu_table_printer_allocate(a) nu_table_printer_allocate_(a, __FILE__, __LINE__)

#endif