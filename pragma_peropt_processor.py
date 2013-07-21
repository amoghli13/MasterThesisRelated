import sys,getopt,re

#! /usr/bin/python

##########################################
# Implementation:
#	1. Accept pragma for following options. Check for each option and throw error if one of them is not available.
#		a. Size in each dimension (hence figure out which dimension) - should be CSV atleast. 
#		b. Name of current and previous grid.
#		c. Line at which the equation is present.
#
#
#

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
	    	 		print "\n\t Source file should be of type *.cpp or *.cc \n"
	    	 		sys.exit()
        	else:
            		assert False, "unhandled option"
    # ...
    	src_file_handle=open(src_file)
    	src_file_contents=src_file_handle.readlines()
    	src_file_handle.close()
	
	pragma_params={}
	pragma_params['size']=[]

	line_count=0;
    	for curr_line in src_file_contents:
    		matchObj=re.match(r'\s*\#pragma',curr_line)
    		if matchObj:
    			syntax_chk=re.match(r'\s*\#pragma\s+struct_grid',curr_line)
	    		print "\n\t Found pragma with struct_grid in line: "+str(line_count)+" : "+curr_line    
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
							pragma_params['size'].append(curr_size)
				elif para_chk.group(1)=="curr_array":
					print "\n\t Found curr_array --> "+str(para_chk.group(1))+" , "+str(para_chk.group(2))
					pragma_params['curr_array']=para_chk.group(2)
				elif para_chk.group(1)=="prev_array":
					print "\n\t Found prev_array --> "+str(para_chk.group(1))+" , "+str(para_chk.group(2))
					pragma_params['prev_array']=para_chk.group(2)
					

					#else 		
						#print "\n\t -- Temp: "+str(temp1)
if __name__ == "__main__":
    main()














