##########################################
# Implementation:
#	1. Support the pragma insertion. The stencil region is marked with pragma start & end and #dimensions, size of each dimension is extracted. - Done
#	2. Parse the #dim, size in each dim to a method, which replaces the work. - Pending.
#		a. Figure out number of dimensions in the array. - in regex find #of '['
#		b. From 'a' we can determine the way in which the matrix/array elements are accessed. Need this info when replacing the pragma region.
#		c. Array which is being updated and it's previous state should be determiined. -- LHS can be point to array_updating & term which appears the most in RHS can be thought of as array which 
#		is providing the values. 
#			side-note: All the extracted parameters should be conveyed to the user, so that any required change can be made. 
#	3. items 2.a & 2.b would provide info to the method, which can replace the pramga bound region and does the following work
#		a. Decompose the matrix into 3 levels, as required.
#		b. Innsert L2 norm calculation.
#		c. Use a method to check after each grid-update or just use it in between.
#		d. Run for required number of iterations - SHOULD TAKE CARE!!!!!!


# Assumptions for now
#	1. for loop: 
#		Param-1: a. The first term is either 'int' or the variable in respective dimension.
#		Param-2: a. The terms after the variable (discovered in param-1) would be the relation and the subseqhuent term would be the size of this dimension.
#
#
#
##########################################
# Future to-do ?
#	1. Should take care of comments- how?
#
#
############################################
 
import getopt, sys,re

def usage():
	print '\n\t Usage: pragma_processor.py -h -output <output_file> '

def extract_info(pragma_starts,pragma_ends,src_file_contents):
	print "\n\t Start: "+src_file_contents[pragma_starts];
	print "\n\t Ends: "+src_file_contents[pragma_ends];	
	pragma_region=[]
	
	analysis_stats={}#'pragma_starts':pragma_starts,'pragma_ends':pragma_ends}
	analysis_stats['pragma_starts']=pragma_starts; 	analysis_stats['pragma_ends']=pragma_ends
	analysis_stats['num_for_loops']=0
	analysis_stats['size_dim']=[];	analysis_stats['variable_dim']=[]
	for i in range (pragma_starts,pragma_ends):
		pragma_region.append(src_file_contents[i]);
	for curr_line in pragma_region:
		matchObj=re.match(r'\s*.*for.*\(',curr_line)
		if matchObj:
			matchObj=re.match(r'\s*.*for.*\(.*\;.*\;.*\)',curr_line);			
			if matchObj:
				print "\n\t Have a properly framed \" for-loop\" "+curr_line
				analysis_stats['num_for_loops']+=1
  				#matchObj=re.findall(r'\s*.*for.*\((.*)\;.*\;',curr_line) 				#for_params=str( curr_line.split(';',3) );
  				for_params=curr_line.split(';',3)
  				matchObj=re.match(r'\s*.*for.*\((.*)',for_params[0])
 				if matchObj: 
 					first_param=matchObj.group(1)
 					terms_within_param1=first_param.split(' ')
 					#for term in terms_within_param1:
 					if(terms_within_param1[0]=='int'):
 						print "\n\t\t -- Variable: "+ terms_within_param1[1]
 						analysis_stats['variable_dim'].append(terms_within_param1[1])
 					else:
 						print "\n\t\t -- Variable: "+ terms_within_param1[0]
 						analysis_stats['variable_dim']=terms_within_param1[0] 	
 				terms_within_param2=for_params[1].split(' ')								
 				print "\n\t Second term "+for_params[1] +" length: "+str( len(terms_within_param2) )
 				print "\t "
 				term_count=0;
 				for term in terms_within_param2:
					if( term==analysis_stats['variable_dim'][( len(analysis_stats['variable_dim']) -1) ] ):
						size_param=re.match(r'\s*(\w+)+',terms_within_param2[term_count+2])
						print "\n\t Found the term "+term+" and its size is "+str(size_param.group(1))					
						analysis_stats['size_dim'].append(size_param.group(1))						
					term_count+=1
					
 				
	for i in analysis_stats:
		print "\n\t Stats: "+i+" "+str(analysis_stats[i])				
	return analysis_stats
								
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
    
    pragma_starts=[0]
    pragma_ends=[0]
    
    line_count=0;
    for curr_line in src_file_contents:
    	
    	#print "\n\t "+str(line_count)+" : "+curr_line
    	matchObj=re.match(r'\s*\#pragma',curr_line)
    	if matchObj:
    		syntax_chk=re.match(r'\s*\#pragma\s+struct_grid',curr_line)
    		print "\n\t Found pragma with struct_grid in line: "+str(line_count)+" : "+curr_line    
		if syntax_chk:
			start_chk=re.match(r'\s*\#pragma\s+struct_grid\s+start',curr_line)   
			if start_chk:
				print "\n\t Found start: "+str(line_count)+" "+curr_line
				pragma_starts[0]+=1;
				pragma_starts.append(line_count);
			elif( re.match(r'\s*\#pragma\s+struct_grid\s+end',curr_line) ):
				print "\n\t Found end: "+str(line_count)+" "+curr_line
				pragma_ends[0]+=1;
				pragma_ends.append(line_count);
	line_count=line_count+1

    if( pragma_starts[0]==pragma_ends[0] ):
	print "\n\t Number of start and end is equal: "+str(pragma_starts[0] ) + " , "+ str(pragma_ends[0] );
	# Ideally there should be only one case each of struct_grid start and end. 
	# Can check the sequence of start and end and determine whether there is any start-end pair incomplete.
    else:
   	print "\n\t Number of start and end is NOT equal: "+str(pragma_starts[0] ) + " , "+ str(pragma_ends[0] );
   	sys.exit()
	
    analysis_stats=extract_info(pragma_starts[1],pragma_ends[1],src_file_contents);

if __name__ == "__main__":
    main()
