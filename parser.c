#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

static void error();
static AST *parse_re();
static AST *parse_union();
static AST *parse_simple();
static AST *parse_basic();
static AST *parse_cat();
static AST *parse_element();

void error()
{
    printf("Parsing error around position %d.\n", getp());
    exit(-1);
}

AST *parse_re()
{
    AST *left, *right;
    left  = parse_simple();
    right = parse_union();
    return ast_alt_new(left, right);
}

AST *parse_union() {
    AST *left, *right;
    Token *token;

    token = get_token();
    if (token == NULL) {
        return NULL;
    } else if (token->t == END) {
        return NULL;
    } else if (token->t == METACHAR && token->c == '|') {
        left  = parse_simple();
        right = parse_union();
        return ast_cat_new(left, right);
    } else {
        rollback();
        return NULL;
    }
}

AST *parse_simple()
{
    AST *left, *right;

    left  = parse_basic();
    right = parse_cat();

    return ast_cat_new(left, right);
}

AST *parse_cat()
{
    AST *left, *right;

    left  = parse_basic();
    if (left == NULL) {
        return NULL;
    }
    right = parse_cat();

    return ast_cat_new(left, right);
}

AST *parse_basic()
{
    AST *p;
    Token *token;

    p = parse_element();
    token = get_token();
    if (token->t == END) {
        return p;
    } else if (token->t == METACHAR && token->c == '*') {
        return ast_star_new(p);
    } else {
        rollback();
        return p;
    }
}

AST *parse_element()
{
    AST *p;
    Token *token;

    p = NULL;
    token = get_token();
    switch (token->t) {
        case METACHAR:      // METACHAR
            if (token->c == '(') {
                p = parse_re();
                token = get_token();

                if (token->t == END) {
                    printf("Missing ')'.\n");
                    error();
                } else if (token->t == METACHAR && token->c == ')') {
                    return p;
                } else {
                    error();
                }
            } else {
                rollback();
                return NULL;
            }
            break;
        case END:         // END OF STRING
            return NULL;
        default:        // NONMETA
            if (isprint(token->c)) {
                p = ast_char_new(token->c);
                return p;
            } else {
                error();
            }
    }
}

AST *re2ast(const char *re)
{
    set_re(re);
    return parse_re();
}