/*
 * 语法分析器（parser）的代码实现。
 *
 * 值得注意的是，这里是使用“递归下降分析法”实现的。
 * 递归下降分析，要求对每一个终结符都给出一个分析函数，
 * 提供给每个非终结符函数调用。
 * 因此，根据parser.h中定义的文法，
 * 在开头处就直接声明了一系列内部静态分析函数，
 * 他们都是递归下降分析法所必要的。
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <setjmp.h>
#include "parser.h"

/**
 * handler      错误处理函数
 *
 * @param id    错误代码
 */
static void handler(int id);

static AST *parse_re();
static AST *parse_union();
static AST *parse_simple();
static AST *parse_basic();
static AST *parse_cat();
static AST *parse_element();

/* 全局env，用于错误处理 */
static jmp_buf env;

AST *re2ast(const char *re)
{
    /* 错误标记，0表示无错误 */
    int id;

    if ((id = setjmp(env)) == 0) {
        set_re(re);
    } else {
        handler(id);
    }

    return parse_re();
}

AST *parse_re()
{
    AST *left, *right;

    left  = parse_simple();
    right = parse_union();

    return ast_alt(left, right);
}

AST *parse_union() {
    AST *p;
    AST *left, *right;
    Token *token;

    token = next_token();
    if (token == NULL || token->t == END) {
        p = NULL;

    } else if (token->t == METACHAR && token->c == '|') {
        left  = parse_simple();
        right = parse_union();
        p = ast_alt(left, right);

    } else {
        rollback();
        p = NULL;
    }

    return p;
}

AST *parse_simple()
{
    AST *left, *right;

    left  = parse_basic();
    right = parse_cat();

    return ast_cat(left, right);
}

AST *parse_cat()
{
    AST *left, *right;

    left  = parse_basic();
    if (left == NULL) {
        return NULL;
    }
    right = parse_cat();

    return ast_cat(left, right);
}

AST *parse_basic()
{
    AST *p;
    Token *token;

    p = parse_element();
    token = next_token();
    if (token->t == END) {
        ;

    } else if (token->t == METACHAR && token->c == '*') {
        p = ast_star(p);

    } else {
        rollback();
    }

    return p;
}

AST *parse_element()
{
    AST *p;
    Token *token;

    token = next_token();
    switch (token->t) {
        case METACHAR:      // METACHAR
            if (token->c == '(') {
                p = parse_re();
                token = next_token();

                if (token->t == METACHAR && token->c == ')') {
                    ;
                } else {
                    longjmp(env, 1);    /* Missing a ')'. */
                }

            } else {
                rollback();
                p = NULL;
            }
            break;

        case END:         // END OF PARSING STRING
            p = NULL;
            break;

        default:        // NONMETA
            if (isprint(token->c)) {
                p = ast_char(token->c);
            } else {
                longjmp(env, 2);    /* Char is not printable. */
            }
            break;
    }

    return p;
}

void handler(int id)
{
    switch (id) {
        case 1:
            printf("Missing a ')' at position %d.\n", getp());
            break;
        case 2:
            printf("Character at position %d is not printable.\n", getp());
            break;
        case 3:
        default:
            printf("Unknown error.\n");
            break;
    }
    exit(id);
}
