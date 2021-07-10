#pragma once

typedef struct test_thing thing_t;

struct test_thing {
    thing_t *m_next;
    thing_t *m_prev;
    int prio;
    char *name;
};

static inline thing_t *
thing_head(void **const p_list)
{
    return *p_list;
}

void thing_push(void **p_list, thing_t *const e);
void thing_rem(void **p_list, thing_t *const e);

