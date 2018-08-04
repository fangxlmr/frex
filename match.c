#include <stdlib.h>
#include <string.h>
#include "nfa.h"
#include "automata.h"


void delta(List *clist, int c, List *nlist);
DState *next_dstate(DState *d, int c);
int is_match(List *l);
DState *ds_tree(List *l);
void add_nstate(List *l, NState *ns);

static int listid;

/*
 * 开始匹配，运行自动机
 */
List *l1, *l2;
extern int NSTATE;
int match(NFA *nfa, char *s)
{
    int c;
    DState *d, *next;

    /*
     * 初始化初始DState节点d
     */
    d = (DState *) malloc(sizeof(DState));
    memset(d, 1, sizeof(d));
    /*
     * 根据NSTATE初始化clist, nlist变量。
     */
    l1->ns = (NState **) malloc(NSTATE * sizeof(NState *));
    l2->ns = (NState **) malloc(NSTATE * sizeof(NState *));

    /* 初始化clist */
    listid++;
    add_nstate(l1, nfa->start);
    //TODO 添加到二叉树 ds_tree(l1);

    /*
     * 运行nfa，生成dfa，并同步匹配
     */
    for (; *s; ++s) {
        c = *s;
        if ((next = d->next[c]) == NULL) {
            next = next_dstate(d, c);
        }
        d = next;
    }

    return is_match(&d->l);

}


/*
 * 判断是否达到match状态
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
 * DState *d，吃进字符c之后，
 * 能够达到的下一个DState状态。
 */
DState *next_dstate(DState *d, int c)
{
    delta(&d->l, c, l1);


    return ds_tree(&nlist);
}

/*
 * 转移函数delta
 * 计算由clist集合吃进字符c后，可以转换到的nlist集合
 */
void delta(List *clist, int c, List *nlist)
{
    int i;
    NState *ns;

    listid++;   /* 标记NState的状态，是否已经被当前nlist添加 */
    nlist->n = 0;   /* 重置nlist */
    for (i = 0; i < clist->n; ++i) {    /* 循环clist中的每一个元素 */
        ns = clist->ns[i];

        if (ns->c == c) {   /* 存在匹配字符c的元素 */
            /*
             * 将ns状态，吃进字符c后，
             * 能够到达的出口状态添加进nlist集合。
             */
            add_nstate(nlist, ns->out1);
        }
    }

}

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
        add_nstate(l,ns->out2);
        return;
    }
    l->ns[l->n++] = ns;
}
