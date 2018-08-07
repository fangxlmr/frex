/*
 * 词法分析器的代码实现。
 */

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

/* 内部全局变量，存储lexer的对re字符串的解析位置 */
static int _RE_INDEX = 0;
/* 内部全局变量，存储lexer解析的字符串 */
static const char *RE = NULL;

void set_re(const char *re)
{
    RE = re;
}

char *get_re()
{
    return (char *) RE;
}

int getp()
{
    return _RE_INDEX;
}

int step(int c)
{
    if (*(RE + _RE_INDEX) == c) {
        ++_RE_INDEX;
        return 1;
    } else {
        return 0;
    }
}

void rollback()
{
    --_RE_INDEX;
}

/**
 * next_char     从RE字符串中获取下一个字符
 *
 * @return      返回下一个字符
 */
static char next_char()
{
    return *(RE + _RE_INDEX++);
}

char get_char()
{
    return *(RE + _RE_INDEX);
}

Token *next_token()
{
    int c;
    Token *token;

    token = (Token *) malloc(sizeof(Token *));
    if (!token) {
        printf("Malloc failed.\n");
        return NULL;
    }

    c = next_char();
    switch (c) {
        /* 非元字符，Non-metachar */
        default:
            token->t = OTHER;
            token->c = c;
            break;

        /* 结束符 */
        case '\0':
            token->t = END;
            token->c = '\0';
            rollback();
            break;

        /* 元字符 */
        case '|':
        case '*':
        case '(':
        case ')':
            token->t = METACHAR;
            token->c = c;
            break;

        /* 转义字符 */
        case '\\':
            c = next_char();
            switch(c) {
                case '|':
                case '*':
                case '(':
                case ')':
                case '\\':
                    token->t = ESCAPE;
                    token->c = c;
                    break;
                default:
                    token->t = METACHAR;
                    token->c = c;
                    rollback();
                    break;
            }
            break;
    }
    return token;
}
