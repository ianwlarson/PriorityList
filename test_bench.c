#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>
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
    void *list = NULL;
    printf("Starting!\n");


    unsigned rng = time(NULL);
    for (int i = 0; i < (getpid() & 0xfff); ++i) {
        (void)xorshift32(&rng);
    }

    thing_t *objs = malloc(sizeof(*objs) * NUM_OBJS);
    for (int i = 0; i < NUM_OBJS; ++i) {
        int const prio = xorshift32(&rng) % NUM_PRIOS;
        objs[i].prio = prio;
    }

    struct timespec start,end;

    status = clock_gettime(CLOCK_REALTIME, &start);
    assert(status == 0);
    for (int i = 0; i < 1000; ++i) {

        for (int j = 0; j < NUM_OBJS; ++j) {
            thing_push(&list, &objs[j]);
        }

        while (list != NULL) {
            thing_t *const obj = thing_head(&list);
            thing_rem(&list, obj);
        }
    }
    status = clock_gettime(CLOCK_REALTIME, &end);
    assert(status == 0);

    uint64_t time_diff = end.tv_sec - start.tv_sec;
    time_diff *= 1000000000;
    time_diff += end.tv_nsec - start.tv_nsec;

    printf("Time difference was %"PRIu64" nanoseconds\n", time_diff);

    printf("Ending!\n");

    return 0;
}
