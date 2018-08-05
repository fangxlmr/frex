/**
 * @file ast2nfa.h
 *
 * @brief 抽象语法树转换为NFA
 *
 */
#ifndef FREX_AST2NFA_H
#define FREX_AST2NFA_H

#include "nfa.h"
#include "ast.h"
/**
 * 全局变量 NSTATE，
 * 记录AST生成的NFA中状态点的数量。
 */
extern int NSTATE;

/**
 * ast2nfa      AST转NFA
 *
 * @param ast   抽象语法树
 * @return      返回NFA片段
 */
extern NFA *ast2nfa(AST *ast);
#endif /* FREX_AST2NFA_H */
