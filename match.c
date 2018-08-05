/*
 * match函数运行自动机的代码实现
 */
#include <stdlib.h>
#include <string.h>
#include "dfa.h"
#include "match.h"

/*
 * 生成DState时，用来区分该NState是否已经添加进当前
 * List，每次进入转移函数delta，listid++
 */
static int listid = 1;

/**
 * add_nstate   将NState添加进集合List
 *
 * @param l     集合List
 * @param ns    NState
 */
static void add_nstate(List *l, NState *ns)
{
    /*
     * 根据NState中的lid与当前循环中的listid是否
     * 相等，判断是否添加
     */
    if (ns == NULL || ns->lid == listid) {
        return;
    }
    /* 未加入List的NState，添加后修改lid */
    ns->lid = listid;

    /*
     * 同时递归添加通过epsilon边
     * 能够达到的状态。
     */
    if (ns->c == EPSILON) {
        add_nstate(l, ns->out1);
        add_nstate(l, ns->out2);
        return;
    }
    /* 添加当前NState进入List集合 */
    l->ns[l->n++] = ns;
}

/**
 * delta        转移函数
 *
 * @param cl    当前NState集合（clist）
 * @param c     字符c
 * @param nl    接收字符c后可能转换到的NState集合
 *
 * @note
 * 1. 计算由clist集合吃进字符c后，可以转换到的nlist集合
 * 2. 避免每次进入函数都初始化nl，设置全局变量nlist，
 *    nlist在match函数中分配内存并初始化。
 *    因此，这里的nl总是等于&nlist。
 */
List nlist;
void delta(List *cl, int c, List *nl)
{
    int i;
    NState *ns;

    /*
     * 标记当前List的状态，
     * 是NState是否已经添加进nlist的依据。
     */
    listid++;
    /* 重置nlist */
    nl->n = 0;

    /* 循环clist中的每一个元素 */
    for (i = 0; i < cl->n; ++i) {
        ns = cl->ns[i];

        if (ns->c == c) {   /* ns接收字符c */
            /*
             * 将ns吃进字符c后，
             * 能够到达的状态添加进nlist集合。
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


/**
 * 二叉树，用于存储已经计算出的DState
 * 注意：List l能够唯一确定一个DState。
 */
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

/**
 * next_dstate  下一个DState
 *
 * @param d     当前DState
 * @param c     输入字符c
 *
 * @return      返回当前DState接收字符c后能够达到的DState。
 * //TODO 当不存在DState时，未处理，
 * //TODO 包括delta函数和ds_tree都没有处理这个问题
 */
DState *next_dstate(DState *d, int c)
{
    delta(&d->l, c, &nlist);

    return d->next[c] = ds_tree(&nlist);
}

/**
 * is_match     是否为接受状态
 *
 * @param l     NState集合List
 * @return      若l中含有ACCEPT的NState，则返回1，
 *              否则，返回0。
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

extern int NSTATE;  /* 生成的NState数量，在ast2nfa.h中定义 */
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
     * 新建初始DState节点d
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