SRC=floydwarshall.cpp
OBJ=FloydWarshall
TMROBJ= FloydWarshallTMR
main:
	g++ -g $(SRC) -o $(OBJ)
tmr:
	g++ -g $(SRC) -o $(TMROBJ)
clean:
	rm -f $(OBJ) $(TMROBJ) *.o
