
EXEC = bminor
FUNC = encoder
MAIN = main
CMP = gcc
FLAGS = -Wall -g

all: $(EXEC)

scanner.c: src/scanner.flex
	flex -oscanner.c src/scanner.flex

scanner.o: scanner.c
	$(CMP) -c scanner.c -o scanner.o

$(FUNC).o: src/$(FUNC).c include/$(FUNC).h
	$(CMP) $(FLAGS) -c src/$(FUNC).c -o $(FUNC).o

$(MAIN).o: src/$(MAIN).c include/$(FUNC).h include/$(MAIN).h
	$(CMP) $(FLAGS) -c src/$(MAIN).c -o $(MAIN).o

$(EXEC): $(MAIN).o $(FUNC).o scanner.o
	$(CMP) $(FLAGS) $(MAIN).o $(FUNC).o scanner.o -o exec/$(EXEC)

test: bin/runtests.sh bin/runscans.sh
	./bin/runtests.sh
	./bin/runscans.sh

scan: bin/runscans.sh
	./bin/runscans.sh

clean:
	rm *.o
	rm exec/$(EXEC)
	rm scanner.c
	rm test/scanner/*.out
	rm test/encode/*.out
