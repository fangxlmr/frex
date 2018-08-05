/**
 * @file match.h
 *
 * @brief 运行自动机匹配字符串
 *
 * 使用match函数，运行NFA，同步生成DFA，匹配字符串。
 */
#ifndef FREX_MATCH_H
#define FREX_MATCH_H

#include "nfa.h"

/**
 * match        匹配函数
 *
 * @param nfa   NFA片段
 * @param str   带匹配字符串
 *
 * @return      若字符串str满足NFA，则返回非0值，
 *              否则，返回0。
 */
extern int match(NFA *nfa, char *str);
#endif /* FREX_MATCH_H */
