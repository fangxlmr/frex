#ifndef FREX_PARSER_H
#define FREX_PARSER_H

/*
 * 正则文法，仅支持：*|
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

#include "lexer.h"
#include "ast.h"

extern AST *re2ast(const char *re);
#endif /* FREX_PARSER_H */
