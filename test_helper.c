
#include "test_helper.h"

#include "pril.h"

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
static inline void **
thing_getprevr(void *const arg)
{
    thing_t *const p = arg;
    return (void **)&p->m_prev;
}

__attribute__((always_inline))
static inline void **
thing_getnextr(void *const arg)
{
    thing_t *const p = arg;
    return (void **)&p->m_next;
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

void
thing_push(void **p_list, thing_t *const e)
{
    pl_push(p_list, e, false,
        thing_getp,
        thing_getnextr, thing_getprevr);
}

void
thing_pushleft(void **p_list, thing_t *const e)
{
    pl_push(p_list, e, true,
        thing_getp,
        thing_getnextr, thing_getprevr);
}

void
thing_rem(void **p_list, thing_t *const e)
{
    pl_rem(p_list, e,
        thing_getp,
        thing_getnext, thing_getprev,
        thing_setnext, thing_setprev);
}
