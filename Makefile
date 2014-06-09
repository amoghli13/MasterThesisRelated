
SRC=dp_shortestpathOO.cpp
OBJ=(DP_ShortestPathOO)

InitSRC=shortestpath.cpp 
InitOBJ=ShortestPath

Init:
	g++ -g $(InitSRC) -o $(InitOBJ)

InitClean:
	rm -f *.o $(InitOBJ)
clean:
	rm -f *.o $(OBJ)
