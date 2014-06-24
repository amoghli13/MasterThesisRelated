SRC=floydwarshall.cpp
OBJ=FloydWarshall
TMROBJ= FloydWarshallTMR
PRISTINEOBJ=FloydWarshallPristine
DEGRADATIONOBJ= FloydWarshallDeg
main:
	g++ -g $(SRC) -o $(OBJ)
tmr:
	g++ -g $(SRC) -o $(TMROBJ)
pristine: 
	g++ -g $(SRC) -o $(PRISTINEOBJ)
deg:
	g++ -g $(SRC) -o $(DEGRADATIONOBJ)
clean:
	rm -f $(OBJ) $(TMROBJ) $(PRISTINEOBJ) *.o
