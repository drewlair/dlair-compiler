
EXEC = bminor
FUNC = encoder
MAIN = main
CMP = gcc
FLAGS = -Wall -g


$(EXEC): $(MAIN).o $(FUNC).o
	$(CMP) $(FLAGS) $(MAIN).o $(FUNC).o -o exec/$(EXEC)

$(MAIN).o: src/$(MAIN).c include/$(FUNC).h
	$(CMP) $(FLAGS) -c src/$(MAIN).c -o $(MAIN).o


$(FUNC).o: src/$(FUNC).c include/$(FUNC).h
	$(CMP) $(FLAGS) -c src/$(FUNC).c -o $(FUNC).o

test: bin/runtests.sh
	./bin/runtests.sh

clean:
	rm *.o
	rm exec/$(EXEC)
