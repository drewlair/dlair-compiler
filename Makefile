EXEC = bminor
FUNC = encoder
MAIN = main
CMP = gcc
FLAGS = -Wall -g

$(EXEC): $(MAIN).o $(FUNC).o
	$(CMP) $(FLAGS) $(MAIN).o $(FUNC).o -o $(EXEC)

$(MAIN).o: $(MAIN).c $(FUNC).h
	$(CMP) $(FLAGS) -c $(MAIN).c -o $(MAIN).o

$(FUNC).o: $(FUNC).c $(FUNC).h
	$(CMP) $(FLAGS) -c $(FUNC).c -o $(FUNC).o

clean:
	rm *.o
	rm $(EXEC)
