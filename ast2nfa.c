/*
 * ast2nfa的代码实现。
 */

#include <stdlib.h>
#include "ast2nfa.h"

/*
 * 全局变量NSTATE，
 * 记录总共产生的NState数量
 */
int NSTATE;

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
        NSTATE += 2;    /* NState数量+2 */
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
    NState *beg, *end;
    NFA *e;

    e = (NFA *) malloc(sizeof(NFA));

    if (state_malloc(&beg, &end) && e) {
        end->c    = ACCEPT;
        end->lid  = 0;
        end->out1 = NULL;
        end->out2 = NULL;

        beg->c    = c;
        beg->lid  = 0;
        beg->out1 = end;
        beg->out2 = NULL;

        e->beg = beg;
        e->end = end;

    } else {
        free(e);
    }

    return e;
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
    if ((e1 && e2) == 0) {
        return NULL;
    }

    e1->end->c    = EPSILON;
    e1->end->out1 = e2->beg;
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
    NState *beg, *end;

    if ((e1 && e2) == 0) {
        return NULL;
    }

    if (state_malloc(&beg, &end)) {
        beg->c    = EPSILON;
        beg->lid  = 0;
        beg->out1 = e1->beg;
        beg->out2 = e2->beg;

        end->c    = ACCEPT;
        end->lid  = 0;
        end->out1 = NULL;
        end->out2 = NULL;

        e1->end->c    = EPSILON;
        e1->end->out1 = end;
        e2->end->c    = EPSILON;
        e2->end->out1 = end;

    } else {
        return NULL;
    }

    e1->beg = beg;
    e1->end = end;
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
    NState *beg, *end;

    if (!e) {
        return NULL;
    }

    if (state_malloc(&beg, &end)) {
        end->c    = ACCEPT;
        end->lid  = 0;
        end->out1 = NULL;
        end->out2 = NULL;

        beg->c    = EPSILON;
        beg->lid  = 0;
        beg->out1 = e->beg;
        beg->out2 = end;

        e->end->c    = EPSILON;
        e->end->out1 = end;
        e->end->out2 = e->beg;
    } else {
        return NULL;
    }

    e->beg = beg;
    e->end = end;
    return e;
}

/*
 * 后序遍历AST，递归构造NFA。
 */
NFA *ast2nfa(AST *ast)
{
    int c;
    NFA *left, *right;

    if (ast == NULL) {
        return NULL;
    }

    /*
     * 后续遍历时，根绝Kind k域的值，判断后续数据域
     * 的类型，并将该节点强制转换为相应的数据类型。
     * 再使用AST中相应节点的构造函数，递归构造出NFA。
     *
     * if-else代码块中的分支，对应与ast.h中定义的
     * 运算种类。
     */
    if (ast->k == AST_CHAR) {       /* 字符类型 */
        c = ((AST_Char *) ast)->c;
        return nfa_atom(c);

    } else if (ast->k == AST_ALT) {     /* 选择算符 */
        left  = ast2nfa(((AST_Alt *) ast)->left);
        right = ast2nfa(((AST_Alt *) ast)->right);
        return nfa_alt(left, right);

    } else if (ast->k == AST_CAT) {     /* 连接算符 */
        left  = ast2nfa(((AST_Cat *) ast)->left);
        right = ast2nfa(((AST_Cat *) ast)->right);
        return nfa_cat(left, right);

    } else {        /* “*”闭包算符 */
        left = ast2nfa(((AST_Star *) ast)->next);
        return nfa_star(left);
    }
}
