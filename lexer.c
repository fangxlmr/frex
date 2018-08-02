#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

int INDEX = 0;
const char *RE;

static char next_char()
{
    return *(RE + INDEX++);
}

void rollback()
{
    --INDEX;
}

Token *next_token()
{
    int c;
    Token *token;

    token = (Token *) malloc(sizeof(token));
    if (!token) {
        return NULL;
    }
    token->c = 0;

    c = next_char();
    switch (c) {
        default:
            token->t = CHAR;
            token->c = c;
            break;
        case '\0':
            token = NULL;
            break;
        case '|':
            token->t = OR;
            break;
        case '*':
            token->t = STAR;
            break;
        case '(':
            token->t = LPAREN;
            break;
        case ')':
            token->t = RPAREN;
            break;
        case '\\':
            c = next_char();
            switch(c) {
                case '|':
                case '*':
                case '(':
                case ')':
                case '\\':
                    token->t = CHAR;
                    token->c = c;
                    break;
                default:
                    token->t = CHAR;
                    token->c = '\\';
                    rollback();
                    break;
            }
            break;
    }
    return token;
}
