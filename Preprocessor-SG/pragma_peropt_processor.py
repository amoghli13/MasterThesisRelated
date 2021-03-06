import sys,getopt,re

#! /usr/bin/python

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
    	src_file=''
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
        if(src_file==''): 
        	print "\n\t The script requires an input -i/--input \n"
        	sys.exit(0)
    	src_file_handle=open(src_file)
    	SrcFileContents=src_file_handle.readlines()
    	src_file_handle.close()
	
	PragmaParams={}
	PragmaParams['NumDimensions']=0
	PragmaParams['size']=[]
	PragmaParams['CurrArray']=''
	PragmaParams['PrevArray']=''
	PragmaParams['Eqn']=''
	PragmaParams['Iteration']=''
	PragmaParams['DataLayout']=0
	
	PragmaLines={}
	PragmaLines['Size']=0
	PragmaLines['CurrArray']=0
	PragmaLines['PrevArray']=0
	PragmaLines['StencilLength']=0
	PragmaLines['InsertLine']=0
	PragmaLines['DataLayout']=0
	PragmaLines['Eqn']=0
	PragmaLines['Iteration']=0


	SizeNotFound=1
	CurrArrayNotFound=1
	PrevArrayNotFound=1
	StencilLengthNotFound=1
	InsertLineNotFound=1
	DataLayoutNotFound=1
	EqnNotFound=1
	IterationNotFound=1
	PragmasDeleteOrder=[]
	
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
					PragmasDeleteOrder.append(LineCount)
				elif para_chk.group(1)=="curr_array":
					print "\n\t Found curr_array --> "+str(para_chk.group(1))+" , "+str(para_chk.group(2))
					PragmaParams['curr_array']=para_chk.group(2)
					CurrArrayNotFound=0
					PragmaLines['CurrArray']=LineCount
					PragmasDeleteOrder.append(LineCount)
				elif para_chk.group(1)=="prev_array":
					print "\n\t Found prev_array --> "+str(para_chk.group(1))+" , "+str(para_chk.group(2))
					PragmaParams['prev_array']=para_chk.group(2)
					PrevArrayNotFound=0
					PragmaLines['PrevArray']=LineCount
					PragmasDeleteOrder.append(LineCount)
				elif para_chk.group(1)=="stencil_length":
					print "\n\t Found stencil_length --> "+str(para_chk.group(1))+" , "+str(para_chk.group(2))
					PragmaParams['stencil_length']=int(para_chk.group(2))
					StencilLengthNotFound=0
					PragmaLines['StencilLength']=LineCount
					PragmasDeleteOrder.append(LineCount)
				elif para_chk.group(1)=="insert_here":
					print "\n\t Found insert here --> "+str(para_chk.group(1))+" , "+str(para_chk.group(2))
					#PragmaParams['insert_line']=LineCount
					PragmaLines['InsertLine']=LineCount
					InsertLineNotFound=0
					#PragmasDeleteOrder.append(LineCount)
				elif para_chk.group(1)=="data_layout":
					print "\n\t Found data-layout here --> "+str(para_chk.group(1))+" , "+str(para_chk.group(2))
					Dummy=re.sub('^\s*','',para_chk.group(2))
					DataLayout=re.sub('\s*$','',Dummy)
					PragmaParams['DataLayout']=int(DataLayout)
					PragmaLines['DataLayout']=LineCount
					DataLayoutNotFound=0
					PragmasDeleteOrder.append(LineCount)
					if((PragmaParams['DataLayout']!=1)):
						print "\n\t Currently only data-layout option 1 is supported and the code is: "+str(PragmaParams['DataLayout'])+"\n"
						sys.exit()
				elif para_chk.group(1)=="equation":
					print "\n\t Found the equation: "+str(para_chk.group(2))
					PragmaParams['Eqn']=str(para_chk.group(2))
					PragmaLines['Eqn']=LineCount
					EqnNotFound=0
					#PragmasDeleteOrder.append(LineCount)
				elif para_chk.group(1)=="iteration":
					print "\n\t Found the iteration: "+str(para_chk.group(2))
					PragmaParams['Iteration']=str(para_chk.group(2))
					PragmaLines['Iteration']=LineCount
					IterationNotFound=0
					PragmasDeleteOrder.append(LineCount)
				
					
	if( SizeNotFound or CurrArrayNotFound or PrevArrayNotFound or StencilLengthNotFound or InsertLineNotFound or DataLayoutNotFound or EqnNotFound or IterationNotFound):
		CallThePolice("\n\t One of the required preprocessor directive is missing. Rerun it with debug enabled.")
	else:

		RobustFileContents=list(SrcFileContents)
		AdjustLines=0
		for LineNum in PragmasDeleteOrder:
			RobustFileContents.pop(LineNum-AdjustLines)
			AdjustLines+=1 
 
		
		SizeString=''
		TabSpace='\t\t'
		for CurrSizeIdx in range(len(PragmaParams['size'])):
			SizeString+=PragmaParams['size'][CurrSizeIdx]+','
			TabSpace+='\t'
		#SizeString+=PragmaParams['size'][len(PragmaParams['size'])-1]
		
		FuncCall=TabSpace+'grid_analyze_per_quadrant('+str(PragmaParams['curr_array'])+','+str(PragmaParams['prev_array'])+','+str(SizeString)+str(PragmaParams['DataLayout'])+','+str(PragmaParams['Iteration'])+');'
		print "\n\t FuncCall: "+str(FuncCall)
		print "\n\t Before: "+str(RobustFileContents[PragmaLines['InsertLine']-AdjustLines])
		RobustFileContents[PragmaLines['InsertLine']-AdjustLines]=FuncCall
		print "\n\t After: "+str(RobustFileContents[PragmaLines['InsertLine']-AdjustLines])		
		RobustFileContents[PragmaLines['Eqn']-AdjustLines]=TabSpace+PragmaParams['Eqn']
		
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


		if(EqnNotFound or (NumEqnFound!=1)):
			CallThePolice("\n\t Equation insert loation not found or the number of equations found is "+str(NumEqnFound))
		else:
			IterativeLib=open("RobustIterativeSG.h","w")
			for CurrLine in ChangeLib:
				IterativeLib.write(CurrLine)
			IterativeLib.close()
			print "\n\t Library is also updated! "
		
if __name__ == "__main__":
    main()














