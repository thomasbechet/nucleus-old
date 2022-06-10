#include <nucleus/core/utility/table_printer.h>

#include <nucleus/core/system/allocator/api.h>
#include <nucleus/core/utility/container/vector.h>

#define MAX_COLUMN_COUNT        16
#define DEFAULT_RENDER_CAPACITY 128
#define DEFAULT_LINE_CAPACITY   8
#define SEPARATOR               "|"

typedef enum {
    NU_LINE_TYPE_DEFAULT,
    NU_LINE_TYPE_END_SECTION,
    NU_LINE_TYPE_SEPARATOR
} nu_line_type_t;

typedef struct {
    nu_line_type_t type;
    nu_string_t data;
    bool align_center;
} nu_line_data_t;

typedef struct {
    nu_allocator_t allocator;
    nu_vector(nu_line_data_t) lines;
    nu_string_t render;
    uint8_t column_count;
} nu_table_printer_data_t;

static void compute_sizes(const nu_line_data_t *line, uint8_t *out_column_count, uint8_t sizes[MAX_COLUMN_COUNT])
{
    NU_ASSERT(line->type == NU_LINE_TYPE_DEFAULT);

    const char *str = line->data;
    const char *end = str + nu_string_size(line->data);
    uint8_t column_count = 0;
    while (true) {

        // Find next separator
        uint32_t len = nu_strfirst(str, end - str, SEPARATOR, strlen(SEPARATOR));
        str += (len + 1);

        // Update size
        if (sizes) sizes[column_count] = NU_MAX(len, sizes[column_count]);

        // Increment column count
        column_count++;

        // End of the line
        if (str == (end + 1)) {
            if (out_column_count) *out_column_count = column_count;
            break;
        }
    }
}
static uint32_t compute_section_width(const nu_line_data_t *start_it)
{
    // Compute max column sizes
    uint8_t max_sizes[MAX_COLUMN_COUNT];
    memset(max_sizes, 0, sizeof(uint8_t) * MAX_COLUMN_COUNT);
    uint8_t column_count;
    for (const nu_line_data_t *it = start_it; it->type != NU_LINE_TYPE_END_SECTION; it++) {
        if (it->type == NU_LINE_TYPE_SEPARATOR) continue;
        compute_sizes(it, &column_count, max_sizes);
    }

    // Compute total width
    uint32_t total_width = 1; // 2 * 2 - 3
    for (uint32_t i = 0; i < column_count; i++) {
        total_width += max_sizes[i] + 3;
    }

    return total_width;
}
static nu_string_t render_separator(
    nu_string_t out, 
    uint8_t prev[MAX_COLUMN_COUNT],
    uint8_t prev_count,
    uint8_t next[MAX_COLUMN_COUNT],
    uint8_t next_count,
    uint32_t table_width,
    bool newline
)
{
    // Allocate line
    uint32_t previous_size = nu_string_size(out);
    for (uint32_t i = 0; i < table_width; i++) {
        nu_string_append(&out, "-");
    }
    if (newline) nu_string_append(&out, "\n");
    out[previous_size] = '+';

    // Render previous
    uint32_t pos = previous_size; 
    for (uint32_t i = 0; i < prev_count; i++) {
        pos += prev[i] + 3;
        out[pos] = '+';
    }

    // Render next
    pos = previous_size; 
    for (uint32_t i = 0; i < next_count; i++) {
        pos += next[i] + 3;
        out[pos] = '+';   
    }

    return out;
}
static nu_string_t render_section(
    nu_string_t out, 
    const nu_line_data_t *start_it, 
    uint8_t sizes[MAX_COLUMN_COUNT],
    uint8_t column_count,
    uint32_t table_width
)
{
    // Render lines
    for (const nu_line_data_t *it = start_it; it->type != NU_LINE_TYPE_END_SECTION; it++) {

        // Render line separator
        if (it->type == NU_LINE_TYPE_SEPARATOR) {
            out = render_separator(out, sizes, column_count, NULL, 0, table_width, true);
            continue;
        }

        // Draw columns
        const char *str = it->data;
        const char *end = str + nu_string_size(it->data);
        uint32_t index = 0;
        while (true) {
            nu_string_append(&out, "| ");
            uint32_t len = nu_strfirst(str, end - str, SEPARATOR, strlen(SEPARATOR));
            if (it->align_center) {
                uint32_t space_count = (sizes[index] - len + 1);
                for (uint8_t i = 0; i < space_count / 2; i++) {
                    nu_string_append(&out, " ");
                }
                nu_string_append_l(&out, str, len);
                for (uint8_t i = 0; i < (space_count / 2) + (space_count % 2); i++) {
                    nu_string_append(&out, " ");
                }
            } else {
                nu_string_append_l(&out, str, len);
                for (uint8_t i = 0; i < (sizes[index] - len + 1); i++) {
                    nu_string_append(&out, " ");
                }
            }
            str += (len + 1);
            index++;
            if (str == (end + 1)) break;
        }
        nu_string_append(&out, "|\n");
    }
    return out;
}
static void render(nu_table_printer_data_t *data)
{
    // Append end line
    nu_line_data_t *end_line = nu_vector_push(&data->lines);
    end_line->type = NU_LINE_TYPE_END_SECTION;

    // Compute table width
    const nu_line_data_t *current = data->lines;
    uint32_t table_width = 0;
    for (const nu_line_data_t *it = data->lines; it != nu_vector_end(data->lines); it++) {
        if (it->type == NU_LINE_TYPE_END_SECTION) {
            // if (*it == SEPARATOR_LINE) continue;
            table_width = NU_MAX(table_width, compute_section_width(current));
            current = it + 1;
        }
    }

    // Render sections
    uint8_t prev_sizes[MAX_COLUMN_COUNT];
    uint8_t prev_count = 0;
    current = data->lines;
    for (const nu_line_data_t *it = data->lines; it != nu_vector_end(data->lines); it++) {
        if (it->type == NU_LINE_TYPE_END_SECTION) {

            // Compute section column sizes
            uint8_t sizes[MAX_COLUMN_COUNT];
            memset(sizes, 0, sizeof(uint8_t) * MAX_COLUMN_COUNT);
            uint8_t column_count;
            for (const nu_line_data_t *sub_it = current; sub_it->type != NU_LINE_TYPE_END_SECTION; sub_it++) {
                if (sub_it->type == NU_LINE_TYPE_SEPARATOR) continue;
                compute_sizes(sub_it, &column_count, sizes);
            }

            // Update the last column size
            uint32_t section_width = compute_section_width(current);
            if (section_width < table_width) {
                sizes[column_count - 1] += (table_width - section_width);
            }

            // Render separator
            data->render = render_separator(data->render, prev_sizes, prev_count, sizes, column_count, table_width, true);
            memcpy(prev_sizes, sizes, column_count * sizeof(uint8_t));
            prev_count = column_count;

            // Render section
            data->render = render_section(data->render, current, sizes, column_count, table_width);

            // Render section
            current = it + 1;
        }
    }

    // Render end separator
    data->render = render_separator(data->render, prev_sizes, prev_count, NULL, 0, table_width, false);
}

nu_table_printer_t nu_table_printer_allocate_(nu_allocator_t a, const char *file, uint32_t line)
{
    nu_table_printer_data_t *data = nu_malloc_(a, sizeof(*data), file, line);
    data->allocator    = a;
    data->lines        = nu_vector_allocate_capacity_(a, sizeof(*data->lines), DEFAULT_LINE_CAPACITY, file, line);
    data->render       = nu_string_allocate_capacity_(a, DEFAULT_RENDER_CAPACITY, file, line);
    data->column_count = 0;
    return (nu_table_printer_t)data;
}
void nu_table_printer_free(nu_table_printer_t t)
{
    nu_table_printer_data_t *data = (nu_table_printer_data_t*)t;
    nu_string_free(data->render);
    for (const nu_line_data_t *it = data->lines; it != nu_vector_end(data->lines); it++) {
        if (it->type == NU_LINE_TYPE_DEFAULT) {
            nu_string_free(it->data);
        }
    }
    nu_vector_free(data->lines);
    nu_free(data->allocator, data);
}
void nu_table_printer_separator(nu_table_printer_t t)
{
    nu_table_printer_data_t *data = (nu_table_printer_data_t*)t;
    nu_line_data_t *line = nu_vector_push(&data->lines);
    line->type = NU_LINE_TYPE_SEPARATOR;
}
static void nu_table_printer_vrow(nu_table_printer_t t, bool align_center, const char *format, va_list args)
{
    nu_table_printer_data_t *data = (nu_table_printer_data_t*)t;

    // Format the complete line
    nu_line_data_t line = {
        .type         = NU_LINE_TYPE_DEFAULT,
        .data         = nu_string_allocate_vformat(data->allocator, format, args),
        .align_center = align_center
    };

    // Compute column count
    uint8_t column_count;
    compute_sizes(&line, &column_count, NULL);

    // Append line
    if (data->column_count != 0 && column_count != data->column_count) {
        nu_line_data_t *end_section = nu_vector_push(&data->lines);
        end_section->type = NU_LINE_TYPE_END_SECTION;
    }
    data->column_count = column_count;
    nu_vector_push_value(&data->lines, line);
}
void nu_table_printer_row(nu_table_printer_t t, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_table_printer_vrow(t, false, format, args);
    va_end(args);
}
void nu_table_printer_row_center(nu_table_printer_t t, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_table_printer_vrow(t, true, format, args);
    va_end(args);
}
const char *nu_table_printer_render(nu_table_printer_t t)
{
    nu_table_printer_data_t *data = (nu_table_printer_data_t*)t;
    render(data);
    return data->render;
}