SRC=floydwarshall.cpp
OBJ=FloydWarshall
TMROBJ= FloydWarshallTMR
PRISTINEOBJ=FloydWarshallPristine
main:
	g++ -g $(SRC) -o $(OBJ)
tmr:
	g++ -g $(SRC) -o $(TMROBJ)
pristine: 
	g++ -g $(SRC) -o $(PRISTINEOBJ)
clean:
	rm -f $(OBJ) $(TMROBJ) $(PRISTINEOBJ) *.o
