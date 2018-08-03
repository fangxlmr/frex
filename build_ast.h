#ifndef FREX_BUILD_AST_H
#define FREX_BUILD_AST_H

#include "ast.h"

extern AST *ast_char_new(int c);

extern AST *ast_cat_new(AST *left, AST *right);

extern AST *ast_alt_new(AST *left, AST *right);

extern AST *ast_star_new(AST *next);

#endif /* FREX_BUILD_AST_H */
