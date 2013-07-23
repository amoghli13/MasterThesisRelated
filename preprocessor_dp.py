
import getopt, sys,re

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
            if( re.match(r'\s*.*\.(cpp|cc|c)',src_file,re.I) ):
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
    conditions_found=0;
    num_conditions_found=0; # Working variable to keep track of all the conditions that have been interpreted.
    num_conditions=0;

    for curr_line in src_file_contents:
    	
    	#print "\n\t "+str(line_count)+" : "+curr_line
 	line_count+=1;
    	matchObj=re.match(r'\s*\#pragma',curr_line)
    	if matchObj:
    		syntax_chk=re.match(r'\s*\#pragma\s+dynamic_prog',curr_line)
    		print "\n\t Found pragma with struct_grid in line: "+str(line_count)+" : "+curr_line    
		if syntax_chk:
			solve_chk=re.match(r'\s*\#pragma\s+dynamic_prog\s+solve',curr_line)   
			if solve_chk:
				print "\n\t Found solve "+str(line_count)+" "+curr_line
				condns_stmt=re.match(r'\s*\#pragma\s+dynamic_prog\s+solve\s+num_conditions',curr_line)   
				if condns_stmt:
					print "\n\t ** Found solve num_conditions in line "+str(line_count)+ " ** "
					condns_stmt=re.match(r'\s*\#pragma\s+dynamic_prog\s+solve\s+num\_conditions\s+(\d+)+',curr_line)   
					if condns_stmt:
						num_conditions=int(condns_stmt.group(1));
						conditions_found=1;
						print "\n\t Found number of conditions for solve: "+str( condns_stmt.group(1) )+" num_conditions_found "+str(num_conditions_found)+' num_conditions '+str(num_conditions);						
					else:
						print "\n\t Couldn't find the actual number of conditions for solve in line: "+str(line_count)
				elif ( conditions_found and (num_conditions_found < num_conditions) ) :
					condns_stmt=re.match(r'\s*\#pragma\s+dynamic_prog\s+solve\s+\cond\s+(\d+)+',curr_line);
					print "\n\t Num_conditions_found "+str(num_conditions_found)+" is less than num_condtions: "+str(num_conditions)+"\n"
					if condns_stmt:
						#if( condns_stmt.group(1)== num_conditions_found+1 ):
						print "\n\t ** Found the condition number "+str(condns_stmt.group())+' !! '
						num_conditions_found+=1;#num_conditions;
				else:
					print "\n\t I go nowhere!! since num_conditions_found is "+str(num_conditions_found)+' and num_conditions is '+str(num_conditions )+" :'( :'( ";

						
				    
if __name__ == "__main__":
    main()    
