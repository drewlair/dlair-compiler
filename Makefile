
EXEC = bminor
FUNC = encoder
MAIN = main
CMP = gcc
FLAGS = -Wall -g

all: $(EXEC)

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

main.o: src/main.c include/main.h include/encoder.h parser.h
	$(CMP) $(FLAGS) src/main.c -c -o main.o

scanner.o: scanner.c include/token.h
	$(CMP) -g scanner.c -c -o scanner.o

parser.o: parser.c parser.h include/type.h
	$(CMP) $(FLAGS) parser.c -c -o parser.o

encoder.o: src/encoder.c include/encoder.h
	$(CMP) $(FLAGS) src/encoder.c -c -o encoder.o

$(EXEC): $(MAIN).o $(FUNC).o scanner.o parser.o expr.o type.o param_list.o decl.o stmt.o
	$(CMP) $(FLAGS) $(MAIN).o $(FUNC).o scanner.o parser.o expr.o type.o param_list.o decl.o stmt.o -o exec/$(EXEC) -lm

test: bin/runprints.sh
	./bin/runprints.sh

scan: bin/runscans.sh
	./bin/runscans.sh

parse: bin/runparse.sh
	./bin/runparse.sh

print: bin/runprints.sh
	./bin/runprints.sh

clean:

	rm *.o
	rm scanner.c
	rm parser.h
	rm parser.c
	rm parser.output
	rm exec/$(EXEC)
	rm test/parser/*.bminor.out
	rm test/scanner/*.bminor.out
	rm test/encode/*.bminor.out
