
EXEC = bminor
FUNC = encoder
MAIN = main
CMP = gcc
FLAGS = -Wall -g

all: $(EXEC)

parser.c parser.h token_bison.h: src/parser.bison
	bison -t -v --defines=parser.h --output=parser.c -v src/parser.bison

scanner.c: src/scanner.flex token_bison.h parser.h
	flex -oscanner.c src/scanner.flex

main.o: src/main.c include/main.h include/encoder.h
	$(CMP) $(FLAGS) src/main.c -c -o main.o

scanner.o: scanner.c include/token.h
	$(CMP) -g scanner.c -c -o scanner.o

parser.o: parser.c parser.h
	$(CMP) $(FLAGS) parser.c -c -o parser.o

encoder.o: src/encoder.c include/encoder.h
	$(CMP) $(FLAGS) src/encoder.c -c -o encoder.o

$(EXEC): $(MAIN).o $(FUNC).o scanner.o parser.o
	$(CMP) $(FLAGS) $(MAIN).o $(FUNC).o scanner.o parser.o -o exec/$(EXEC) -lm

test: bin/runtests.sh bin/runscans.sh
	./bin/runtests.sh
	./bin/runscans.sh

scan: bin/runscans.sh
	./bin/runscans.sh

parse: bin/runparse.sh
	./bin/runparse.sh

clean:

	rm *.o
	rm scanner.c
	rm parser.h
	rm parser.c
	rm parser.output
	rm exec/$(EXEC)
	rm test/scanner/*.out
	rm test/encode/*.out
