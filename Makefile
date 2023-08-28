EXEC = bminor
FUNC = encoder
MAIN = main
CMP = gcc

$(EXEC): $(MAIN).o $(FUNC).o
	$(CMP) $(MAIN).o $(FUNC).o -o $(EXEC)

$(MAIN).o: $(MAIN).c $(FUNC).h
	$(CMP) -c $(MAIN).c -o $(MAIN).o

$(FUNC).o: $(FUNC).c $(FUNC).h
	$(CMP) -c $(FUNC).c -o $(FUNC).o

clean:
	rm *.o
	rm $(EXEC)
