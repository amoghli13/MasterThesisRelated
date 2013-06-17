# Author: Amoghavarsha Suresh
EXE='Erroneous_lcs'
SRC='lcs_error_inject.c'
STD='-std=c99'

main:
	gcc -g -o $(EXE) $(SRC) $(STD)
