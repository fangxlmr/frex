#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "ast.h"

Token *token;

static AST *parse_re();
static AST *parse_union();
static AST *parse_simple();
static AST *parse_basic();
static AST *parse_cat();
static AST *parse_element();

static void error()
{
    printf("Parsing error of %c at position %d.\n", *(RE + INDEX - 1), INDEX);
    exit(-1);
}

AST *parse_re()
{
    AST *left, *right;
    left  = parse_simple();
    right = parse_union();
    return ast_cat_new(left, right);
}

AST *parse_union() {
    AST *left, *right;

    token = next_token();
    if (token == NULL) {
        return NULL;
    }

    if (token->c != OR) {
        return NULL;
    }
    left  = parse_simple();
    right = parse_union();

    return ast_cat_new(left, right);
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

    p = parse_element();

    if (match('*')) {
        next_token();
        return ast_star_new(p);
    } else {
        return p;
    }
}

AST *parse_element()
{
    AST *p;

    token = next_token();
    if (token == NULL) {
        return NULL;
    }

    if (match(LPAREN)) {
        next_token();
        p = parse_re();
        token = next_token();

        if (token == NULL || token->t != RPAREN) {
            error();
        }
        return p;
    }  else if (match(CHAR)) {
        next_token();
        if (isprint(token->c)) {
            p = ast_char_new(token->c);
            return p;
        } else {
            error();
        }
    } else {
        return NULL;
    }
}

AST *re2ast(const char *re)
{
    RE = re;
    return parse_re();
}