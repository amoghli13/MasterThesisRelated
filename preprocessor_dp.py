# Comments section

"""

#Pending
	1.  The parameter extraction code/logic available for '+' operator should be repeated for '-','*','/'


"""
# Actual code
import getopt, sys,re
 
 

 


############ Method: break_statement(search_line_op1,search_line_op2)
 
# Return hash: return_params
#	*eqn_params
#	*lhs_operand
#	*lhs_operand_indices
#	*rhs_num_operands
#	*rhs_num_operators
#	*rhs_operands
#	*rhs_operators
#	*rhs_operands_indices



def break_statement( search_line_op1,search_line_op2):
	#search_stmt=re.match('.*\=.*',search_line);
	#eqn_params=search_line.split('=',2)
	#eqn_params[1]=re.sub('\s$','',eqn_params[1])
	eqn_params=[]
	eqn_params.append(search_line_op1)
	eqn_params.append(search_line_op2)	
	temp_trial_erase_quickly='lengths'
	return_params={}
	return_params['res']={};
	lhs_operand=eqn_params[0].split(']');
	print "\n\t LHS operands: "
	for i in range(len(lhs_operand)):
		print "\n\t # "+str(i)+" term: "+str(lhs_operand[i]);
	lhs_indices=[]
	for i in range(len(lhs_operand)-1):
		curr_term=lhs_operand[i].split('[')
		if(len(curr_term) >1):
			print "\n\t Yo! lhs-term: "+str(i)+" index--> "+str(curr_term[1])
			lhs_indices.append(curr_term[1])
		else:
			print "\n\t Something fishy going on here! "+str(lhs_operand[i])
	return_params['res']['lhs_operand']=eqn_params[0]
	return_params['res']['lhs_operand_indices']=lhs_indices
	
	pseudo_term=1
	if pseudo_term: # Commenting this to make the code usable to all statemetns seperated by =,>,>=,<,<=,==
		#print "\n\t ----- Found following items in the statement: "+str(eqn_params[0])+" , "+str(eqn_params[1])+" on line "+str(search_line_idx)
		#condn_params[condn_term_key]['operations']['operation_count_key'].append(eqn_params[0])
		#condn_params[condn_term_key]['operations']['operation_count_key'].append(eqn_params[1])
		operands_sqbrace2_split=eqn_params[1].split(']');
		print "\n\t NODAPPA len(operands_sqbrace1_split) "+str(len(operands_sqbrace2_split) )											


		return_params['res']['eqn_params']=[];
		return_params['res']['eqn_params'].append(eqn_params[0]);
		return_params['res']['eqn_params'].append(eqn_params[1])
		term_start=0
		term_end=0
		operator_found=0
		return_params['res']['rhs_num_operands']=0
		return_params['res']['rhs_num_operators']=0 # rhs_num_operators=rhs_num_operands-1
		return_params['res']['rhs_operands']=[]
		return_params['res']['rhs_operators']=[]	
		return_params['res']['rhs_operands_indices']=[]
		curr_operand=""
		curr_operand_copy=curr_operand
		pass_new_operand=""
		pass_new_operator=""
		curr_indices=[]
		for i in range( len(operands_sqbrace2_split) ):
			curr_term=operands_sqbrace2_split[i].split('[')
			if curr_term:
				print "\n\t Number-of-groups in curr_term is "+str( len(curr_term) )+" , "+str(operands_sqbrace2_split[i])
				for j in range( len(curr_term) ):
					print "\n\t j: "+str(j)+" "+str(curr_term[j])
				find_plus_operator=curr_term[0].split('+');find_minus_operator=curr_term[0].split('-');find_mul_operator=curr_term[0].split('*');find_div_operator=curr_term[0].split('/')
				if (len(find_plus_operator)>=2):
					print "\n\t -- FOUND a plus operator: "+str(find_plus_operator[0] ) +" , "+str(find_plus_operator[1])
					operator_found=1
					pass_new_operator=find_plus_operator[0]
					pass_new_operand=find_plus_operator[1]+'['
					pass_new_operand_copy=pass_new_operand
					curr_idx='';curr_idx_copy=''
					
					if( len(curr_term) >2 ):
						idx_plus_split= curr_term[1].split( '+');idx_minus_split= curr_term[1].split( '-');idx_mul_split= curr_term[1].split( '*');idx_div_split= curr_term[1].split( '/');
						if ( len(idx_plus_split) > 1):
							print "\n\t Index has plus operator!! "+str(curr_term[1])+" len(idx_split) "+str(len(idx_plus_split))
						elif ( len(idx_minus_split) > 1):
							print "\n\t Index has minus operator!! "+str(curr_term[1])+" len(idx_split) "+str(len(idx_minus_split))
						elif ( len(idx_mul_split) > 1):
							print "\n\t Index has mul operator!! "+str(curr_term[1])+" len(idx_split) "+str(len(idx_mul_split))	
						if ( len(idx_div_split) > 1):
							print "\n\t Index has div operator!! "+str(curr_term[1])+" len(idx_split) "+str(len(idx_div_split))		
							
 						for k in range( len(curr_term) -2):
							curr_idx=curr_idx_copy+str(curr_term[k+1])+'['
							curr_idx_copy=curr_idx
							pass_new_operand=pass_new_operand_copy+str(curr_term[k+1])+'['
							pass_new_operand_copy=pass_new_operand
							print "\n\t Found an operator-- so currently pass_new_operand "+str(curr_idx)
						curr_idx=curr_idx_copy+str(curr_term[len(curr_term)-1])+']'
						curr_idx_copy=curr_idx
						pass_new_operand=pass_new_operand_copy+str(curr_term[len(curr_term)-1])+']'
						pass_new_operand_copy=pass_new_operand
						#num_open_braces-=1;
						#curr_indices.append(curr_idx);
						print "\n\t ++ Current idx: "+str(curr_idx)+"\t and pass_new_operand is "+str(pass_new_operand)
					else:
					
						#duh=re.match('\.*\;\.*',find_plus_operator[1])#curr_term[0])
						check_semicolon=curr_term[0].split(';')
						curr_idx=curr_idx_copy+str(curr_term[len(curr_term)-1])
						curr_idx_copy=curr_idx
						if ( len(check_semicolon) > 1 ):
							remove_semicolon=find_plus_operator[1].split(';')
							pass_new_operand=remove_semicolon[0] #check_semicolon[0]#find_plus_operator[1]
							pass_new_operand_copy=pass_new_operand		
							curr_idx=''
							curr_idx_copy=''				
							print "\n\t ++ Current idx: "+str(curr_idx)+"\t and pass_new_operand is "+str(pass_new_operand)							
						else:
							pass_new_operand=pass_new_operand_copy+str(curr_term[len(curr_term)-1])+']'
							pass_new_operand_copy=pass_new_operand
						#num_open_braces-=1;
							print "\n\t -- Current idx: "+str(curr_idx)+"\t and pass_new_operand is "+str(pass_new_operand)
					print "\n\t Disassemble the following operator!! \n"
					for k in range( len(find_plus_operator) ):
						print "\n\t K: "+str(k)+" "+str(find_plus_operator[k])
					
				elif (len(find_minus_operator)>=2):
					print "\n\t -- FOUND a minus operator: "+str(find_minus_operator[0] ) +" , "+str(find_minus_operator[1])
					operator_found=1	
					operator_found=1
					pass_new_operator=find_minus_operator[0]
					pass_new_operand=find_minus_operator[1]+'['
					pass_new_operand_copy=pass_new_operand
					curr_idx='';curr_idx_copy=''
					for k in range( len(curr_term) -2):
						curr_idx=curr_idx_copy+str(curr_term[k+1])+'['
						curr_idx_copy=curr_idx
						print "\n\t Found an operator-- so currently pass_new_operand "+str(curr_idx)

					curr_idx=curr_idx_copy+str(curr_term[len(curr_term)-1])+']'
					curr_idx_copy=curr_idx
					pass_new_operand=pass_new_operand_copy+str(curr_idx)
					print "\n\t Current idx: "+str(curr_idx)										
				elif (len(find_mul_operator)>=2):
					print "\n\t -- FOUND a mul operator: "+str(find_mul_operator[0] ) +" , "+str(find_mul_operator[1])		
					operator_found=1					
					pass_new_operator=find_mul_operator[0]
					pass_new_operand=find_mul_operator[1]+'['
					pass_new_operand_copy=pass_new_operand
					curr_idx='';curr_idx_copy=''
					for k in range( len(curr_term) -2):
						curr_idx=curr_idx_copy+str(curr_term[k+1])+'['
						curr_idx_copy=curr_idx
						print "\n\t Found an operator-- so currently pass_new_operand "+str(curr_idx)

					curr_idx=curr_idx_copy+str(curr_term[len(curr_term)-1])+']'
					curr_idx_copy=curr_idx
					pass_new_operand=pass_new_operand_copy+str(curr_idx)
					print "\n\t Current idx: "+str(curr_idx)			
				elif (len(find_div_operator)>=2):
					print "\n\t -- FOUND a div operator: "+str(find_div_operator[0] ) +" , "+str(find_div_operator[1])									
					operator_found=1			
					operator_found=1
					pass_new_operator=find_div_operator[0]
					pass_new_operand=find_div_operator[1]+'['
					pass_new_operand_copy=pass_new_operand
					curr_idx='';curr_idx_copy=''
					for k in range( len(curr_term) -2):
						curr_idx=curr_idx_copy+str(curr_term[k+1])+'['
						curr_idx_copy=curr_idx
						print "\n\t Found an operator-- so currently pass_new_operand "+str(curr_idx)

					curr_idx=curr_idx_copy+str(curr_term[len(curr_term)-1])+']'
					curr_idx_copy=curr_idx
					pass_new_operand=pass_new_operand_copy+str(curr_idx)
					print "\n\t Current idx: "+str(curr_idx)										
				else:
					if(len(curr_term) >1 ):
						if(len(curr_term)>2):
							idx_plus_split= curr_term[1].split( '+');idx_minus_split= curr_term[1].split( '-');idx_mul_split= curr_term[1].split( '*');idx_div_split= curr_term[1].split( '/');
							if ( len(idx_plus_split) > 1):
								print "\n\t Index has plus operator!! "+str(curr_term[1])+" len(idx_split) "+str(len(idx_plus_split))
							elif ( len(idx_minus_split) > 1):
								print "\n\t Index has minus operator!! "+str(curr_term[1])+" len(idx_split) "+str(len(idx_minus_split))
							elif ( len(idx_mul_split) > 1):
								print "\n\t Index has mul operator!! "+str(curr_term[1]+" len(idx_split) ")+str(len(idx_mul_split))	
							if ( len(idx_div_split) > 1):
								print "\n\t Index has div operator!! "+str(curr_term[1])+" len(idx_split) "+str(len(idx_div_split))														
							curr_idx=''
							curr_idx_copy=curr_idx
							curr_operand=curr_operand_copy+curr_term[0]
							curr_operand_copy=curr_operand
							for j in range( len(curr_term) -2):
								#print "\n\t -- Now- curr_operand"
								curr_idx=curr_idx_copy+str(curr_term[j+1])+'['
								curr_idx_copy=curr_idx		
								curr_operand=curr_operand_copy+'['+str(curr_term[j+1])
								curr_operand_copy=curr_operand
							curr_idx=curr_idx_copy+str(curr_term[len(curr_term)-1] )+']'
							curr_idx_copy=curr_idx
							#curr_operand=curr_operand_copy+curr_term[0]+'['+str(curr_term[len(curr_term)-1] )+']'
							curr_operand=curr_operand_copy+'['+str(curr_term[len(curr_term)-1] )+']'									
							curr_operand_copy=curr_operand
							curr_indices.append(curr_idx)
							print "\n\t ++ Did not find any operator: "+str(curr_term[0])+" and curr_operand is "+str(curr_operand)+" Current idx is "+str(curr_idx)	
						else:
							curr_idx=''
							curr_idx_copy=curr_idx
							curr_idx=curr_idx_copy+str(curr_term[len(curr_term)-1] )
							curr_idx_copy=curr_idx
							curr_operand=curr_operand_copy+curr_term[0]+'['+str(curr_term[len(curr_term)-1] )+']'
							curr_operand_copy=curr_operand
							curr_indices.append(curr_idx)
							print "\n\t -- Did not find any operator: "+str(curr_term[0])+" and curr_operand is "+str(curr_operand)+" Current idx is "+str(curr_idx)	
									
					else:
						b4=curr_term[0];
					        #curr_term[0]=re.sub('\s*$','',curr_term[0])
					        print "\n\t b4: "+str(b4)+" now-- "+str(curr_term[0])+" end"
						test_semicolon=curr_term[0].split(';')
						if (len(test_semicolon) ==1 ) :
							if(test_semicolon[0]==''):
								print "\n\t Space found instead of semicolon detected!! "+str(curr_term[0])
								curr_operand=curr_operand_copy+']'
								curr_operand_copy=curr_operand
							else:
								print "\n\t FATAL no semicolon/space in "+str(curr_term[0])	
						else: 
							only_semicolon_found=1;
							for semicolon_idx in range(len( test_semicolon ) ):
								only_space=re.match('\s*^\s*$',test_semicolon[semicolon_idx])
								if( not only_space ):
									only_semicolon_found=0
								print "\n\t semicolon-idx "+str(semicolon_idx)+" "+str(test_semicolon[semicolon_idx])+" only_semicolon_found: "+str(only_semicolon_found)
							duh=re.match(';$',curr_term[0])
							if( only_semicolon_found ):
								print "\n\t Hurray Semicolon detected!! \n";
								return_params['res']['rhs_operands'].append(curr_operand)
								return_params['res']['rhs_num_operands']=return_params['res']['rhs_num_operands']+1
								#return_params['res']['rhs_operands_indices'][return_params['res']['rhs_num_operators']]=curr_indices								
								return_params['res']['rhs_operands_indices'].append(curr_indices)
								curr_indices=[]
								curr_operand=pass_new_operand
								curr_operand_copy=pass_new_operand																
							else:
								print "\n\t FATAL: Semicolon detection has a BUG!!!! "+str(curr_term[0])+" length of test_semicolon "+str(len(test_semicolon))+" "+str(test_semicolon[0])+" "+str(test_semicolon[1])+" end"
								#print "\n\t Semicolon found!! "
								
						#else:
						#	print "\n\t FATAL: Semicolon detection has a BUG!!!! "+str(curr_term[0])+" length of test_semicolon "+str(len(test_semicolon))
				if operator_found:
					print "\n\t Yes an operator has been found! pass_new_operand: "+str(pass_new_operand);

					operator_found=0
					return_params['res']['rhs_operands'].append(curr_operand)					
					return_params['res']['rhs_operators'].append(pass_new_operator);
					#return_params['res']['rhs_operands_indices'][return_params['res']['rhs_num_operators']]=curr_indices
					return_params['res']['rhs_operands_indices'].append(curr_indices)					
					curr_indices=[]							
					if curr_idx:
						curr_indices.append(curr_idx)					

					return_params['res']['rhs_num_operands']=return_params['res']['rhs_num_operands']+1
					return_params['res']['rhs_num_operators']=return_params['res']['rhs_num_operators']+1					
					curr_operand=pass_new_operand
					curr_operand_copy=pass_new_operand

					if ( i==( len(operands_sqbrace2_split) -1 ) ):
					       
						print "\n\t And this is the last term!!! "+str(curr_term[0])+"\n"
						return_params['res']['rhs_num_operands']=return_params['res']['rhs_num_operands']+1
						return_params['res']['rhs_operands'].append(pass_new_operand)
						if curr_idx:
							return_params['res']['rhs_operands_indices'].append(curr_idx)
						else:
							curr_indices=[0];
							return_params['res']['rhs_operands_indices'].append(curr_indices)	
					
					pass_new_operand=''
					pass_new_operator=""			

														
					#return_params
		rhs_num_operands=return_params['res']['rhs_num_operands']			
		for i in range(return_params['res']['rhs_num_operands']):
			print "\n\t -- NOTICE return_params['res']['rhs_operands'][i] is "+str(return_params['res']['rhs_operands'][i])+" and the indices are"
			for k in range( len( return_params['res']['rhs_operands_indices'][i] ) ):		
				print "\n\t\t index-no: "+str(k)+" index --> "+str(return_params['res']['rhs_operands_indices'][i][k])
		return return_params;


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
									result_params=break_statement(find_condn_greater_or_eq_operator[0],find_condn_greater_or_eq_operator[1])								
									condn_params[condn_term_key]['condn_unrolled']=result_params['res']
									condn_params[condn_term_key]['condn_unrolled']['condn']='>='
								elif (len(find_condn_lesser_or_equal_operator)>1):
									print "\n\t --- CONDN ALERT found a lesser than or equal operator! "+str(condn_line_contents)+" term-0: "+str(find_condn_lesser_or_equal_operator[0])+" term-1: "+str(find_condn_lesser_or_equal_operator[1])								
									duh=find_condn_lesser_or_equal_operator[0].split('(') # Should have only 2 parts
									find_condn_lesser_or_equal_operator[0]=duh[1]
									duh=find_condn_lesser_or_equal_operator[1].split(')')
									find_condn_lesser_or_equal_operator[1]=duh[0]+';'									
									result_params=break_statement(find_condn_lesser_or_equal_operator[0],find_condn_lesser_or_equal_operator[1])										
									condn_params[condn_term_key]['condn_unrolled']=result_params['res']
									condn_params[condn_term_key]['condn_unrolled']['condn']='<='									
								elif (len(find_condn_great_operator)>1):
									print "\n\t --- CONDN ALERT found a greater than operator! "+str(condn_line_contents)+" term-0: "+str(find_condn_great_operator[0])+" term-1: "+str(find_condn_great_operator[1])							
									duh=find_condn_great_operator[0].split('(') # Should have only 2 parts
									find_condn_great_operator[0]=duh[1]
									duh=find_condn_great_operator[1].split(')')
									find_condn_great_operator[1]=duh[0]+';'
									print "\n\t Before sending find_condn_great_operator[0] "+str(find_condn_great_operator[0])+" find_condn_great_operator[1] "+str(find_condn_great_operator[1])
									result_params=break_statement(find_condn_great_operator[0],find_condn_great_operator[1])		
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
									result_params=break_statement(find_condn_equal_operator[0],find_condn_equal_operator[1])									
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
												return_params=break_statement(search_stmt[0],search_stmt[1])  #(search_line,operation_count,search_line_idx)
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

############## Method: idx_breakdown

#idx_info
#	*idx_breakdown=[]
#	*idx_breakdown_operations=[]
#	*rest_of_the_idx=[]

def idx_breakdown(idx,idx_to_match):
	idx_plus_split=idx.split('+');idx_minus_split=idx.split('-'); # Neglect these for now! idx_mul_split=idx.split('*');idx_div_split=idx.split('/')
	#idx_excess=0
	idx_breakdown=[]
	idx_operations_breakdown=[]
	if( len(idx_plus_split) > 1 ): # ASSUMPTION: There is only one operator possible between iterative variable i and rest.
		#indexed_variable_notfound=1
		for m in range( len(idx_plus_split) ):
			# check_indexed_variable=idx_plus_split[m].split('[') # It should not matter whether the other term is indexed variable or not! :-o
			check_minus_variable=idx_plus_split[m].split('-')			
			if( len(check_minus_variable)>1):
				for n in range( len(check_minus_variable) ):
					idx_breakdown.append(check_minus_variable[n])
					idx_operations_breakdown.append('-')					
				print "\n\t Index also has a minus term !! "				
			else:
				idx_breakdown.append(idx_plus_split[m])
				idx_operations_breakdown.append('+')
	elif( len(idx_minus_split) > 1 ): # ASSUMPTION: There is only one operator possible between iterative variable i and rest.
		#indexed_variable_notfound=1
		for m in range( len(idx_minus_split) ):
			# check_indexed_variable=idx_plus_split[m].split('[') # It should not matter whether the other term is indexed variable or not! :-o
			check_plus_variable=idx_plus_split[m].split('-')			
			if( len(check_plus_variable)>1):
				for n in range( len(check_plus_variable) ):
					idx_breakdown.append(check_plus_variable[n])
					idx_operations_breakdown.append('+')					
				print "\n\t Index also has a plus term !! "				
			else:
				idx_breakdown.append(idx_plus_split[m])	
				idx_operations_breakdown.append('+')							
	else:
		idx_breakdown.append(idx_plus_split[0])
	idx_info={}
	idx_info['idx_breakdown']=idx_breakdown;
	idx_info['idx_breakdown_operations']=idx_operations_breakdown
	
	idx_terms_length=len(idx_info['idx_breakdown'] );

	#idx_match_term;
	rest_of_the_idx=''
	for m in range(idx_terms_length):
		if( idx_info['idx_breakdown'][m]==idx_to_match ):
			idx_match_term=m		

	if( idx_match_term==0):
		print "\n\t CASE-1 "
		for m in range(idx_terms_length-2):
			rest_of_the_idx=rest_of_the_idx+str(idx_info['idx_breakdown_operations'][m])+str(idx_info['idx_breakdown'][m+1])
		if( idx_terms_length-1 ):
			rest_of_the_idx=rest_of_the_idx+str(idx_info['idx_breakdown'][idx_terms_length-1])
	elif( idx_match_term==(idx_terms_length-1) ):
		print "\n\t CASE-2 "
		for m in range(idx_terms_length-2):
			rest_of_the_idx=rest_of_the_idx+str(idx_info['idx_breakdown'][m])+str(idx_info['idx_breakdown_operations'][m])
		rest_of_the_idx=rest_of_the_idx+str(idx_info['idx_breakdown'][idx_terms_length-2])
	else:
		print "\n\t CASE-3 "
		for m in range(idx_terms_length-1):
			if( not ( (m)==idx_match_term ) ):
				if( not ( (m+1)==idx_match_term ) ):
					rest_of_the_idx=rest_of_the_idx+str(idx_info['idx_breakdown'][m])+str(idx_info['idx_breakdown_operations'][m])
				else:
					rest_of_the_idx=rest_of_the_idx+str(idx_info['idx_breakdown'][m])+str(idx_info['idx_breakdown_operations'][m+1])
					m=m+1
			print "\n\t m: "+str(m)+" rest_of_the_idx "+str(rest_of_the_idx)
		rest_of_the_idx=rest_of_the_idx+str(idx_info['idx_breakdown'][idx_terms_length-1])
	print "\n\t idx: "+idx+" idx_variable: "+idx_to_match+" and rest of the idx: "+str(rest_of_the_idx)
	idx_info['rest_of_the_idx']=rest_of_the_idx
	return idx_info
	
####################### Method: insert_checkpoints ####################

def insert_checkpoints(condn_params,recreate_condn_params,src_file_contents):
	print "\n\t [[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[ "
	print "\n\t Number of dimensions in the given problem: "+str(condn_params['num_dimensions'])
	for i in range(condn_params['num_dimensions']):
		print "\n\t Idx-# "+str(i)+" index: "+str(condn_params['indices'][i])+" size "+str(condn_params['size'][i]);
	print "\n\t Inner-loop open: "+str(condn_params['inner_loop_open'])+" close: "+str(condn_params['inner_loop_close'])+" \n"
	for_loop_line=condn_params['inner_loop_open'] + 1;
	for_loop_not_found=1;
	curr_line= src_file_contents[for_loop_line];	
	while for_loop_not_found:
		curr_line= src_file_contents[for_loop_line];
		is_for_loop=re.match(r'\s*.*for.*\(.*\;.*\;.*\)',curr_line);
		if is_for_loop:
			print "\n\t Found a for loop in line no: "+str(for_loop_line)+" "+str(curr_line);
			for_loop_not_found=0;
		else:
			for_loop_line+=1;
	print "\n\t Start inserting stuff until this line: "+str(for_loop_line);
	checkpoint_length= ( condn_params['size'][ (condn_params['num_dimensions'] -1) ] / 20 );
	print "\n\t Without bounding, the checkpoint length would be "+str(checkpoint_length);
	if ( checkpoint_length < 10 ):
		checkpoint_length=10;
	elif (checkpoint_length > 25):
		checkpoint_length=25;
	print 	"\n\t With bounding, the checkpoint length would be "+str(checkpoint_length);	
	num_checkpoints= ( condn_params['size'][ (condn_params['num_dimensions'] -1) ] / checkpoint_length );
 
 	for i in range(recreate_condn_params['num_condns'] ):
 		cond_term_key='cond'+str(i+1)
 		if( recreate_condn_params['update_condn']['condn_num']==i ):
 			print "\n\t Current condition is the update condition!! Condn: "+str(condn_params[cond_term_key]['my_condn']);
 		for j in range( recreate_condn_params[cond_term_key]['num_statements'] ):
			statement_keywd='statement'+str(j+1)
			for k in range( len(recreate_condn_params[cond_term_key][statement_keywd]['index_translation']) ):
				print "\n\t Cond: "+str(i)+" statement: "+str(j)+" index: "+str(k)+" index-translation: "+str(recreate_condn_params[cond_term_key][statement_keywd]['index_translation'][k])
	
############## Method: recreate_condns

#return -> recreate_condn_params={}
# num_condns
# update_condn
#	*condn_num
#	*condn_line
# cond_term_key= cond+str(condn_num)
#	*num_statements
# statement_keywd=statement+str(statement_num)
# 	*rhs_operands_indices=[]
# 	*lhs_operand_indices=[]
#	*index_translation=[]

def recreate_condns(condn_params,src_file_contents):
	print "\n\t ------------------------------------------------------------------------------------------ "
	print "\n\t Number of dimensions in the given problem: "+str(condn_params['num_dimensions'])
	for i in range(condn_params['num_dimensions']):
		print "\n\t Idx-# "+str(i)+" index: "+str(condn_params['indices'][i])+" size "+str(condn_params['size'][i]);
	print "\n\t Inner-loop open: "+str(condn_params['inner_loop_open'])+" close: "+str(condn_params['inner_loop_close'])+" \n"
	print "\n\t Summarizing the condtions: \n "
	print "\n\t Number of conditions: "+str(condn_params['num_condns'])
	

	condn_stmts_4rollback=[]
	curr_set_condn_stmts=[]
	for i in range(condn_params['num_condns']):
		print "\n\t I am condition number: "+str(i)
		condn_term_key='cond'+str(i+1)
		statement_keywd='statement'+str(1) # NOTICE: This should be parameterized!!
		print "\n\t\t Condn: "+str(condn_params[condn_term_key]['condn_line'])
		print "\n\t\t Is else condn: "+str(condn_params[condn_term_key]['is_else_condn'])
		if not (condn_params[condn_term_key]['is_else_condn']):
			condn_expression=str(condn_params[condn_term_key]['condn_unrolled']['lhs_operand'])+' '
			print "\n\t\t\t LHS: "+str(condn_params[condn_term_key]['condn_unrolled']['lhs_operand'])
			condn_expression=condn_expression+str(condn_params[condn_term_key]['condn_unrolled']['condn'])+'  '
			for j in range(condn_params[condn_term_key]['condn_unrolled']['rhs_num_operands']):
				print "\n\t\t\t RHS-operator "+str(j)+' --> '+str(condn_params[condn_term_key]['condn_unrolled']['rhs_operands'][j])+" and the indices are"
				condn_expression=condn_expression+str(condn_params[condn_term_key]['condn_unrolled']['rhs_operands'][j])+'  '
				for k in range( len( condn_params[condn_term_key]['condn_unrolled']['rhs_operands_indices'][j] ) ):		
					print "\n\t\t\t\t index-no: "+str(k)+" index --> "+str(condn_params[condn_term_key]['condn_unrolled']['rhs_operands_indices'][j][k])	

			print "\n\t Condition expression: "+str(condn_expression)
			print "\n\t Condition :"+str(condn_params[condn_term_key]['my_condn'])
			curr_set_condn_stmts.append(condn_params[condn_term_key]['my_condn'])
			curr_set_condn_stmts.append('{')
		else:
			curr_set_condn_stmts.append('else')	
			curr_set_condn_stmts.append('{')
			"""
					rhs_num_operands=return_params['res']['rhs_num_operands']			
		for i in range(return_params['res']['rhs_num_operands']):
			print "\n\t -- NOTICE return_params['res']['rhs_operands'][i] is "+str(return_params['res']['rhs_operands'][i])+" and the indices are"
			for k in range( len( return_params['res']['rhs_operands_indices'][i] ) ):		
				print "\n\t\t index-no: "+str(k)+" index --> "+str(return_params['res']['rhs_operands_indices'][i][k])
			
			"""
		print "\n\t\t statement: "+str(condn_params[condn_term_key][statement_keywd]['search_line'])
		local_stmt=str('\t')+str(condn_params[condn_term_key][statement_keywd]['search_line'])
		curr_set_condn_stmts.append(local_stmt)
		print "\n\t\t\t LHS: "+str(condn_params[condn_term_key][statement_keywd]['eqn_params'][0])+"\t RHS: "+str(condn_params[condn_term_key][statement_keywd]['eqn_params'][1])
		for j in range(condn_params[condn_term_key][statement_keywd]['rhs_num_operands']):
			print "\n\t\t\t RHS-operator "+str(j)+' --> '+str(condn_params[condn_term_key][statement_keywd]['rhs_operands'][j])+" and the indices are"
			for k in range( len( condn_params[condn_term_key][statement_keywd]['rhs_operands_indices'][j] ) ):		
				print "\n\t\t\t\t index-no: "+str(k)+" index --> "+str(condn_params[condn_term_key][statement_keywd]['rhs_operands_indices'][j][k])	
		test_condn=i+1
		set_condn_num= str('\t')+' test_condn='+str(test_condn)
		print "\n\t Inserting set_condn_num: "+str(set_condn_num)+" test_condn: "+str(test_condn)+" i: "+str(i)+" condn_term_key "+condn_term_key
		curr_set_condn_stmts.append(set_condn_num)
		curr_set_condn_stmts.append('}')
		condn_stmts_4rollback.append(curr_set_condn_stmts)
		curr_set_condn_stmts=[]
	
	print "\n\t ----------------------------------------------------------------------------------- \n"
	print "\n\t These are the condition(s) and other statments going to the method int rollback() \n"
	for i in range( len(condn_stmts_4rollback) ): # This should be equal to condn_params['num_condns']
		for j in range(len( condn_stmts_4rollback[i] ) ):
			print "\n\t "+str(condn_stmts_4rollback[i][j])

	# Alternate logic to produce conditions block! 			
	"""print "\n\t ----------------------------------------------------------------------------------- \n"			
	for i in range(condn_params['num_condns']):
		print "\n\t I am condition number: "+str(i)
		condn_term_key='cond'+str(i+1)
		statement_keywd='statement'+str(1)
		#if not (condn_params[condn_term_key]['is_else_condn']):
		print "\n\t "+str(condn_params[condn_term_key]['my_condn'])  #+str(condn_params[condn_term_key]['condn_unrolled']['condn'] )
		#else:
		#	print "\n\t "+'else'
		for j in range(condn_params[condn_term_key]['brace_start'],condn_params[condn_term_key]['brace_end']):
			print str(src_file_contents[j])
		test_condn=i+1
		set_condn_num= str('\t')+' test_condn='+str(test_condn)
		print "\t"+str(set_condn_num)			
		print str(src_file_contents[condn_params[condn_term_key]['brace_end']])
	"""	
	dependency_indices=[]	
	curr_dependency_indices=[]
	update_condn_not_found=1;
	update_condn_line=0;

	recreate_condn_params={}
	
	recreate_condn_params['num_condns']=condn_params['num_condns']
	recreate_condn_params['update_condn']={}			
	
	for i in range(condn_params['num_condns']):
		condn_term_key='cond'+str(i+1)
		recreate_condn_params[condn_term_key]={}
		recreate_condn_params[condn_term_key]['num_statements']=condn_params[condn_term_key]['num_statements']		
		print "\n\t NEW CONDN ALERT \n"
		for j in range(condn_params[condn_term_key]['num_statements']):
			statement_keywd='statement'+str(j+1)
			recreate_condn_params[condn_term_key][statement_keywd]={}
			recreate_condn_params[condn_term_key][statement_keywd]['lhs_operand_indices']=	[]
			recreate_condn_params[condn_term_key][statement_keywd]['lhs_operand_indices']= condn_params[condn_term_key][statement_keywd]['lhs_operand_indices'];			
			for k in range( len(condn_params[condn_term_key][statement_keywd]['lhs_operand_indices']) ):
				print "\n\t LHS-index: "+str(condn_params[condn_term_key][statement_keywd]['lhs_operand_indices'][k])
			lhs_operand_split=condn_params[condn_term_key][statement_keywd]['lhs_operand'].split('[')
			lhs_operand_split[0]=re.sub('^\s*','',lhs_operand_split[0])
			if( lhs_operand_split[0] == condn_params['fill_array'] ):
				print "\n\t The lhs_operand is same as that of 'fill_array' "+str(lhs_operand_split[0]) # ASSUMPTION: LHS operand should be that of 'fill_array'
			else:
				print "\n\t The lhs_operand is NOT same as that of 'fill_array' "+str(lhs_operand_split[0])

			print "\n\t There are "+str(condn_params[condn_term_key][statement_keywd]['rhs_num_operands'])+" number of RHS operands. "				
			if( condn_params[condn_term_key][statement_keywd]['rhs_num_operands'] > 1 ):
				if( update_condn_not_found):							
					condn_params['update_condn']=condn_params[condn_term_key];
					update_condn_not_found=0;
					update_condn_line= condn_params[condn_term_key]['condn_line'];
					recreate_condn_params['update_condn']['condn_num']=i
					recreate_condn_params['update_condn']['condn_line']=update_condn_line					
					print "\n\t %$^& The update equation for the problem is at: "+str(update_condn_line);
				else:
					print "\n\n\t ERROR: The Dynamic Programming problem has more than one update equation. \n\t\t The previous update equation was found at "+str(update_condn_line)
					print "\n\t\t The problem seems to be Polyadic and is not supported by this parser. \n\n"
					sys.exit()
			for k in range(condn_params[condn_term_key][statement_keywd]['rhs_num_operands']):
				rhs_operand_split=condn_params[condn_term_key][statement_keywd]['rhs_operands'][k].split('[')
				rhs_operand_split[0]=re.sub('^\s*','',rhs_operand_split[0])
				operand_same_as_fill_array=0;
				if( rhs_operand_split[0] == condn_params['fill_array'] ):
					operand_same_as_fill_array=1
					print "\n\t The rhs_operand is same as that of 'fill_array' "+str(rhs_operand_split[0])
				else:
					print "\n\t The rhs_operand is NOT same as that of 'fill_array' "+str(rhs_operand_split[0])	
				if( not (condn_params[condn_term_key][statement_keywd]['rhs_operands_indices'][k][0]==0) ):
					recreate_condn_params[condn_term_key][statement_keywd]['rhs_operands_indices']=	[]
					recreate_condn_params[condn_term_key][statement_keywd]['rhs_operands_indices']= condn_params[condn_term_key][statement_keywd]['rhs_operands_indices'][k]
					recreate_condn_params[condn_term_key][statement_keywd]['index_translation']=[]
					for l in range( len(condn_params[condn_term_key][statement_keywd]['rhs_operands_indices'][k]) ):
						rhs_idx=condn_params[condn_term_key][statement_keywd]['rhs_operands_indices'][k][l]
						rhs_idx_info=idx_breakdown(rhs_idx,condn_params['indices'][l] )
						rhs_idx_terms_length=len(rhs_idx_info['idx_breakdown'])
						if( len(rhs_idx_info['idx_breakdown'])>2):
							print "\n\n\t ERROR: The RHS operand has an index "+str(rhs_idx)+" with more than two terms in it. This feature is currently not supported \n\n"
							sys.exit()
						lhs_idx=condn_params[condn_term_key][statement_keywd]['lhs_operand_indices'][l] 
						lhs_idx_info=idx_breakdown(lhs_idx,condn_params['indices'][l] )
						lhs_idx_terms_length=len(lhs_idx_info['idx_breakdown'])						
						use_idx=0#( rhs_idx- lhs_idx)
						print "\n\t LHS-idx has "+str(lhs_idx_terms_length)+" terms and RHS-idx "+str(rhs_idx)+" has "+str(rhs_idx_terms_length)+" terms!! "	
 						if( rhs_operand_split[0] == condn_params['fill_array'] ):
 							difference_in_idx=str( rhs_idx_info['rest_of_the_idx'] )+' - '+str(lhs_idx_info['rest_of_the_idx'])
 							recreate_condn_params[condn_term_key][statement_keywd]['index_translation'].append(difference_in_idx)
 							print "\n\t Difference in idx: "+str(difference_in_idx)
								
  			
	return recreate_condn_params								
					
			
############## Method: summarize_condns	
def summarize_condns(condn_params):
	print "\n\t ------------------------------------------------------------------------------------------ "
	print "\n\t Summarizing the condtions: \n "
	print "\n\t Number of conditions: "+str(condn_params['num_condns'])
	
	for i in range(condn_params['num_condns']):
		print "\n\t I am condition number: "+str(i)
		condn_term_key='cond'+str(i+1)
		statement_keywd='statement'+str(1)
		print "\n\t\t Condn: "+str(condn_params[condn_term_key]['condn_line'])
		print "\n\t\t Is else condn: "+str(condn_params[condn_term_key]['is_else_condn'])
		if not (condn_params[condn_term_key]['is_else_condn']):
			print "\n\t\t\t LHS: "+str(condn_params[condn_term_key]['condn_unrolled']['lhs_operand'])
			for i in range(condn_params[condn_term_key]['condn_unrolled']['rhs_num_operands']):
				print "\n\t\t\t RHS-operator "+str(i)+' --> '+str(condn_params[condn_term_key]['condn_unrolled']['rhs_operands'][i])+" and the indices are"
				for k in range( len( condn_params[condn_term_key]['condn_unrolled']['rhs_operands_indices'][i] ) ):		
					print "\n\t\t\t\t index-no: "+str(k)+" index --> "+str(condn_params[condn_term_key]['condn_unrolled']['rhs_operands_indices'][i][k])	

			"""
					rhs_num_operands=return_params['res']['rhs_num_operands']			
		for i in range(return_params['res']['rhs_num_operands']):
			print "\n\t -- NOTICE return_params['res']['rhs_operands'][i] is "+str(return_params['res']['rhs_operands'][i])+" and the indices are"
			for k in range( len( return_params['res']['rhs_operands_indices'][i] ) ):		
				print "\n\t\t index-no: "+str(k)+" index --> "+str(return_params['res']['rhs_operands_indices'][i][k])
			
			"""
		print "\n\t\t statement: "+str(condn_params[condn_term_key][statement_keywd]['search_line'])
		print "\n\t\t\t LHS: "+str(condn_params[condn_term_key][statement_keywd]['eqn_params'][0])+"\t RHS: "+str(condn_params[condn_term_key][statement_keywd]['eqn_params'][1])
		for i in range(condn_params[condn_term_key][statement_keywd]['rhs_num_operands']):
			print "\n\t\t\t RHS-operator "+str(i)+' --> '+str(condn_params[condn_term_key][statement_keywd]['rhs_operands'][i])+" and the indices are"
			for k in range( len( condn_params[condn_term_key][statement_keywd]['rhs_operands_indices'][i] ) ):		
				print "\n\t\t\t\t index-no: "+str(k)+" index --> "+str(condn_params[condn_term_key][statement_keywd]['rhs_operands_indices'][i][k])		
	




############# Main method
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
 	src_file_handle=open(src_file)
    	src_file_contents=src_file_handle.readlines()
    	src_file_handle.close()
	
	condn_params=extract_condn_params(src_file_contents)
	#summarize_condns(condn_params)
	recreate_condn_params=recreate_condns(condn_params,src_file_contents)			    
	insert_checkpoints(condn_params,recreate_condn_params,src_file_contents)
	
if __name__ == "__main__":
    main()    
