# Author: Amoghavarsha Suresh
CORRECTION_EXE='Erroneous_lcs'
CORRECTION_SRC='lcs_error_inject.c'

NOCORRECTION_EXE='Erroneous_lcs_nocorrection'
NOCORRECTION_SRC='lcs_error_inject_nocorrection.c'


main:
	gcc -g -o $(CORRECTION_EXE) $(CORRECTION_SRC) 
nocorrection:
	gcc -g -o $(NOCORRECTION_EXE) $(NOCORRECTION_SRC)

