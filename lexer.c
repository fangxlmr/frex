#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

static int INDEX = 0;
const char *RE = NULL;

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
    return INDEX;
}

void rollback()
{
    --INDEX;
}

static char get_char()
{
    return *(RE + INDEX++);
}

Token *get_token()
{
    int c;
    Token *token;

    token = (Token *) malloc(sizeof(Token *));
    if (!token) {
        printf("Malloc failed.\n");
        return NULL;
    }

    c = get_char();
    switch (c) {
        default:
            token->t = NONMETA;
            token->c = c;
            break;
        case '\0':
            token->t = END;
            token->c = '\0';
            rollback();
            break;
        case '|':
        case '*':
        case '(':
        case ')':
            token->t = METACHAR;
            token->c = c;
            break;
        case '\\':
            c = get_char();
            switch(c) {
                case '|':
                case '*':
                case '(':
                case ')':
                case '\\':
                    token->t = NONMETA;
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
