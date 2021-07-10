#pragma once

#include <stdlib.h>
#include <stdbool.h>

typedef int (*pgetp_t)(void const*);
typedef void (*psetnext_t)(void *, void *);
typedef void (*psetprev_t)(void *, void *);
typedef void* (*pgetnext_t)(void *);
typedef void* (*pgetprev_t)(void *);

__attribute__((always_inline))
static inline void
pl_push(void **const p_list, void *const obj,
    pgetp_t const getp,
    pgetnext_t const getnext, pgetprev_t const getprev,
    psetnext_t const setnext, psetprev_t const setprev)
{
    void *prev = NULL; // points to head of previous priority level
    void *insertion = *p_list;
    int const iprio = getp(obj);

    bool add_to_prev = false;
    bool add_to_tail = false;

    while (insertion != NULL) {
        int const cprio = getp(insertion);
        if (cprio < iprio) {
            add_to_prev = true;
            break;
        } else if (cprio == iprio) {
            add_to_tail = true;
            break;
        } else {
            prev = insertion;
            void *const tail = getprev(insertion);
            insertion = getnext(tail);
        }
    }

    if (insertion == NULL) {
        // new priority level at end
        setnext(obj, NULL);
        setprev(obj, obj);
        if (prev == NULL) {
            *p_list = obj;
        } else {
            void *const tail = getprev(prev);
            setnext(tail, obj);
        }
    } else {
        if (add_to_tail) {
            // insertion is the same prio
            void *oldtail = getprev(insertion);
            setprev(insertion, obj); // set obj as new tail
            setprev(obj, oldtail);
            setnext(obj, getnext(oldtail));
            setnext(oldtail, obj);
        } else if (add_to_prev) {
            // New priority level inbetween prev and insertion
            setprev(obj, obj);
            if (prev == NULL) {
                setnext(obj, insertion);
                *p_list = obj;
            } else {
                void *prevtail = getprev(prev);
                setnext(obj, getnext(prevtail));
                setnext(prevtail, obj);
            }
        } else {
            // insertion is a higher priority, we need to make a new priority
            void *tail = getprev(insertion);
            setnext(obj, NULL);
            setprev(obj, obj);
            setnext(tail, obj);
        }
    }
}

__attribute__((always_inline))
static inline void
pl_rem(void **const p_list, void *const obj,
    pgetp_t const getp,
    pgetnext_t const getnext, pgetprev_t const getprev,
    psetnext_t const setnext, psetprev_t const setprev)
{
    void *prev = NULL; // points to head of previous priority level
    void *curr = *p_list;
    int const iprio = getp(obj);

    for (;;) {
        int const cprio = getp(curr);
        if (cprio == iprio) {
            break;
        } else {
            prev = curr;
            void *const tail = getprev(curr);
            curr = getnext(tail);
        }
    }

    bool const obj_is_head = curr == obj;
    bool const obj_is_tail = getprev(curr) == obj;
    if (!obj_is_head) {
        // Easy cases!
        if (!obj_is_tail) {
            // Neither head nor tail, the current priority level has at least 2
            // more things.
            void *const prev_node = getprev(obj);
            void *const next_node = getnext(obj);
            setnext(prev_node, next_node);
            setprev(next_node, prev_node);
        } else {
            // not head, but tail, the current priority level has at least 1
            // more thing in it.
            void *const new_tail = getprev(obj);
            setprev(curr, new_tail);
            setnext(new_tail, getnext(obj));
        }

    } else {
        // We are the head of a priority level
        void *const new_head = getnext(obj);
        if (prev == NULL) {
            *p_list = new_head;
        } else {
            void *const prevtail = getprev(prev);
            setnext(prevtail, new_head);
        }

        if (!obj_is_tail) {
            // If there was another node after us in this priority level, we
            // must make it into a head.
            setprev(new_head, getprev(obj));
        }
    }
}

