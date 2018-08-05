OBJECTS = frex.o lexer.o parser.o ast.o ast2nfa.o match.o
frex: $(OBJECTS)
	gcc -o $@ $(OBJECTS)

frex.o: lexer.h parser.h ast2nfa.h match.h
parser.o: lexer.h ast.h
ast2nfa.o: ast.h nfa.h
match.o: nfa.h dfa.h

.PHONY: clean
clean:
	-rm frex  $(OBJECTS)
