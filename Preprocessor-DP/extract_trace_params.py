import sys,re
import break_statement

def extract_trace_params(src_file_contents):
# src_file_handle=open(src_file)
#    src_file_contents=src_file_handle.readlines()
#    src_file_handle.close()

    pragma_starts=[0]
    pragma_ends=[0]
    src_file_length=len(src_file_contents);
    
    line_count=0;
    conditions_found=0;
    num_conditions_found=0; # Working variable to keep track of all the conditions that have been interpreted.
    num_conditions=0;
 
    size_found=0;
    condn_params={}
    accept_choice_not_found=1
 
    inner_loop_open=0;
    inner_loop_close=0;
    size_found=0;
    condn_params={}
    
    # num_dimensions.
    for curr_line in src_file_contents:
    	
    	#print "\n\t "+str(line_count)+" : "+curr_line

    	matchObj=re.match(r'\s*\#pragma',curr_line)
    	if matchObj:
    		syntax_chk=re.match(r'\s*\#pragma\s+dynamic_prog',curr_line)
    		print "\n\t Found pragma with struct_grid in line: "+str(line_count)+" : "+curr_line    
		if syntax_chk:
			trace_chk=re.match(r'\s*\#pragma\s+dynamic_prog\s+trace',curr_line)   
			if trace_chk:
				print "\n\t Found trace "+str(line_count)+" "+curr_line
				condns_stmt=re.match(r'\s*\#pragma\s+dynamic_prog\s+trace\s+num_conditions',curr_line) 
				condn_term_key=''  
				if condns_stmt:
					print "\n\t ** Found trace num_conditions in line "+str(line_count)+ " ** "
					condns_stmt=re.match(r'\s*\#pragma\s+dynamic_prog\s+trace\s+num\_conditions\s+(\d+)+',curr_line)   
					if condns_stmt:
						num_conditions=int(condns_stmt.group(1));
						conditions_found=1;
						condn_params['num_condns']=num_conditions
						print "\n\t Found number of conditions for trace: "+str( condns_stmt.group(1) )+" num_conditions_found "+str(num_conditions_found)+' num_conditions '+str(num_conditions);						
					else:
						print "\n\t Couldn't find the actual number of conditions for trace in line: "+str(line_count)
												
				elif ( conditions_found and inner_loop_open and (not inner_loop_close ) and (num_conditions_found < num_conditions) ) :
					condns_stmt=re.match(r'\s*\#pragma\s+dynamic_prog\s+trace\s+\cond\s+(\d+)+',curr_line);
					#print "\n\t Num_conditions_found "+str(num_conditions_found)+" is less than num_condtions: "+str(num_conditions)+"\n"
					if condns_stmt:
						#if( condns_stmt.group(1)== num_conditions_found+1 ):
						
						condn_term_key='cond'+str(condns_stmt.group(1))
						print "\n\t condn_term_key is "+str(condn_term_key)+"\n"
						condn_params[condn_term_key]={}
						condn_params[condn_term_key]['trial']='trial'+str(condns_stmt.group(1))
						#condn_params['condn_term_key']={}						
						condn_params[condn_term_key]['pragma_line']=line_count;
						print "\n\t ** Found the condition number "+str(condns_stmt.group())+' !! '
						num_conditions_found+=1;#num_conditions;
						start_search_line=line_count;
						search_line_idx=line_count+1;
						braces_notcompleted=1;
						brace_notstarted=1;brace_start_line=0;
						brace_notclosed=1;brace_end_line=0;
						while (braces_notcompleted and ( search_line_idx < src_file_length ) ):
							search_line=src_file_contents[search_line_idx];
							confirm_curr_condition=re.match('\s*\#pragma\s+dynamic_prog\s+trace\s+\cond\s+(\d+)+ ',search_line)
							if confirm_curr_condition:
								print "\n\t FATAL braces did not complete before coming across next pragma dynamic_prog trace cond! \n\n "
								sys.exit()
							else:
								if brace_notstarted:
									search_braces=re.match('.*\{.*',search_line);
									if search_braces:
										print "\n\t Found brace { on line: "+str(search_line_idx);
										brace_notstarted=0
										brace_start_line=search_line_idx
										condn_params[condn_term_key]['brace_start']=brace_start_line;
									else:
										print "\n\t Did NOT find brace { on line: "+str(search_line_idx);
								elif brace_notclosed:
									search_braces=re.match('.*\}.*',search_line);
									if search_braces:
										print "\n\t Found brace } on line: "+str(search_line_idx);
										brace_notclosed=0
										braces_notcompleted=0
										brace_end_line=search_line_idx
										condn_params[condn_term_key]['brace_end']=brace_end_line;										
									else:
										print "\n\t Did NOT find brace } on line: "+str(search_line_idx);
								search_line_idx+=1;
								#print "\n\t STATUS: brace_notstarted "+str(brace_notstarted)+" brace_notclosed "+str(brace_notclosed)+" braces_notcompleted "+str(braces_notcompleted)

						if search_line_idx>=src_file_length:
							print "FATAL: braces did not complete! search line "+str(search_line_idx)+" src_file_length"+str(src_file_length)+"\n\n"
							sys.exit()
#### This part searches for the actual if-else part. Although counter-intutitve to search the condition after looking for braces, does not make any difference! 
						else:
							braces_notcompleted=1;
							brace_notstarted=1;
							brace_notclosed=1;
							search_line_idx=brace_start_line;
							while (braces_notcompleted and ( search_line_idx > start_search_line ) ):
								search_line=src_file_contents[search_line_idx];
								confirm_curr_condition=re.match('\s*\#pragma\s+dynamic_prog\s+trace\s+\cond\s+(\d+)+ ',search_line)
								if confirm_curr_condition:
									print "\n\t FATAL if(condition) not found before coming across the pragma dynamic_prog trace cond statement on line "+str( search_line_idx ) +"\n\n "
									sys.exit()
								else:
									if brace_notstarted:
										search_braces=re.match('.*\(.*\).*',search_line);
										search_bases2=re.match('\s+else\s+',search_line);
										if search_braces:
											print "\n\t Found condition within () on line: "+str(search_line_idx);
											brace_notstarted=0;
											braces_notcompleted=0;
											condn_params[condn_term_key]['condn_line']=search_line_idx
											condn_params[condn_term_key]['is_else_condn']=0	
											condn_params[condn_term_key]['my_condn']=src_file_contents[search_line_idx]										
											search_line_idx+=1 # To ensure the if(search_line_idx part does not come into play)																				
										elif search_bases2:
											brace_notstarted=0;
											braces_notcompleted=0;		
											condn_params[condn_term_key]['condn_line']=search_line_idx		
											condn_params[condn_term_key]['is_else_condn']=1	
											condn_params[condn_term_key]['my_condn']=src_file_contents[search_line_idx]																					
											search_line_idx+=1 # To ensure the if(search_line_idx part does not come into play)									
											print "\n\t Found 'else' instead of condition "+str(search_line)
										else:
											print "\n\t Did NOT find condition within () on line: "+str(search_line_idx);
								search_line_idx-=1;
							if ( search_line_idx<=start_search_line):
								print "\n\t Condition not found before reaching pragma dynamic_prog trace cond statement on line "+str( search_line_idx ) +"\n\n "
							else:
								search_solve_condn=re.match(r'\s*\#pragma\s+dynamic_prog\s+trace\s+\cond\s+(\d+)+\s+solve\s+cond\s+(\d+)',curr_line)
								if search_solve_condn:
									print "\n\t STARTED with detecting associated solve term "
								else:
									print "\n\t COULD not detect associated solve term"
 
				elif ( accept_choice_not_found ):
					accept_choice_stmt=re.match(r'\s*\#pragma\s+dynamic_prog\s+trace\accept_choice',curr_line);		
					if accept_choice_stmt:
						print "\n\t Found accept choice in line "+str(line_count)+" "+str(curr_line)
					else:
						print "\n\t Did not find accept choice in line "+str(line_count)+" "+str(curr_line)									
				else:
					print "\n\t I go nowhere!! since num_conditions_found is "+str(num_conditions_found)+' and num_conditions is '+str(num_conditions )+" :'( :'( ";			
							
 			elif ( (not inner_loop_open) or (not inner_loop_close) ):
				inner_loop_stmt=re.match(r'\s*\#pragma\s+dynamic_prog\s+inner_loop_trace\s+(\w+)+',curr_line);		
				if inner_loop_stmt:
					if ( inner_loop_stmt.group(1)=='open' ):
						print "\n\t Found inner loop-open in line "+str(line_count)+" "+str(curr_line)
						inner_loop_open=1;
						inner_loop_close=0;
						condn_params['inner_loop_open']=line_count;
					elif ( inner_loop_stmt.group(1)=='close' ):						
						print "\n\t Found inner loop-close in line "+str(line_count)+" "+str(curr_line)
						inner_loop_open=1;
						inner_loop_close=1;				
						condn_params['inner_loop_close']=line_count;								
					else:
						print "\n\t Did NOT find inner loop-open in line "+str(line_count)+" "+str(curr_line)						
				else:
					print "\n\t Did NOT find inner loop-open in line "+str(line_count)+" "+str(curr_line)	
			elif ( accept_choice_not_found ):
				accept_choice_stmt=re.match(r'\s*\#pragma\s+dynamic_prog\s+trace\accept_choice',curr_line);		
				if accept_choice_stmt:
					print "\n\t Found accept choice in line "+str(line_count)+" "+str(curr_line)
				else:
					print "\n\t Did not find accept choice in line "+str(line_count)+" "+str(curr_line)					
					# #pragma dynamic_prog mat array lengths
 	line_count+=1;						
    return condn_params
