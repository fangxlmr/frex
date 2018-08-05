/*
 * AST的代码实现。
 */
#include <stdlib.h>
#include "ast.h"

/*
 * 值得一提的是，下面所有新建AST节点
 * 函数的返回值类型都是AST *，
 * 但节点的类型本身并不是AST *，而可能是
 * AST_Char * 或 AST_Cat * 或其他，
 * 但是，这里为了统一，还是将返回值隐式转换
 * 为AST *类型。（这也是ast.h中定义AST *类型的原因。）
 *
 * 所以，在ast2nfa.c中，需要先读取Kind k的值，
 * 根据k值，再强制转换数据类型，之后才能访问
 * Kind k之后的数据域。否则，由于AST *类型
 * 之后一个Kind k数据域，后续的数据域（或指针域）
 * 是无法被访问的。
 */
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
