#include <stdlib.h>
#include <string.h>
#include "nfa.h"
#include "match.h"

static int listid = 1;
void add_nstate(List *l, NState *ns)
{
    if (ns == NULL || ns->lid == listid) {
        return;
    }
    ns->lid = listid;
    /*
     * 同时添加通过epsilon边
     * 能够达到的状态。
     */
    if (ns->c == EPSILON) {
        add_nstate(l, ns->out1);
        add_nstate(l, ns->out2);
        return;
    }
    l->ns[l->n++] = ns;
}

/*
 * 转移函数delta
 * 计算由clist集合吃进字符c后，可以转换到的nlist集合
 */
List  nlist;    /* 避免每次进入函数都初始化，设置全局变量 */
/* 这里的nl每次都是nlist */
void delta(List *cl, int c, List *nl)
{
    int i;
    NState *ns;

    listid++;   /* 标记NState的状态，是否已经被当前nlist添加 */
    nl->n = 0;   /* 重置nlist */
    for (i = 0; i < cl->n; ++i) {    /* 循环clist中的每一个元素 */
        ns = cl->ns[i];

        if (ns->c == c) {   /* 存在匹配字符c的元素 */
            /*
             * 将ns状态，吃进字符c后，
             * 能够到达的出口状态添加进nlist集合。
             */
            add_nstate(nl, ns->out1);
        }
    }

}

static int ptrcmp(const void *a, const void *b)
{
    if (a < b) {
        return -1;
    } else if (a > b) {
        return 1;
    } else {
        return 0;
    }
}

static int listcmp(List *l1, List *l2)
{
    int i;

    if(l1->n < l2->n)
        return -1;
    if(l1->n > l2->n)
        return 1;
    for(i=0; i<l1->n; i++)
        if(l1->ns[i] < l2->ns[i])
            return -1;
        else if(l1->ns[i] > l2->ns[i])
            return 1;
    return 0;
}


DState *root;
DState *ds_tree(List *l)
{
    int i;
    DState *d, **dp;

    qsort(l->ns, l->n, sizeof(l->ns[0]), ptrcmp);
    dp = &root;
    while ((d = *dp) != NULL) {
        i = listcmp(l, &d->l);
        if (i < 0) {
            dp = &d->left;
        } else if (i > 0) {
            dp = &d->right;
        } else {
            return d;
        }
    }

    /*
     * 树中不存在当前List *l，新建DState并插入
     */
    d = (DState *) malloc(sizeof(*d) + l->n * sizeof(l->ns[0]));
    memset(d, 0, sizeof(*d));
    d->l.ns = (NState **) (d + 1);
    memmove(d->l.ns, l->ns, l->n * sizeof(l->ns[0]));
    d->l.n = l->n;
    *dp = d;

    return d;
}

/*
 * DState *d，吃进字符c之后，
 * 能够达到的下一个DState状态。
 */
DState *next_dstate(DState *d, int c)
{
    delta(&d->l, c, &nlist);

    return d->next[c] = ds_tree(&nlist);
}

/*
 * 判断是否达到接受状态
 */
int is_match(List *l)
{
    int i;

    for (i = 0; i < l->n; ++i) {
        if (l->ns[i]->c == ACCEPT) {
            return 1;
        }
    }
    return 0;
}
/*
 * 开始匹配，运行自动机
 */
extern int NSTATE;
int match(NFA *nfa, char *str)
{
    int c;
    DState *d, *next;

    /*
     * 根据NSTATE初始化nlist变量。
     */
    nlist.ns = (NState **) malloc(NSTATE * sizeof(NState *));
    nlist.n  = 0;

    /*
     * 新建并初始化DState节点d
     */
    add_nstate(&nlist, nfa->start);
    d = ds_tree(&nlist);        /* 初始节点存进二叉树 */

    /*
     * 运行nfa，生成dfa，并同步匹配
     */
    for (; *str; ++str) {
        c = *str;
        if ((next = d->next[c]) == NULL) {
            next = next_dstate(d, c);
        }
        d = next;
    }

    return is_match(&d->l);
}