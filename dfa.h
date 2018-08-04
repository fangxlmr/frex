#ifndef FREX_DFA_H
#define FREX_DFA_H

#include "nfa.h"

typedef struct _List {
    NState **ns;
    int n;
}List;

typedef struct _DState DState;
struct _DState {
    List l;
    DState *next[256];
    DState *left;
    DState *right;
};

#endif /* FREX_DFA_H */
