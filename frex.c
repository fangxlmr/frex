#include <stdio.h>
#include "lexer.h"
#include "parser.h"
#include "ast2nfa.h"
#include "match.h"

int main()
{
    AST *ast;
    NFA *nfa;
    char *re = "ab|c|d*";
    char *str = "abbccd";

    ast = re2ast(re);
    nfa = ast2nfa(ast);

    if (match(nfa, str) != 0) {
        printf("Match.\n");
    } else {
        printf("Error.\n");
    }

    return 0;
}

