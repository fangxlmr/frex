#ifndef FREX_NFA_H
#define FREX_NFA_H

enum {
    EPSILON = -1,   /* epsilon边 */
    ACCEPT  = -2    /* 接受状态 */
};

/**
 * NFA的状态节点
 */
typedef struct _NState NState;
struct _NState {
    int c;          /* 输入字符c */
    NState *out1;   /* 指向下一个NFA状态节点 */
    NState *out2;   /* 指向下一个NFA状态节点 */
    int lid;        /* list id */
    /*
     * 当只有一条向外的指向边时，使用out1，不使用out2
     */
};

/**
 * NFA片段
 */
typedef struct _NFA NFA;
struct _NFA {
    NState *start;  /* NFA片段开始状态 */
    NState *end;    /* NFA片段结束状态 */
};

#endif /* FREX_NFA_H */
