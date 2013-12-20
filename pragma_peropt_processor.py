import sys,getopt,re

#! /usr/bin/python

##########################################
# Implementation:
#	1. Accept pragma for following options. Check for each option and throw error if one of them is not available.
#		a. Size in each dimension (hence figure out which dimension) - should be CSV atleast. 
#		b. Name of current and previous grid.
#		c. Line at which the equation is present.
#
#PENDING:
#1. Should have valid code for the library of "RobustIterativeSG.h"

# Assumptions for now
#
#
#
#
#
#
##########################################
# Future to-do ?
#	
#
#
############################################

def CallThePolice(Msg):
	print Msg
	print "\n\t If running in Debug mode does not help, please report this to sure0043@umn.edu and provide as much detail as possible to reproduce the bug. "

def main():
	try: 
        	opts, args = getopt.getopt(sys.argv[1:],'i:h:v',['input',"help", 'verbose='])
    	except getopt.GetoptError as err:
        # print help information and exit:
		print str(err) # will print something like "option -a not recognized"
		usage()
		sys.exit(2)
    	verbose = False
    	for o, a in opts:
        	if o == "-v":
            		verbose = True
            #print verbose
        	elif o in ("-h", "--help"):
            		usage()
            		sys.exit()
        	elif o in ("-i","--input"):
            		src_file = a
           		if( re.match(r'\s*.*\.(cpp|cc)',src_file,re.I) ):
	            		print "\n\t Source file: "+a+"\n";
	    		else:
	    			print "\n\t Source file is "+str(src_file)
	    	 		print "\n\t Source file should be of type *.cpp or *.cc \n"
	    	 		sys.exit()
        	else:
            		assert False, "unhandled option"
    # ...
    	src_file_handle=open(src_file)
    	SrcFileContents=src_file_handle.readlines()
    	src_file_handle.close()
	
	PragmaParams={}
	PragmaParams['NumDimensions']=0
	PragmaParams['size']=[]
	PragmaLines={}
	PragmaLines['Size']=0
	PragmaLines['CurrArray']=0
	PragmaLines['PrevArray']=0
	PragmaLines['StencilLength']=0
	PragmaLines['InsertLine']=0
	PragmaLines['Datalayout']=0


	SizeNotFound=1
	CurrArrayNotFound=1
	PrevArrayNotFound=1
	StencilLengthNotFound=1
	InsertLineNotFound=1
	DataLayoutNotFound=1
	
	
	LineCount=-1; # To make line-count 0-indexed
	for curr_line in SrcFileContents:
    		LineCount+=1
    		matchObj=re.match(r'\s*\#pragma',curr_line)
    		if matchObj:
    			syntax_chk=re.match(r'\s*\#pragma\s+struct_grid',curr_line)
	    		print "\n\t Found pragma with struct_grid in line: "+str(LineCount)+" : "+curr_line    
			if syntax_chk:
				para_chk=re.match(r'\s*\#pragma\s+struct_grid\s+(\w+)+\s*(.*)',curr_line)  
				if para_chk.group(1)=="size":
					print "\n\t Found size: "+str(para_chk.group(1))
					temp=para_chk.group(2);#.split()
					temp1=temp.split(",")
					print "\n\t Number of dimensions "+str(len(temp1))
					dim_num=0
					for curr_size in temp1:
							print "\n\t Dim: "+str(dim_num)+" size "+str(curr_size)
							dim_num+=1;
							PragmaParams['size'].append(curr_size)
					print "\n\t Found "+str(dim_num)+" number of dimensions"
					SizeNotFound=0
					PragmaParams['NumDimensions']=dim_num
					PragmaLines['Size']=LineCount
				elif para_chk.group(1)=="curr_array":
					print "\n\t Found curr_array --> "+str(para_chk.group(1))+" , "+str(para_chk.group(2))
					PragmaParams['curr_array']=para_chk.group(2)
					CurrArrayNotFound=0
					PragmaLines['CurrArray']=LineCount
				elif para_chk.group(1)=="prev_array":
					print "\n\t Found prev_array --> "+str(para_chk.group(1))+" , "+str(para_chk.group(2))
					PragmaParams['prev_array']=para_chk.group(2)
					PrevArrayNotFound=0
					PragmaLines['PrevArray']=LineCount
				elif para_chk.group(1)=="stencil_length":
					print "\n\t Found stencil_length --> "+str(para_chk.group(1))+" , "+str(para_chk.group(2))
					PragmaParams['stencil_length']=int(para_chk.group(2))
					StencilLengthNotFound=0
					PragmaLines['StencilLength']=LineCount
				elif para_chk.group(1)=="insert_here":
					print "\n\t Found insert here --> "+str(para_chk.group(1))+" , "+str(para_chk.group(2))
					#PragmaParams['insert_line']=LineCount
					PragmaLines['InsertLine']=LineCount
					InsertLineNotFound=0
				elif para_chk.group(1)=="data_layout":
					print "\n\t Found data-layout here --> "+str(para_chk.group(1))+" , "+str(para_chk.group(2))
					PragmaParams['DataLayout']=int(para_chk.group(2))
					PragmaLines['DataLayout']=LineCount
					DataLayoutNotFound=0
				elif para_chk.group(1)=="equation":
					print "\n\t Found the equation: "+str(para_chk.group(2))
					PragmaParams['Eqn']=str(para_chk.group(2))
				
					
	if( SizeNotFound or CurrArrayNotFound or PrevArrayNotFound or StencilLengthNotFound or InsertLineNotFound or DataLayoutNotFound):
		CallThePolice("\n\t One of the required preprocessor directive is missing. Rerun it with debug enabled.")
	else:
		DeleteLines=5
		RobustFileContents=list(SrcFileContents)			
		RobustFileContents.pop(PragmaLines['Size']) 
		RobustFileContents.pop(PragmaLines['CurrArray']-1) 		
		RobustFileContents.pop(PragmaLines['PrevArray']-2) 		
		RobustFileContents.pop(PragmaLines['StencilLength']-3) 		
		RobustFileContents.pop(PragmaLines['DataLayout']-4) 	
		SizeString=''
		TabSpace='\t'
		for CurrSizeIdx in range(len(PragmaParams['size'])):
			SizeString+=PragmaParams['size'][CurrSizeIdx]+','
			TabSpace+='\t'
		#SizeString+=PragmaParams['size'][len(PragmaParams['size'])-1]
		
		FuncCall=TabSpace+'grid_analyze_per_quadrant('+str(PragmaParams['curr_array'])+','+str(PragmaParams['prev_array'])+','+str(SizeString)+str(PragmaParams['DataLayout'])+')'
		print "\n\t FuncCall: "+str(FuncCall)
		print "\n\t Before: "+str(RobustFileContents[PragmaLines['InsertLine']-DeleteLines])
		RobustFileContents[PragmaLines['InsertLine']-DeleteLines]=FuncCall
		print "\n\t After: "+str(RobustFileContents[PragmaLines['InsertLine']-DeleteLines])		
		
		RobustFileName='Robust_'+str(src_file)
		RobustFile=open(RobustFileName,'w')
		RobustFile.write("\n\t #include RobustIterativeSG.h \n")
		for CurrLine in RobustFileContents:
			RobustFile.write(CurrLine)
		
		print "\n\t Should have generated the RobustFile: "+str(RobustFileName)
		RobustFile.close()
		
		IterativeLib=open("RobustIterativeSGDefault.h","r")
		ChangeLib=IterativeLib.readlines()
		IterativeLib.close()
		EqnNotFound=1
		NumEqnFound=0
		LineNum=-1
		for CurrLine in ChangeLib:
			LineNum+=1
			ShouldInsertHere=re.match('^\s*insert\_equation\s*$',CurrLine)
			if ShouldInsertHere:
				print "\n\t Is this the line where I should insert?? "+str(CurrLine)
				ChangeLib[LineNum]='\n\t'+PragmaParams['Eqn']	
				EqnNotFound=0
				NumEqnFound+=1


		if(EqnNotFound or (NumEqnFound!=2)):
			CallThePolice("\n\t Equation insert loation not found or the number of equations found is "+str(NumEqnFound))
		else:
			IterativeLib=open("RobustIterativeSG.h","w")
			for CurrLine in ChangeLib:
				IterativeLib.write(CurrLine)
			IterativeLib.close()
			print "\n\t Library is also updated! "
		
if __name__ == "__main__":
    main()














