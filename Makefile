
EXEC = bminor
FUNC = encoder
MAIN = main
CMP = gcc
FLAGS = -Wall -g

all: $(EXEC)

typecheck.o: include/typecheck.h src/typecheck.c
	$(CMP) $(FLAGS) src/typecheck.c -c -o typecheck.o

resolve.o: src/resolve.c include/resolve.h include/main.h include/symbol.h
	$(CMP) $(FLAGS) src/resolve.c -c -o resolve.o

symbol.o: src/symbol.c include/symbol.h 
	$(CMP) $(FLAGS) src/symbol.c -c -o symbol.o

ht.o: src/hash_table.c include/hash_table.h
	$(CMP) $(FLAGS) src/hash_table.c -c -o ht.o

scope.o: src/scope.c include/scope.h
	$(CMP) $(FLAGS) src/scope.c -c -o scope.o

expr.o: src/expr.c include/expr.h include/encoder.h
	$(CMP) $(FLAGS) src/expr.c -c -o expr.o

type.o: src/type.c include/type.h
	$(CMP) $(FLAGS) src/type.c -c -o type.o

param_list.o: src/param_list.c include/param_list.h
	$(CMP) $(FLAGS) src/param_list.c -c -o param_list.o

decl.o: src/decl.c include/param_list.h
	$(CMP) $(FLAGS) src/decl.c -c -o decl.o

stmt.o: src/stmt.c include/stmt.h
	$(CMP) $(FLAGS) src/stmt.c -c -o stmt.o

parser.c parser.h token_bison.h: src/parser.bison
	bison --defines=parser.h --output=parser.c -v src/parser.bison

scanner.c: src/scanner.flex token_bison.h parser.h
	flex -oscanner.c src/scanner.flex

main.o: src/main.c include/typecheck.h include/encoder.h parser.h
	$(CMP) $(FLAGS) src/main.c -c -o main.o

scanner.o: scanner.c include/token.h
	$(CMP) -g scanner.c -c -o scanner.o

parser.o: parser.c parser.h include/type.h
	$(CMP) $(FLAGS) parser.c -c -o parser.o

encoder.o: src/encoder.c include/encoder.h
	$(CMP) $(FLAGS) src/encoder.c -c -o encoder.o

$(EXEC): $(MAIN).o $(FUNC).o scanner.o parser.o expr.o type.o param_list.o decl.o stmt.o scope.o ht.o resolve.o symbol.o typecheck.o
	$(CMP) $(FLAGS) $(MAIN).o $(FUNC).o scanner.o parser.o expr.o type.o param_list.o decl.o stmt.o scope.o ht.o resolve.o symbol.o typecheck.o -o exec/$(EXEC) -lm

test: bin/runtypechecks.sh
	./bin/runtypechecks.sh

scan: bin/runscans.sh
	./bin/runscans.sh

parse: bin/runparse.sh
	./bin/runparse.sh

print: bin/runprints.sh
	./bin/runprints.sh

resolve: bin/runresolve.sh 
	./bin/runresolve.sh

typecheck: bin/runtypecheck.sh 
	./bin/typecheck.sh 


clean:

	rm *.o
	rm scanner.c
	rm parser.h
	rm parser.c
	rm parser.output
	rm exec/$(EXEC)
	rm test/typechecker/*.bminor.out
	rm test/resolver/*.bminor.out
	rm test/printer/*.bminor.out
	rm test/parser/*.bminor.out
	rm test/scanner/*.bminor.out
	rm test/encode/*.bminor.out
