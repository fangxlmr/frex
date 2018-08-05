/**
 * @file parser.h
 *
 * @brief   语法分析器
 *
 * 语法分析器（parser）是接收词法分析器（lexer）输出的记号（token），
 * （实际上是parser驱动lexer），对正则表达式字符串进行语法分析，判断语法是否合法。
 * 若语法合法，则输出抽象语法树（AST：Abstract Syntax Tree）。
 *
 * 使用re2ast函数，处理re字符串，输出AST。
 *
 * @note
 * 正则文法的CFG（上下文无关文法）表示，仅支持：*|()
 * re -> ε
 *     | char
 *     | re "|" re
 *     | re re
 *     | re "*"
 *     | "(" re ")"
 *
 * re -> union | simple
 * union -> re "|" simple
 * simple -> concatenation | basic
 * concatenation -> simple basic
 * basic -> star | element
 * star -> element "*"
 * element -> group | char
 * group -> "(" re ")"
 * char -> any-non-metacharacter | "\"
 *
 * 消除左递归
 * re -> simple union
 * union -> "|" simple union | ε
 * simple -> basic concatenation
 * concatenation -> basic concatenation | ε
 * basic -> star | element
 * star -> element "*"
 * element -> group | char
 * group -> "(" re ")"
 * char -> any-non-metachar | "\"
 */
#ifndef FREX_PARSER_H
#define FREX_PARSER_H

#include "lexer.h"
#include "ast.h"

/**
 * re2ast       语法分析，生成语法树
 *
 * @param re    正则表达式
 * @return      若分析成功，则返回AST，否则报出合适错误，程序终止。
 */
extern AST *re2ast(const char *re);
#endif /* FREX_PARSER_H */
