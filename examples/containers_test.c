#include <stdlib.h>
#include <math.h>

#include <nucleus/nucleus.h>

#define MAIN_LOGGER_NAME "MAIN"

typedef struct {
    uint32_t id;
    uint32_t v;
} stest;
static uint32_t nu_random()
{
    static uint32_t next = 1231125;
    next = (unsigned int)(next * 123423542) % 23423425;
    return next;
}
static nu_result_t on_start(void)
{
    /* test nu_array_swap */
    {
        nu_array_t ar;
        uint32_t val;
        nu_array_allocate(&ar, sizeof(uint32_t));
        val = 0; nu_array_push(ar, &val);
        val = 1; nu_array_push(ar, &val);
        val = 2; nu_array_push(ar, &val);
        val = 3; nu_array_push(ar, &val);
        val = 4; nu_array_push(ar, &val);
        val = 5; nu_array_push(ar, &val);
        val = 6; nu_array_push(ar, &val);
        nu_array_swap(ar, 5, 6);
        uint32_t *value;
        nu_array_get(ar, 5, &value);
        NU_ASSERT(*value == 6);
        nu_array_get(ar, 6, &value);
        NU_ASSERT(*value == 5);
        nu_array_free(ar);
    }
    
    /* test nu_indexed_array_t */
    {
        nu_indexed_array_t iar;
        nu_array_t ids;
        stest s;
        nu_indexed_array_allocate(&iar, sizeof(uint32_t));
        nu_array_allocate(&ids, sizeof(stest));

        for (uint32_t i = 0; i < 100000; i++) {
            if (nu_random() % 3 != 0) {
                s.v = nu_random() % 1000;
                nu_indexed_array_add(iar, &s.v, &s.id);
                nu_array_push(ids, &s);
            } else {
                uint32_t len = nu_array_get_size(ids);
                if (len > 0) {
                    nu_array_swap_last(ids, nu_random() % len);
                    stest s;
                    stest *ps; nu_array_get_last(ids, &ps);
                    s = *ps;
                    nu_array_pop(ids);

                    uint32_t *pv; nu_indexed_array_get(iar, s.id, &pv);
                    uint32_t v = *pv;
                    NU_ASSERT(s.v == v);
                    nu_indexed_array_remove(iar, s.id);
                }
            }
        }

        nu_indexed_array_free(iar);
        nu_array_free(ids);
    }

    /* test hashmap */
    {
        
    }

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