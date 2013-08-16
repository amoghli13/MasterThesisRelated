import sys,re
import break_statement

############ Method: extract_condn_params(src_file):

    # Keys for return (dictionary!)
    # num_condns
    # fill_array
    # indices
    # size=[]
    # num_dimensions
    # inner_loop_open
    # inner_loop_close    
    # condn_term_key=cond+str(condition-number)
    #		*brace_start
    #		*brace_end
    #		*num_statements
    #		*statement_keywd - the method break_statement returns all the info about the STATEMENT to this key.
    #		*operation_count
    #		*is_else_condn
    #		*condn_line
    #		*condn_unrolled - the method break_statement returns all the info about the CONDITION to this key.
    #			*condn
    #		*my_condn

    
 
#def extract_condn_params(src_file):



def extract_condn_params(src_file_contents):
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

    dimensions_found=0;
    array_found=0;
    num_dimensions=0;
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
						condn_params['num_condns']=num_conditions
						print "\n\t Found number of conditions for solve: "+str( condns_stmt.group(1) )+" num_conditions_found "+str(num_conditions_found)+' num_conditions '+str(num_conditions);						
					else:
						print "\n\t Couldn't find the actual number of conditions for solve in line: "+str(line_count)
												
				elif ( conditions_found and dimensions_found and size_found and inner_loop_open and (not inner_loop_close ) and array_found and (num_conditions_found < num_conditions) ) :
					condns_stmt=re.match(r'\s*\#pragma\s+dynamic_prog\s+solve\s+\cond\s+(\d+)+',curr_line);
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
								print "\n\t Condition not found before reaching pragma dynamic_prog solve cond statement on line "+str( search_line_idx ) +"\n\n "
							else:
								condn_line_contents_b4correction=src_file_contents[condn_params[condn_term_key]['condn_line']]
								condn_line_contents_front_space=re.sub('^\s*','',condn_line_contents_b4correction)
								condn_line_contents=re.sub('\s*$','',condn_line_contents_front_space)
								find_condn_great_operator=condn_line_contents.split('>');find_condn_greater_or_eq_operator=condn_line_contents.split('>=');
								find_condn_less_operator=condn_line_contents.split('<');find_condn_lesser_or_equal_operator=condn_line_contents.split('<=');
								find_condn_equal_operator=condn_line_contents.split('==');
								condn_params[condn_term_key]['condn_unrolled']={}
								if (len(find_condn_greater_or_eq_operator)>1):
									print "\n\t --- CONDN ALERT found a greater than or equal operator! "+str(condn_line_contents)+" term-0: "+str(find_condn_greater_or_eq_operator[0])+" term-1: "+str(find_condn_greater_or_eq_operator[1])
									duh=find_condn_greater_or_eq_operator[0].split('(') # Should have only 2 parts
									find_condn_greater_or_eq_operator[0]=duh[1]
									duh=find_condn_greater_or_eq_operator[1].split(')')
									find_condn_greater_or_eq_operator[1]=duh[0]+';'
									result_params=break_statement.break_statement(find_condn_greater_or_eq_operator[0],find_condn_greater_or_eq_operator[1])								
									condn_params[condn_term_key]['condn_unrolled']=result_params['res']
									condn_params[condn_term_key]['condn_unrolled']['condn']='>='
								elif (len(find_condn_lesser_or_equal_operator)>1):
									print "\n\t --- CONDN ALERT found a lesser than or equal operator! "+str(condn_line_contents)+" term-0: "+str(find_condn_lesser_or_equal_operator[0])+" term-1: "+str(find_condn_lesser_or_equal_operator[1])								
									duh=find_condn_lesser_or_equal_operator[0].split('(') # Should have only 2 parts
									find_condn_lesser_or_equal_operator[0]=duh[1]
									duh=find_condn_lesser_or_equal_operator[1].split(')')
									find_condn_lesser_or_equal_operator[1]=duh[0]+';'									
									result_params=break_statement.break_statement(find_condn_lesser_or_equal_operator[0],find_condn_lesser_or_equal_operator[1])										
									condn_params[condn_term_key]['condn_unrolled']=result_params['res']
									condn_params[condn_term_key]['condn_unrolled']['condn']='<='									
								elif (len(find_condn_great_operator)>1):
									print "\n\t --- CONDN ALERT found a greater than operator! "+str(condn_line_contents)+" term-0: "+str(find_condn_great_operator[0])+" term-1: "+str(find_condn_great_operator[1])							
									duh=find_condn_great_operator[0].split('(') # Should have only 2 parts
									find_condn_great_operator[0]=duh[1]
									duh=find_condn_great_operator[1].split(')')
									find_condn_great_operator[1]=duh[0]+';'
									print "\n\t Before sending find_condn_great_operator[0] "+str(find_condn_great_operator[0])+" find_condn_great_operator[1] "+str(find_condn_great_operator[1])
									result_params=break_statement.break_statement(find_condn_great_operator[0],find_condn_great_operator[1])		
									condn_params[condn_term_key]['condn_unrolled']=result_params['res']								
									print "\n\t --**&&^^  condn_params[condn_term_key]['condn_unrolled']['eqn_params'][0] "+str(condn_params[condn_term_key]['condn_unrolled']['eqn_params'][0])
									condn_params[condn_term_key]['condn_unrolled']['condn']='>'									
									
								elif (len(find_condn_less_operator)>1):
									print "\n\t --- CONDN ALERT found a lesser than operator! "+str(condn_line_contents)+" term-0: "+str(find_condn_less_operator[0])+" term-1: "+str(find_condn_less_operator[1])															
									duh=find_condn_less_operator[0].split('(') # Should have only 2 parts
									find_condn_less_operator[0]=duh[1]
									duh=find_condn_less_operator[1].split(')')
									find_condn_less_operator[1]=duh[0]+';'
									result_param=break_statement(find_condn_less_operator[0],find_condn_less_operator[1])
									condn_params[condn_term_key]['condn_unrolled']=result_params['res']
									condn_params[condn_term_key]['condn_unrolled']['condn']='<'									
									
								elif (len(find_condn_equal_operator)>1):
									print "\n\t --- CONDN ALERT found a logical-equal operator! "+str(condn_line_contents)+" term-0: "+str(find_condn_equal_operator[0])+" term-1: "+str(find_condn_equal_operator[1])															
									duh=find_condn_equal_operator[0].split('(') # Should have only 2 parts
									find_condn_equal_operator[0]=duh[1]
									duh=find_condn_equal_operator[1].split(')')
									find_condn_equal_operator[1]=duh[0]+';'
									result_params=break_statement.break_statement(find_condn_equal_operator[0],find_condn_equal_operator[1])									
									condn_params[condn_term_key]['condn_unrolled']=result_params['res']
									condn_params[condn_term_key]['condn_unrolled']['condn']='=='									
									
								elif (condn_params[condn_term_key]['is_else_condn']):
									print "\n\t --- CONDN ALERT found an else operator! "+str(condn_line_contents)																																																
								else:
									print "\n\t --- CONDN ALERT did NOT find a conditional operator! "+str(condn_line_contents)									
 								search_line_idx=brace_start_line;
								print "\n\t Venturing to find statements from line "+str(search_line_idx)
								statement_num=0
								statement_keywd='statement'+str(0)
								condn_params[condn_term_key][statement_keywd]={}
								#condn_params[condn_term_key][statement_keywd]['duh']='duh'
								#condn_params[condn_term_key]['statement_keywd']={}
								condn_params[condn_term_key]['num_statements']=0
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
											search_line_dummy=re.sub('\s*$','',search_line)
											search_line_dummy2=re.sub('^\s*','',search_line_dummy)
											search_line=search_line_dummy2
											search_stmt=search_line.split('=') # SHOULD have only 2 parts!!
											if (len(search_stmt)==2):
												condn_params[condn_term_key]['num_statements']=condn_params[condn_term_key]['num_statements']+1;
												statement_keywd='statement'+str(condn_params[condn_term_key]['num_statements'])									
												return_params=break_statement.break_statement(search_stmt[0],search_stmt[1])  #(search_line,operation_count,search_line_idx)
											else:
												print "\n\t Search_line has following number of equals "+str( len(search_stmt) -1 )
												sys.exit()										
											condn_params[condn_term_key][statement_keywd]=return_params['res']
											condn_params[condn_term_key][statement_keywd]['search_line']=search_line_dummy2											
											print "\n\t condn_params[condn_term_key][statement_keywd']['eqn_params'][0] "+str(condn_params[condn_term_key][statement_keywd]['eqn_params'][0]);
										else:
											print "\n\t WARNING: Could not locate stmt in line "+str(search_line)
									search_line_idx+=1;
							
				else:
					print "\n\t I go nowhere!! since num_conditions_found is "+str(num_conditions_found)+' and num_conditions is '+str(num_conditions )+" :'( :'( ";			
							
			elif not dimensions_found:
					dims_stmt=re.match(r'\s*\#pragma\s+dynamic_prog\s+mat\s+dimensions\s+(\d+)+',curr_line);
					if dims_stmt:
						num_dimensions=dims_stmt.group(1)
						print "\n\t ^^^^ Number of dimensions is found  "+str(num_dimensions)
						dimensions_found=1;	
						condn_params['num_dimensions']=int(num_dimensions)
						dims_stmt=curr_line.split('dimensions')  #re.match(r'\s*\#pragma\s+dynamic_prog\s+mat\s+dimensions\s+(\d+)+\s+(.*)+',curr_line);
						if dims_stmt:
							dummy=dims_stmt[1].split(' ');
							if ( len(dummy) > 1 ): #indices=
								print "\n\t After that I find: "+str( dummy[len(dummy)-1] );
								indices=dummy[len(dummy)-1].split(',');
								condn_params['indices']=[]
								for index in range( len(indices) ):
									print "\n\t Idx-# "+str(index)+" index: "+str(indices[index])+" --";
									local_duh=re.sub('\s*$','',indices[index])
									condn_params['indices'].append(local_duh);
					else:
						print "\n\t Searching for num-dimensions unsuccessful in line "+str(line_count)+" dimensions_found "+str(dimensions_found)+"\n"	
			elif not size_found:
					size_stmt=re.match(r'\s*\#pragma\s+dynamic_prog\s+mat\s+size\s+',curr_line);
					if dims_stmt:
						print "\n\t Size of dimensions is found  "+str(curr_line)
 						dims_stmt=curr_line.split('size')  #re.match(r'\s*\#pragma\s+dynamic_prog\s+mat\s+dimensions\s+(\d+)+\s+(.*)+',curr_line);
						if dims_stmt:
							print "\n\t After that I find: "+str( dummy[len(dummy)-1] );
							sizes=dims_stmt[1].split(',') #dummy[len(dummy)-1].split(',');
							condn_params['size']=[]
							for index in range( len(sizes) ):
								print "\n\t Idx-# "+str(index)+" index: "+str(sizes[index]);
								local_duh1=re.sub('^\s*','',sizes[index])
								local_duh=re.sub('\s*$','',local_duh1)
								local_duh=int(local_duh)
								condn_params['size'].append(local_duh)
							size_found=1
					else:
						print "\n\t Searching for size unsuccessful in line "+str(line_count)+" dimensions_found "+str(size_found)+"\n"																			
			elif not array_found:
					array_stmt=re.match(r'\s*\#pragma\s+dynamic_prog\s+mat\s+array\s+(\w+)+',curr_line);#\s+(\w+)+
					if array_stmt:
						#num_dimensions=dims_stmt.group(1)
						print "\n\t ^^^^ Array is found  "+str(array_stmt.group(1))
						array_found=1;
						condn_params['fill_array']=array_stmt.group(1)
					else:
						print "\n\t Searching for array unsuccessful in line "+str(line_count)+" array_found "+str(array_found)+"\n"	
			elif ( (not inner_loop_open) or (not inner_loop_close) ):
				inner_loop_stmt=re.match(r'\s*\#pragma\s+dynamic_prog\s+inner_loop_solve\s+(\w+)+',curr_line);		
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

					# #pragma dynamic_prog mat array lengths
 	line_count+=1;						
    return condn_params
