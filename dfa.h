/**
 * @file dfa.h
 *
 * @brief 确定有限状态自动机
 */

#ifndef FREX_DFA_H
#define FREX_DFA_H

#include "nfa.h"

/**
 * DState中的NState集合
 */
typedef struct _List {
    NState **ns;    /* NState *数组 */
    int n;    /* ns数组中的元素数量 */
}List;

/**
 * DFA的状态节点结构
 *
 * @note
 * 1. List l 是DState中需要保存的若干个NState集合；
 *
 * 2. 针对ASCII码，确定next数组，也即DState将沿着next数组推进。
 *    DState d，在读取字符c时，能够推进到的下一个DState为 d->next[c]，
 *    若d->next[c]为空（NULL），说明下一个DState状态还没有被计算。
 *
 * 3. left与right指针是为了将已经计算出的DState存储进二叉树中，以便后续使用。
 *
 * 4. List l是能够唯一确定DState的KEY值，相同的List l集合的DState是相同的。
 *    注意：这里的List l应该是无序的。可以将List l排序后判断相等，
 *    或者使用MD5算法，计算List l的唯一性。
 */
typedef struct _DState DState;
struct _DState {
    List l;
    DState *next[256];
    DState *left;
    DState *right;
};

#endif /* FREX_DFA_H */
