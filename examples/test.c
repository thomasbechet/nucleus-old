#include <nucleus/nucleus.h>

#define MAIN_LOGGER_NAME "MAIN"

static nu_result_t on_start(void)
{
    nu_vector(uint32_t) vec;

    nu_vector_allocate(&vec);
    for (uint32_t i = 0; i < 10; i++) {
        nu_vector_push_value(&vec, i);
    }
    nu_vector_erase(vec, 9);
    for (uint32_t i = 0; i < nu_vector_get_size(vec); i++) {
        nu_info("test", "%d", vec[i]);
    }
    nu_vector_free(vec);

    /* exit */
    nu_context_request_stop();

    return NU_SUCCESS;
}

int main(int argc, char *argv[])
{
    nu_context_init_info_t info;
    memset(&info, 0, sizeof(nu_context_init_info_t));
    info.callback.start  = on_start;
    nu_context_init(&info);

    return 0;
}