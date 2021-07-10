#include <stdlib.h>
#include <stdio.h>

#include "test_helper.h"

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
