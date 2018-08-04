#ifndef FREX_AST2NFA_H
#define FREX_AST2NFA_H

#include "nfa.h"
#include "ast.h"

extern int NSTATE;

extern NFA *ast2nfa(AST *ast);
#endif /* FREX_AST2NFA_H */
