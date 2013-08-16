import sys,re

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
#	*rhs_idx_info={}
# 	*lhs_operand_indices=[]
#	*lhs_idx_info={}
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
					
			recreate_condn_params[condn_term_key][statement_keywd]['rhs_operands_indices']=	[]
			#recreate_condn_params[condn_term_key][statement_keywd]['rhs_operands_indices']= condn_params[condn_term_key][statement_keywd]['rhs_operands_indices'][k]
			recreate_condn_params[condn_term_key][statement_keywd]['index_translation']=[]
			recreate_condn_params[condn_term_key][statement_keywd]['rhs_idx_info']=[]
			recreate_condn_params[condn_term_key][statement_keywd]['lhs_idx_info']=[]					
			for k in range(condn_params[condn_term_key][statement_keywd]['rhs_num_operands']):
				recreate_condn_params[condn_term_key][statement_keywd]['rhs_idx_info'].append([])
				recreate_condn_params[condn_term_key][statement_keywd]['lhs_idx_info'].append([])			
				rhs_operand_split=condn_params[condn_term_key][statement_keywd]['rhs_operands'][k].split('[')
				rhs_operand_split[0]=re.sub('^\s*','',rhs_operand_split[0])
				operand_same_as_fill_array=0;
				if( rhs_operand_split[0] == condn_params['fill_array'] ):
					operand_same_as_fill_array=1
					print "\n\t The rhs_operand is same as that of 'fill_array' "+str(rhs_operand_split[0])
				else:
					print "\n\t The rhs_operand is NOT same as that of 'fill_array' "+str(rhs_operand_split[0])	
				if( not (condn_params[condn_term_key][statement_keywd]['rhs_operands_indices'][k][0]==0) ):

					for l in range( len(condn_params[condn_term_key][statement_keywd]['rhs_operands_indices'][k]) ):
						rhs_idx=condn_params[condn_term_key][statement_keywd]['rhs_operands_indices'][k][l]
						rhs_idx_info=idx_breakdown(rhs_idx,condn_params['indices'][l] )
						#recreate_condn_params[condn_term_key][statement_keywd]['rhs_idx_info'][k]={}
						recreate_condn_params[condn_term_key][statement_keywd]['rhs_idx_info'][k].append(rhs_idx_info)
						rhs_idx_terms_length=len(rhs_idx_info['idx_breakdown'])
						if( len(rhs_idx_info['idx_breakdown'])>2):
							print "\n\n\t ERROR: The RHS operand has an index "+str(rhs_idx)+" with more than two terms in it. This feature is currently not supported \n\n"
							sys.exit()
						lhs_idx=condn_params[condn_term_key][statement_keywd]['lhs_operand_indices'][l] 
						lhs_idx_info=idx_breakdown(lhs_idx,condn_params['indices'][l] )
						#"\n\t "operand_num
						#recreate_condn_params[condn_term_key][statement_keywd]['lhs_idx_info'][l]={}
						recreate_condn_params[condn_term_key][statement_keywd]['lhs_idx_info'][k].append(lhs_idx_info)
						lhs_idx_terms_length=len(lhs_idx_info['idx_breakdown'])						
						use_idx=0#( rhs_idx- lhs_idx)
						print "\n\t LHS-idx has "+str(lhs_idx_terms_length)+" terms and RHS-idx "+str(rhs_idx)+" has "+str(rhs_idx_terms_length)+" terms!! "	
 						if( rhs_operand_split[0] == condn_params['fill_array'] ):
 							difference_in_idx=str( rhs_idx_info['rest_of_the_idx'] )+' - '+str(lhs_idx_info['rest_of_the_idx'])
 							recreate_condn_params[condn_term_key][statement_keywd]['index_translation'].append(difference_in_idx)
 							print "\n\t Difference in idx: "+str(difference_in_idx)
								
  			
	return recreate_condn_params								

