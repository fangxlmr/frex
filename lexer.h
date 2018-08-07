/**
 * @file lexer.h
 *
 * @brief   词法分析器
 *
 * 词法分析器（lexer）是用来将正则表达式字符串分割、拆解
 * 成记号（token）结构，向后输入给语法分析器使用。
 *
 * 逻辑上，词法分析器是先工作，返回记号，
 * 实际上，是语法分析器驱动词法分析器工作的。
 *
 * lexer.h定义了记号属性类型Type，分别代表不同的记号属性，
 * 定义了记号类型Token，用来表达记号。
 *
 * 使用set_re函数设置词法分析器需要分析的正则表达式字符串，
 * 使用get_re函数获取set_re函数设置的字符串。
 * 使用getp函数获取解析的位置。
 *
 * 使用next_token获取下一个记号，
 * 使用rollback将解析位置回滚一位。
 */

#ifndef FREX_LEXER_H
#define FREX_LEXER_H

/**
 * Type类型定义了词法分析器返回的记号（token）
 * 的所属类型，总的分为 3 类，如果需要，后续可以
 * 继续扩展。
 *
 * METACHAR：    元字符，包括：|*()\，可扩展
 * ESCAPE:       转义字符
 * END：         结束符，表示没有可用的字符，源代码匹配完毕。
 * OTHER：      非元字符，除了元字符之外的所有字符。
 *
 * @note        元字符本身也存在对应符号的非元字符，
 *              需要使用转义，如下：
 *              "\|"  ->  "|"
 *              "\*"  ->  "*"
 *              "\("  ->  "("
 *              "\)"  ->  ")"
 *              "\\"  ->  "\"
 */
typedef enum Type {
    ESCAPE, METACHAR, END, OTHER,
}Type;

/**
 * 词法分析器返回的记号
 */
typedef struct Token {
    Type t;     /* 属性 */
    int c;      /* 字符 */
}Token;

/**
 * set_re       输入正则表达式字符串
 *
 * @param re    正则表达式字符串
 */
extern void set_re(const char *re);

/**
 * get_re       获取正在解析的正则表达式字符串
 *
 * @return      存在正在解析的re字符串，则返回字符串
 *              指针，否则返回NULL。
 */
extern char *get_re();

/**
 * getp         获取解析位置
 *
 * @return      存在正在解析的re字符串，则返回解析位置，
 *              否则返回-1。
 */
extern int getp();

/**
 * get_char     获取下一个前看符号
 *
 * @return      返回下一个字符
 */
char get_char();

/**
 * next_token   获取下一个记号
 *
 * @return      若存在下一个记号，则返回记号指针，
 *              若内存分配失败，则返回NULL。
 *
 * @note        当输入的字符串解析完毕，遇到结束符（EOF或'\0'）
 *              时，函数会返回属性为END类型的token，并且会将
 *              解析位置回滚一位，因此，下一次调用时，还会返回END类型。
 *              也即，这时函数会不停的返回END类型，而不会停止工作。
 *
 *              因此，该函数只有在内存分配失败时，才会返回NULL。
 */
extern Token *next_token();

/**
 * step         将解析位置按需推进一位
 *
 * @param c     目标字符
 * @return      当下一个字符等于c时，解析位置向前推进一位，
 *              并返回非0值，否则，不推进，且返回0。
 */
extern int step(int c);

/**
 * rollback     将解析位置回滚一位
 */
extern void rollback();

#endif /* FREX_LEXER_H */
