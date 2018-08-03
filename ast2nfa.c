/*
 * 后续遍历语法树，生成nfa。
 */
#include <stdio.h>
#include <stdlib.h>
#include "ast2nfa.h"

/**
 * state_malloc     新建NState
 *
 * @param ns1       NState结构1的二级指针
 * @param ns2       NState结构2的二级指针
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
 * nfa_atom     创建单个nfa片段
 *
 * @param c     基本字符（ASCII码，或EPSILON）
 * @return      创建成功，则返回nfa指针，失败返回NULL。
 */
static NFA *nfa_atom(int c)
{
    NState *start, *end;
    NFA *nfa;

    nfa = (NFA *) malloc(sizeof(NFA));

    if (state_malloc(&start, &end) && nfa) {
        end->c    = ACCEPT;
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
 * nfa_cat       “与”连接两个nfa片段
 *
 * @param e1    nfa片段1
 * @param e2    nfa片段2
 * @return      连接成功，返回新的nfa片段，否则，返回NULL。
 */
static NFA *nfa_cat(NFA *e1, NFA *e2)
{
    e1->end->c    = EPSILON;
    e1->end->out1 = e2->start;

    e1->end = e2->end;
    return e1;
}

/**
 * nfa_alt      “或”连接两个nfa片段
 *
 * @param e1    nfa片段1
 * @param e2    nfa片段2
 * @return      连连接成功，返回新的nfa片段，否则，返回NULL。
 */
static NFA *nfa_alt(NFA *e1, NFA *e2)
{
    NState *start, *end;

    if (state_malloc(&start, &end)) {
        start->c    = EPSILON;
        start->out1 = e1->start;
        start->out2 = e2->start;

        end->c    = ACCEPT;
        end->out1 = NULL;
        end->out2 = NULL;

        e1->end->c    = EPSILON;
        e1->end->out1 = end;
        e2->end->c    = EPSILON;
        e2->end->out1 = end;

    } else {
        return NULL;
    }

    e1->start = start;
    e1->end   = end;
    return e1;
}

/**
 * nfa_star     “*”闭包连接
 *
 * @param e1    nfa片段
 * @return      连连接成功，返回新的nfa片段，否则，返回NULL。
 */
static NFA *nfa_star(NFA *e)
{
    NState *start, *end;

    if (state_malloc(&start, &end)) {
        end->c    = ACCEPT;
        end->out1 = NULL;
        end->out2 = NULL;

        start->c    = EPSILON;
        start->out1 = e->start;
        start->out2 = end;

        e->end->c    = EPSILON;
        e->end->out1 = end;
        e->end->out2 = e->start;
    } else {
        return NULL;
    }

    e->start = start;
    e->end   = end;
    return e;
}

/*
 * 后序遍历语法树，递归构造nfa。
 */

NFA *ast2nfa(AST *ast)
{
    int c;
    NFA *left, *right;

    if (ast == NULL) {
        return NULL;
    }

    if (ast->k == AST_CHAR) {
        c = ((AST_Char *) ast)->c;
        return nfa_atom(c);
    } else if (ast->k == AST_ALT) {
        left  = ast2nfa(((AST_Alt *) ast)->left);
        right = ast2nfa(((AST_Alt *) ast)->right);
        return nfa_alt(left, right);
    } else if (ast->k == AST_CAT) {
        left  = ast2nfa(((AST_Cat *) ast)->left);
        right = ast2nfa(((AST_Cat *) ast)->right);
        return nfa_cat(left, right);
    } else {
        printf("ast2nfa.\n");
        left = ast2nfa(((AST_Star *) ast)->next);
        return nfa_star(left);
    }
}