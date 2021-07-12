#pragma once

#include <stdlib.h>
#include <stdbool.h>

typedef int (*pgetp_t)(void const*);
typedef void (*psetnext_t)(void *, void *);
typedef void (*psetprev_t)(void *, void *);

typedef void* (*pgetnext_t)(void *);
typedef void* (*pgetprev_t)(void *);

typedef void** (*pgetnextr_t)(void *);
typedef void** (*pgetprevr_t)(void *);

__attribute__((always_inline))
static inline void
pl_push(void **const p_list, void *const obj,
    bool const left,
    pgetp_t const getp,
    pgetnextr_t const getnextr, pgetprevr_t const getprevr)
{
    void **prevp = p_list;  // Pointer that points to curr
    void *curr = *prevp;    // Points to head of current priority level

    int const iprio = getp(obj);

    while (curr != NULL) {
        int const cprio = getp(curr);
        if (cprio == iprio) {

            void *const curr_tail = *getprevr(curr);
            if (left) {
                *prevp = obj;
                *getprevr(obj) = curr_tail; // Make obj a head node
                *getnextr(obj) = curr;
                *getprevr(curr) = obj;      // Make curr _not_ a head node.
            } else {
                *getprevr(curr) = obj;      // make us a tail node
                *getprevr(obj) = curr_tail; // set our prev to the current tail
                void *const nextp = *getnextr(curr_tail); // get the node after the current tail
                *getnextr(obj) = nextp;     // point to it
                *getnextr(curr_tail) = obj; // current tail points to us
            }

            return;
        } else if (cprio < iprio) {
            // We need to create a new priority level
            break;
        } else {
            void *const curr_tail = *getprevr(curr);
            prevp = getnextr(curr_tail);
            curr = *prevp;
        }
    }

    *prevp = obj;
    *getprevr(obj) = obj;
    *getnextr(obj) = curr;
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

    bool obj_is_tail = false;
    bool obj_is_head = false;
    {
        // See if our node is a tail
        void const*const maybe_next_prio = getnext(obj);
        obj_is_tail = (maybe_next_prio == NULL) || (getp(maybe_next_prio) != iprio);

        // See if our node is a head
        void *const maybe_tail = getprev(obj);
        obj_is_head = (getnext(maybe_tail) != obj);
    }

    if (!obj_is_head && !obj_is_tail) {
        // This is probably the most common case (?)

        // Neither head nor tail, the current priority level has at least 2
        // more things.
        void *const prev_node = getprev(obj);
        void *const next_node = getnext(obj);
        setnext(prev_node, next_node);
        setprev(next_node, prev_node);
        return;
    }

    // If the node we're removing is a head or tail, we need to get pointers to
    // the current priority level head and the previous priority level head.

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

    if (!obj_is_head) {
        // not head, but tail, the current priority level has at least 1 more
        // thing in it.
        void *const new_tail = getprev(obj);
        setprev(curr, new_tail);
        setnext(new_tail, getnext(obj));

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

