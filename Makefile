
SRC=dp_shortestpathOO.cpp
OBJ=(DP_ShortestPathOO)

InitSRC=shortestpath.c 
InitOBJ=ShortestPath

Init:
	gcc -g $(InitSRC) -o $(InitOBJ)

InitClean:
	rm -f *.o $(InitOBJ)
clean:
	rm -f *.o $(OBJ)
