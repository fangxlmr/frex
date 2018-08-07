#include <stdio.h>
#include "parser.h"
#include "ast2nfa.h"
#include "match.h"

int main()
{
    AST *ast;
    NFA *nfa;
    char *re  = "a(b|c|d)*\\**";
    char *str = "abbbcccbcbcbbcdbcbdcdd******";

    ast = re2ast(re);
    nfa = ast2nfa(ast);

    if (match(nfa, str) != 0) {
        printf("Match.\n");
    } else {
        printf("Not match.\n");
    }

    return 0;
}

