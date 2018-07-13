#include <stdlib.h>
#include "re2nfa.h"

/**
 * state_malloc     新建NState
 *
 * @param ns1       NState结构-1的二级指针
 * @param ns2       NState结构-2的二级指针
 * @return          新建成功，返回1；失败返回0，并释放已分配的内存。
 */
static int state_malloc(NState **ns1, NState **ns2)
{
    *ns1 = (NState *) malloc(sizeof(NState));
    *ns2 = (NState *) malloc(sizeof(NState));

    if (*ns1 && *ns2) {
        return 1;
    } else {
        free(*ns1);
        free(*ns2);
        return 0;
    }
}

/**
 * nfa          创建单个nfa片段
 *
 * @param c     基本字符（ASCII码，或Epsilon）
 * @return      创建成功，则返回nfa指针，失败返回NULL。
 */
static NFA *nfa(int c)
{
    NState *start, *end;
    NFA *nfa;

    nfa = (NFA *) malloc(sizeof(NFA));

    if (state_malloc(&start, &end) && nfa) {
        end->c    = Accept;
        end->out1 = NULL;
        end->out2 = NULL;

        start->c    = c;
        start->out1 = end;
        start->out2 = NULL;

        nfa->start = start;
        nfa->end   = end;

    } else {
        free(nfa);
    }

    return nfa;
}

/**
 * cat          “与”连接两个nfa片段
 *
 * @param e1    nfa片段-1
 * @param e2    nfa片段-2
 * @return      连接成功，返回新的nfa片段，否则，返回NULL。
 */
static NFA *cat(NFA *e1, NFA *e2)
{
    e1->end->c    = Epsilon;
    e1->end->out1 = e2->start;

    e1->end = e2->end;
    return e1;
}

/**
 * alt          “或”连接两个nfa片段
 *
 * @param e1    nfa片段-1
 * @param e2    nfa片段-2
 * @return      连连接成功，返回新的nfa片段，否则，返回NULL。
 */
static NFA *alt(NFA *e1, NFA *e2)
{
    NState *start, *end;

    if (state_malloc(&start, &end)) {
        start->c    = Epsilon;
        start->out1 = e1->start;
        start->out2 = e2->start;

        end->c    = Accept;
        end->out1 = NULL;
        end->out2 = NULL;

        e1->end->c    = Epsilon;
        e1->end->out1 = end;
        e2->end->c    = Epsilon;
        e2->end->out1 = end;

    } else {
        return NULL;
    }

    e1->start = start;
    e1->end   = end;
    return e1;
}

/**
 * closure      闭包连接
 *
 * @param e1    nfa片段
 * @return      连连接成功，返回新的nfa片段，否则，返回NULL。
 */
static NFA *closure(NFA *e)
{
    NState *start, *end;

    if (state_malloc(&start, &end)) {
        end->c    = Accept;
        end->out1 = NULL;
        end->out2 = NULL;

        start->c    = Epsilon;
        start->out1 = e->start;
        start->out2 = end;

        e->end->c    = Epsilon;
        e->end->out1 = end;
        e->end->out2 = e->start;
    } else {
        return NULL;
    }

    e->start = start;
    e->end   = end;
    return e;
}

NFA *re2nfa(char *s)
{

}