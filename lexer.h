#ifndef FREX_LEXER_H
#define FREX_LEXER_H

/*
 * METACHAR包括：|*()\
 */
typedef enum Type {
    NONMETA, METACHAR, END,
}Type;

typedef struct Token {
    Type t;
    int c;
}Token;

extern void set_re(const char *re);

extern char *get_re();

extern int getp();

extern Token *get_token();

extern void rollback();
#endif /* FREX_LEXER_H */
