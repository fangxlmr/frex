/**
 * @file nfa.h
 *
 * @brief 非确定有限状态自动机
 */

#ifndef FREX_NFA_H
#define FREX_NFA_H

/**
 * 定义NFA状态中，除了ASCII码之外的状态值
 */
enum {
    EPSILON = -1,   /* epsilon边 */
    ACCEPT  = -2    /* 接受状态 */
};

/**
 * NFA的状态节点
 *
 * @note
 * 1. 当只有一条向外的指向边时，使用out1，不使用out2
 * 2. lid（list id），生成DFA状态时，用其来区分该NState是否
 *    已经添加进List中，而不需要遍历List。
 */
typedef struct _NState NState;
struct _NState {
    int c;          /* 接收字符c */
    NState *out1;   /* 指向下一个NFA状态节点 */
    NState *out2;   /* 指向下一个NFA状态节点 */
    int lid;        /* list id */
};

/**
 * NFA片段
 */
typedef struct _NFA NFA;
struct _NFA {
    NState *beg;    /* NFA片段开始状态 */
    NState *end;    /* NFA片段结束状态 */
};

#endif /* FREX_NFA_H */
