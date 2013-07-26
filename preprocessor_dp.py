
import getopt, sys,re


def break_statement( search_line,operation_count,search_line_idx):
	search_stmt=re.match('.*\=.*',search_line);
	eqn_params=search_line.split('=',2)
	eqn_params[1]=re.sub('\s$','',eqn_params[1])
	operation_count_key='opcpount'+str(operation_count)
	#condn_params['condn_term_key']['operations']['operation_count_key']=[]
	print "\n\t Searching for stmt in line "+str(search_line)
	temp_trial_erase_quickly='lengths'
	if search_stmt:
		print "\n\t ----- Found following items in the statement: "+str(eqn_params[0])+" , "+str(eqn_params[1])+" on line "+str(search_line_idx)
		#condn_params['condn_term_key']['operations']['operation_count_key'].append(eqn_params[0])
		#condn_params['condn_term_key']['operations']['operation_count_key'].append(eqn_params[1])
		operands_sqbrace2_split=eqn_params[1].split(']');
		print "\n\t NODAPPA len(operands_sqbrace1_split) "+str(len(operands_sqbrace2_split) )											
		
		for i in range( len(operands_sqbrace2_split) ):
			curr_term=operands_sqbrace2_split[i].split('[')
			if curr_term:
				print "\n\t Number-of-groups in curr_term is "+str( len(curr_term) )#+" , "+str(curr_term[1])
				for j in range( len(curr_term) ):
					print "\n\t j: "+str(j)+" "+str(curr_term[j])
				find_operator=curr_term[0].split('+')  #re.match('([\+]+)*\.*',curr_term[0] )
				if (len(find_operator)>=2):
					print "\n\t -- FOUND an operator: "+str(find_operator[0] ) +" , "+str(find_operator[1])
				else:
					print "\n\t -- Did not find any operator: "+str(curr_term[0])


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
    src_file_length=len(src_file_contents);
    
    line_count=0;
    conditions_found=0;
    num_conditions_found=0; # Working variable to keep track of all the conditions that have been interpreted.
    num_conditions=0;

    dimensions_found=0;
    num_dimensions=0;
    condn_params={}
    # Keys:
    # condn_term_key=cond+str(condition-number)
    #		*brace_start
    #		*brace_end
    #		*operations
    #			*operations_count_key='opcpount'+str(operation_count)
    #		*operation_count
    # num_dimensions.
    for curr_line in src_file_contents:
    	
    	#print "\n\t "+str(line_count)+" : "+curr_line

    	matchObj=re.match(r'\s*\#pragma',curr_line)
    	if matchObj:
    		syntax_chk=re.match(r'\s*\#pragma\s+dynamic_prog',curr_line)
    		print "\n\t Found pragma with struct_grid in line: "+str(line_count)+" : "+curr_line    
		if syntax_chk:
			solve_chk=re.match(r'\s*\#pragma\s+dynamic_prog\s+solve',curr_line)   
			if solve_chk:
				print "\n\t Found solve "+str(line_count)+" "+curr_line
				condns_stmt=re.match(r'\s*\#pragma\s+dynamic_prog\s+solve\s+num_conditions',curr_line) 
				condn_term_key=''  
				if condns_stmt:
					print "\n\t ** Found solve num_conditions in line "+str(line_count)+ " ** "
					condns_stmt=re.match(r'\s*\#pragma\s+dynamic_prog\s+solve\s+num\_conditions\s+(\d+)+',curr_line)   
					if condns_stmt:
						num_conditions=int(condns_stmt.group(1));
						conditions_found=1;
						print "\n\t Found number of conditions for solve: "+str( condns_stmt.group(1) )+" num_conditions_found "+str(num_conditions_found)+' num_conditions '+str(num_conditions);						
					else:
						print "\n\t Couldn't find the actual number of conditions for solve in line: "+str(line_count)
				elif ( conditions_found and dimensions_found and (num_conditions_found < num_conditions) ) :
					condns_stmt=re.match(r'\s*\#pragma\s+dynamic_prog\s+solve\s+\cond\s+(\d+)+',curr_line);
					#print "\n\t Num_conditions_found "+str(num_conditions_found)+" is less than num_condtions: "+str(num_conditions)+"\n"
					if condns_stmt:
						#if( condns_stmt.group(1)== num_conditions_found+1 ):
						
						condn_term_key='cond'+str(condns_stmt.group(1))
						print "\n\t condn_term_key is "+str(condn_term_key)+"\n"
						condn_params['condn_term_key']={}
						print "\n\t ** Found the condition number "+str(condns_stmt.group())+' !! '
						num_conditions_found+=1;#num_conditions;
						start_search_line=line_count;
						search_line_idx=line_count+1;
						braces_notcompleted=1;
						brace_notstarted=1;brace_start_line=0;
						brace_notclosed=1;brace_end_line=0;
						while (braces_notcompleted and ( search_line_idx < src_file_length ) ):
							search_line=src_file_contents[search_line_idx];
							confirm_curr_condition=re.match('\s*\#pragma\s+dynamic_prog\s+solve\s+\cond\s+(\d+)+ ',search_line)
							if confirm_curr_condition:
								print "\n\t FATAL braces did not complete before coming across next pragma dynamic_prog solve cond! \n\n "
								sys.exit()
							else:
								if brace_notstarted:
									search_braces=re.match('.*\{.*',search_line);
									if search_braces:
										print "\n\t Found brace { on line: "+str(search_line_idx);
										brace_notstarted=0
										brace_start_line=search_line_idx
										condn_params['condn_term_key']['brace_start']=brace_start_line;
									else:
										print "\n\t Did NOT find brace { on line: "+str(search_line_idx);
								elif brace_notclosed:
									search_braces=re.match('.*\}.*',search_line);
									if search_braces:
										print "\n\t Found brace } on line: "+str(search_line_idx);
										brace_notclosed=0
										braces_notcompleted=0
										brace_end_line=search_line_idx
										condn_params['condn_term_key']['brace_end']=brace_end_line;										
									else:
										print "\n\t Did NOT find brace } on line: "+str(search_line_idx);
								search_line_idx+=1;
								#print "\n\t STATUS: brace_notstarted "+str(brace_notstarted)+" brace_notclosed "+str(brace_notclosed)+" braces_notcompleted "+str(braces_notcompleted)

						if search_line_idx>=src_file_length:
							print "FATAL: braces did not complete! search line "+str(search_line_idx)+" src_file_length"+str(src_file_length)+"\n\n"
							sys.exit()
						else:
							braces_notcompleted=1;
							brace_notstarted=1;
							brace_notclosed=1;
							search_line_idx=brace_start_line;
							while (braces_notcompleted and ( search_line_idx > start_search_line ) ):
								search_line=src_file_contents[search_line_idx];
								confirm_curr_condition=re.match('\s*\#pragma\s+dynamic_prog\s+solve\s+\cond\s+(\d+)+ ',search_line)
								if confirm_curr_condition:
									print "\n\t FATAL if(condition) not found before coming across the pragma dynamic_prog solve cond statement on line "+str( search_line_idx ) +"\n\n "
									sys.exit()
								else:
									if brace_notstarted:
										search_braces=re.match('.*\(.*\).*',search_line);
										search_bases2=re.match('\s+else\s+',search_line);
										if search_braces:
											print "\n\t Found condition within () on line: "+str(search_line_idx);
											brace_notstarted=0;
											braces_notcompleted=0;
											search_line_idx+=1 # To ensure the if(search_line_idx part does not come into play)																				
										elif search_bases2:
											brace_notstarted=0;
											braces_notcompleted=0;		
											search_line_idx+=1 # To ensure the if(search_line_idx part does not come into play)									
											print "\n\t Found 'else' instead of condition "+str(search_line)
										else:
											print "\n\t Did NOT find condition within () on line: "+str(search_line_idx);
								search_line_idx-=1;
							if ( search_line_idx<=start_search_line):
								print "\n\t Condition not found before reaching pragma dynamic_prog solve cond statement on line "+str( search_line_idx ) +"\n\n "
							else:
								search_line_idx=brace_start_line;
								print "\n\t Venturing to find statements from line "+str(search_line_idx)
								condn_params['condn_term_key']['operations']={}
								operation_count=0;
								while ( search_line_idx < brace_end_line  ):
									search_line=src_file_contents[search_line_idx];
									if (search_line_idx== brace_start_line ):
										tmp= search_line.split('}',2)
										#print "\n\t Muhahaha"+str(tmp)+" len(tmp) "+str(len(tmp));		
										if len(tmp)==2:
											search_line=tmp[1]							
										elif len(tmp)==1:
											search_line_idx+=1
											search_line=src_file_contents[search_line_idx];
									search_stmt=re.match('.*\=.*',search_line);
									search_stmt2=re.match('\s*\#pragma\s+dynamic_prog\s+solve\s+\cond\s+(\d+)+ ',search_line)
									if search_stmt2:
										print "\n\t FATAL Found pragma dynamic_prog solve cond statement on line "+str( search_line_idx )+" before end of braces! \n\t This is likely a bug, please report it."
									elif search_stmt:
										search_stmt=re.match('\s+([^=]).*',search_line)
										if search_stmt:
											break_statement(search_line,operation_count,search_line_idx)
											operation_count+=1;	
										else:
											print "\n\t WARNING: Could not locate stmt in line "+str(search_line)
									search_line_idx+=1;
								condn_params['condn_term_key']['operation_count']=operation_count;
							
				else:
					print "\n\t I go nowhere!! since num_conditions_found is "+str(num_conditions_found)+' and num_conditions is '+str(num_conditions )+" :'( :'( ";			
							
			elif ~dimensions_found:
					dims_stmt=re.match(r'\s*\#pragma\s+dynamic_prog\s+mat\s+dimensions\s+(\d+)+',curr_line);
					if dims_stmt:
						num_dimensions=dims_stmt.group(1)
						print "\n\t ^^^^ Number of dimensions is found  "+str(num_dimensions)
						dimensions_found=1;	
						condn_params['num_dimensions']=num_dimensions
					else:
						print "\n\t Searching for num-dimensions unsuccessful in line "+str(line_count)+"\n"									


 	line_count+=1;						
				    
if __name__ == "__main__":
    main()    
