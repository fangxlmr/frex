#ifndef FREX_AST_H
#define FREX_AST_H

typedef enum Kind {
    AST_CHAR, AST_ALT,
    AST_CAT,  AST_STAR,
}Kind;

typedef struct AST {
    Kind k;
}AST;

typedef struct AST_Char {
    Kind k;
    int c;
}AST_Char;

typedef struct AST_Alt {
    Kind k;
    AST *left;
    AST *right;
}AST_Alt;

typedef struct AST_Cat {
    Kind k;
    AST *left;
    AST *right;
}AST_Cat;

typedef struct EXP_Star {
    Kind k;
    AST *next;
}AST_Star;

extern AST *ast_char_new(int c);

extern AST *ast_cat_new(AST *left, AST *right);

extern AST *ast_alt_new(AST *left, AST *right);

extern AST *ast_star_new(AST *next);

#endif /* FREX_AST_H */
