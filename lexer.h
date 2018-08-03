#ifndef FREX_LEXER_H
#define FREX_LEXER_H

typedef enum Type {
    CHAR = 1, OR, STAR, LPAREN, RPAREN,
}Type;

typedef struct Token {
    Type t;
    int c;
}Token;

extern const char *RE;

extern int INDEX;

extern Token *next_token();

extern int match(int c);

#endif /* FREX_LEXER_H */
