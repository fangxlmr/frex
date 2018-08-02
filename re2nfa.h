#ifndef FREX_RE2NFA_H
#define FREX_RE2NFA_H

enum {
    Epsilon = -1,   /* epsilon边 */
    Accept  = -2    /* 接受状态 */
};

/**
 * NFA的状态节点
 */
typedef struct _NState NState;
struct _NState {
    int c;          /* 输入字符c */
    NState *out1;   /* 指向下一个NFA状态节点 */
    NState *out2;   /* 指向下一个NFA状态节点 */
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

extern NFA *re2nfa(char *s);

#endif /* FREX_RE2NFA_H */
