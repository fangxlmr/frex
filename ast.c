#include <stdlib.h>
#include "ast.h"

AST *ast_char_new(int c)
{
    AST_Char *p;

    p = (AST_Char *) malloc(sizeof(p));
    if (p) {
        p->k = AST_CHAR;
        p->c = c;
    }
    return p;
}

AST *ast_cat_new(AST *left, AST *right)
{
    AST_Cat *p;

    if ((left && right) == 0) {
        return left ? left : right;
    }

    p = (AST_Cat *) malloc(sizeof(p));
    if (p) {
        p->k = AST_CAT;
        p->left  = left;
        p->right = right;
    }
    return p;
}

AST *ast_alt_new(AST *left, AST *right)
{
    AST_Alt *p;

    if ((left && right) == 0) {
        return left ? left : right;
    }

    p = (AST_Alt *) malloc(sizeof(p));

    if (p) {
        p->k = AST_ALT;
        p->left  = left;
        p->right = right;
    }
    return p;
}

AST *ast_star_new(AST *next)
{
    AST_Star *p;

    p = (AST_Star *) malloc(sizeof(p));

    if (p) {
        p->k = AST_STAR;
        p->next = next;
    }
    return p;
}
