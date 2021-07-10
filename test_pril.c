#include <stdlib.h>
#include <stdio.h>

#include "pril.h"

typedef struct test_thing thing_t;

struct test_thing {
    thing_t *m_next;
    thing_t *m_prev;
    int prio;
    char *name;
};

__attribute__((always_inline))
static inline int
thing_getp(void const*const arg)
{
    thing_t const*const p = arg;
    return p->prio;
}

__attribute__((always_inline))
static inline void *
thing_getprev(void *const arg)
{
    thing_t *const p = arg;
    return p->m_prev;
}

__attribute__((always_inline))
static inline void *
thing_getnext(void *const arg)
{
    thing_t *const p = arg;
    return p->m_next;
}

__attribute__((always_inline))
static inline void
thing_setprev(void *const arg, void *const e)
{
    thing_t *const p = arg;
    p->m_prev = e;
}

__attribute__((always_inline))
static inline void
thing_setnext(void *const arg, void *const e)
{
    thing_t *const p = arg;
    p->m_next = e;
}

static inline void
thing_push(void **p_list, thing_t *const e)
{
    pl_push(p_list, e,
        thing_getp,
        thing_getnext, thing_getprev,
        thing_setnext, thing_setprev);
}

static inline void
thing_rem(void **p_list, thing_t *const e)
{
    pl_rem(p_list, e,
        thing_getp,
        thing_getnext, thing_getprev,
        thing_setnext, thing_setprev);
}

static inline thing_t *
thing_head(void **p_list)
{
    return *p_list;
}

static void
print_queue(void *p_list)
{
    thing_t *c = thing_head(&p_list);

    while (c != NULL) {
        printf("Node: %s (%p)\n\tm_prev = %p\n\tm_next = %p\n\tprio = %d\n", c->name, c, c->m_prev, c->m_next, c->prio);
        c = c->m_next;
    }
}

int
main(void)
{

    void *list = NULL;

    thing_t *t = malloc(sizeof(*t));
    t->name = "t";
    t->prio = 12;
    thing_t *x = malloc(sizeof(*t));
    x->name = "x";
    x->prio = 12;
    thing_t *y = malloc(sizeof(*t));
    y->name = "y";
    y->prio = 8;
    thing_t *z = malloc(sizeof(*t));
    z->name = "z";
    z->prio = 14;
    thing_t *v = malloc(sizeof(*t));
    v->name = "v";
    v->prio = 10;

    thing_push(&list, t);
    thing_push(&list, x);
    thing_push(&list, y);
    thing_push(&list, z);
    thing_push(&list, v);

    print_queue(list);

    printf("removing!!\n\n");

    thing_rem(&list, v);

    print_queue(list);

    printf("removing!!\n\n");

    thing_rem(&list, z);

    print_queue(list);

    printf("removing!!\n\n");

    thing_rem(&list, t);
    thing_rem(&list, y);
    thing_rem(&list, x);

    print_queue(list);

    free(t);
    free(x);
    free(y);
    free(z);
    free(v);

    return 0;
}
