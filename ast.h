/**
 * @file ast.h
 *
 * @brief 抽象语法树
 *
 * 定义了抽象语法树（AST）的中的算符种类属性Kind，
 * 定义了AST的不同的Kind属性，定义了对应的节点结构。
 *
 * 定义了对应节点的AST生成函数ast_char_new等。
 *
 * @note    在下方定义的抽象语法树节点结构中，不同的Kind值
 *          代表了不同的运算种类。通过读取Kind值，即可确定
 *          节点中还存在哪些数据域。
 *
 * @example
 *          AST_Char {
 *              Kind k;
 *              int c;
 *          }
 *          其中，k的值应该为AST_CHAR，那么，读取到k值后，
 *          便知道，后面的数据域一定只有一个int c。
 *
 *          对于其他节点结构，同理。
 */

#ifndef FREX_AST_H
#define FREX_AST_H

/**
 * 正则表达式的运算种类
 * AST_CHAR:    字符型
 * AST_ALT:     选择运算，对应“|”
 * AST_CAT:     连接运算
 * AST_STAR:    闭包运算，对应“*”
*/
typedef enum Kind {
    AST_CHAR, AST_ALT,
    AST_CAT,  AST_STAR,
}Kind;

/**
 * 抽象语法树顶层结构
 */
typedef struct AST {
    Kind k;
}AST;

/**
 * AST的字符节点
 */
typedef struct AST_Char {
    Kind k;
    int c;
}AST_Char;

/**
 * AST的选择运算节点
 */
typedef struct AST_Alt {
    Kind k;
    AST *left;
    AST *right;
}AST_Alt;

/**
 * AST的连接运算节点
 */
typedef struct AST_Cat {
    Kind k;
    AST *left;
    AST *right;
}AST_Cat;

/**
 * AST的“*”闭包运算
 */
typedef struct EXP_Star {
    Kind k;
    AST *next;
}AST_Star;

/**
 * ast_char_new     新建字符型节点
 *
 * @param c         字符c
 * @return          新建成功，返回AST节点指针，失败则返回NULL。
 */
extern AST *ast_char_new(int c);

/**
 * ast_cat_new      新建连接运算节点
 *
 * @param left      AST左侧子树
 * @param right     AST右侧子树
 * @return          新建成功，返回AST节点指针，失败则返回NULL。
 */
extern AST *ast_cat_new(AST *left, AST *right);

/**
 * ast_alt_new      新建选择运算节点
 *
 * @param left      AST左侧子树
 * @param right     AST右侧子树
 * @return          新建成功，返回AST节点指针，失败则返回NULL。
 */
extern AST *ast_alt_new(AST *left, AST *right);

/**
 * ast_star_new     新建“*”闭包运算节点
 *
 * @param next      AST子树
 * @return          新建成功，返回AST节点指针，失败则返回NULL。
 */
extern AST *ast_star_new(AST *next);

#endif /* FREX_AST_H */
