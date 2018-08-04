#include <stdio.h>
#include "ast.h"
#include "nfa.h"
#include "lexer.h"
#include "parser.h"
#include "ast2nfa.h"


int main()
{
    AST *ast;
    NFA *nfa;
    char *re = "a*|";

    ast = re2ast(re);
    nfa = ast2nfa(ast);

    int table[NSTATE][256];
    

    printf("Done.\n");
    return 0;
}

