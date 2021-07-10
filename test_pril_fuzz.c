#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "test_helper.h"

static inline unsigned
xorshift32(unsigned *const rng_state)
{
    unsigned x = *rng_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *rng_state = x;
    return *rng_state;
}

#define NUM_OBJS 10000
#define NUM_PRIOS 32

int
main(void)
{
    int status = 0;
    printf("Starting!\n");


    unsigned rng = time(NULL);
    for (int i = 0; i < (getpid() & 0xfff); ++i) {
        (void)xorshift32(&rng);
    }

    void *list = NULL;

    {
        for (int i = 0; i < NUM_OBJS; ++i) {
            thing_t *obj = malloc(sizeof(*obj));
            int const prio = xorshift32(&rng) % NUM_PRIOS;
            obj->prio = prio;
            thing_push(&list, obj);
        }

        while (list != NULL) {
            // Remove the head of the list each time.
            thing_t *obj = thing_head(&list);
            thing_rem(&list, obj);
            memset(obj, 0, sizeof(*obj));
            free(obj);
        }
    }

    {
        for (int i = 0; i < NUM_OBJS; ++i) {
            thing_t *obj = malloc(sizeof(*obj));
            int const prio = xorshift32(&rng) % NUM_PRIOS;
            obj->prio = prio;
            thing_push(&list, obj);
        }

        while (list != NULL) {
            // Always remove a middle node instead, if possible.
            thing_t *obj = thing_head(&list);
            thing_t *next_node = obj->m_next;
            if (next_node != NULL) {
                thing_rem(&list, next_node);
                memset(next_node, 0, sizeof(*next_node));
                free(next_node);
            } else {
                thing_rem(&list, obj);
                memset(obj, 0, sizeof(*obj));
                free(obj);
            }
        }
    }

    {
        for (int i = 0; i < NUM_OBJS; ++i) {
            thing_t *obj = malloc(sizeof(*obj));
            int const prio = xorshift32(&rng) % NUM_PRIOS;
            obj->prio = prio;
            thing_push(&list, obj);
        }

        while (list != NULL) {
            thing_t *obj = thing_head(&list);

            // Get the tail of the current priority level and remove it instead.
            thing_t *tail = obj->m_prev;
            thing_rem(&list, tail);
            memset(tail, 0, sizeof(*obj));
            free(tail);
        }
    }

    {
        for (int i = 0; i < NUM_OBJS; ++i) {
            thing_t *obj = malloc(sizeof(*obj));
            int const prio = xorshift32(&rng) % NUM_PRIOS;
            obj->prio = prio;
            thing_push(&list, obj);
        }

        while (list != NULL) {
            thing_t *obj = thing_head(&list);

            // Get the head of next priority level and remove it instead.
            thing_t *tail = obj->m_prev;
            thing_t *next_head = tail->m_next;
            if (next_head != NULL) {
                thing_rem(&list, next_head);
                memset(next_head, 0, sizeof(*obj));
                free(next_head);
            } else {
                thing_rem(&list, tail);
                memset(tail, 0, sizeof(*obj));
                free(tail);
            }
        }
    }

    printf("Ending!\n");

    return 0;
}
