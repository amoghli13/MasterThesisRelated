import sys,re

#######################Method: rest_of_the_idx ####################
def rest_of_the_rhs( idx_info, idx_info_operations,idx_to_match):
	rest_of_the_rhs=''
	idx_terms_length=len(idx_info)
	for m in range(idx_terms_length):
		print "\n\t Operand: "+str(idx_info[m])+" is searched for "+str(idx_to_match)
		duh=idx_info[m].split('[')
		duh[0]=re.sub('^\s*','',duh[0])
		duh[0]=re.sub('\s*$','',duh[0])		
		if( duh[0]==idx_to_match ):
			idx_match_term=m	
			print "\n\t This Operand: "+str(idx_info[m])	
	
	#return;
	idx_link_operator=''
	for m in range(idx_terms_length-1):
		print "\n\t Operator-num: "+str(m)+" operator: "+str(idx_info_operations[m])
	
	if( idx_match_term==0):
		print "\n\t CASE-1 idx_terms_length: "+str(idx_terms_length)+" idx_info_operations[0] "+str(idx_info_operations[0])
		if (idx_terms_length-1):
			idx_link_operator=idx_info_operations[0]
		for m in range(idx_terms_length-1):
			rest_of_the_rhs=rest_of_the_rhs+str(idx_info_operations[m])+str(idx_info[m+1])

	elif( idx_match_term==(idx_terms_length-1) ):
		print "\n\t CASE-2 "
		if (idx_terms_length-1):
			idx_link_operator=idx_info_operations[0]		
		for m in range(idx_terms_length-2):
			rest_of_the_rhs=rest_of_the_rhs+str(idx_info[m])+str(idx_info_operations[m])
		rest_of_the_rhs=rest_of_the_rhs+str(idx_info[idx_terms_length-2])
	else:
		print "\n\t CASE-3 "
		# This case is not tested! 
		for m in range(idx_terms_length-1):
			if( not ( (m)==idx_match_term ) ):
				if( not ( (m+1)==idx_match_term ) ):
					rest_of_the_rhs=rest_of_the_rhs+str(idx_info[m])+str(idx_info_operations[m])
				else:
					rest_of_the_rhs=rest_of_the_rhs+str(idx_info[m])+str(idx_info_operations[m+1])
			else:
				idx_link_operator=idx_info_operations[m-1]
			print "\n\t m: "+str(m)+" rest_of_the_rhs "+str(rest_of_the_rhs)
		rest_of_the_rhs=rest_of_the_rhs+str(idx_info[idx_terms_length-1])
	
	print "\n\t rest_of_the_rhs: "+str(rest_of_the_rhs)+" idx_link_operator "+str(idx_link_operator)
	return rest_of_the_rhs;


	
	
####################### Method: insert_checkpoints ####################

def insert_checkpoints(condn_params,recreate_condn_params,src_file_contents):
	print "\n\t [[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[ In Module [[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[ "
	print "\n\t Number of dimensions in the given problem: "+str(condn_params['num_dimensions'])
	for i in range(condn_params['num_dimensions']):
		print "\n\t Idx-# "+str(i)+" index: "+str(condn_params['indices'][i])+" size "+str(condn_params['size'][i]);
	print "\n\t Inner-loop open: "+str(condn_params['inner_loop_open'])+" close: "+str(condn_params['inner_loop_close'])+" \n"
	for_loop_line=condn_params['inner_loop_open'] + 1;
	curr_line= src_file_contents[for_loop_line];	

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
 		condn_term_key='cond'+str(i+1)
 		if( recreate_condn_params['update_condn']['condn_num']==i ):
 			print "\n\t Current condition is the update condition!! Condn: "+str(condn_params[condn_term_key]['my_condn']);
	 		for j in range( recreate_condn_params[condn_term_key]['num_statements'] ):
				statement_keywd='statement'+str(j+1)
				for k in range( len(condn_params[condn_term_key][statement_keywd]['lhs_operand_indices']) ):
					print "\n\t Cond: "+str(i)+" statement: "+str(j)+" lhs-index-num: "+str(k)+" lhs-index: "+str( condn_params[condn_term_key][statement_keywd]['lhs_operand_indices'][k] )
				for m in range( condn_params[condn_term_key][statement_keywd]['rhs_num_operands']): 	
					print "\n\t Currently considering the operand-num: "+str(m)+" and the operand is "+str(condn_params[condn_term_key][statement_keywd]['rhs_operands'][m])+" and it has "
					for k in range( len( condn_params[condn_term_key][statement_keywd]['rhs_operands_indices'][m] ) ):		
						print "\n\t Cond: "+str(i)+" statement: "+str(j)+" operand-num: "+str(m)+" rhs-index-num: "+str(k)+" rhs-index: "+str( condn_params[condn_term_key][statement_keywd]['rhs_operands_indices'][m][k] )#+" and it has "+str( len(recreate_condn_params[condn_term_key][statement_keywd]['rhs_idx_info'][k]['idx_breakdown']) ) +" term(s)"
					rhs_operand=condn_params[condn_term_key][statement_keywd]['rhs_operands'][m]
					operand_name=rhs_operand.split('[')
					operand_name[0]=re.sub('^\s*','',operand_name[0])
					if(condn_params['fill_array'] == operand_name[0]):
						print "\n\t Operand-num "+str(m)+" is same as 'fill_array' "+str(operand_name[0])
						#idx_cleared=1
						for k in range( len( condn_params[condn_term_key][statement_keywd]['rhs_operands_indices'][m] ) ):						
							for l in range(len( recreate_condn_params[condn_term_key][statement_keywd]['rhs_idx_info'][m][k]['idx_breakdown'] ) ):
								print "\n\t Index-num: "+str(k)+" term-no: "+str(l)+" term: "+str(recreate_condn_params[condn_term_key][statement_keywd]['rhs_idx_info'][m][k]['idx_breakdown'][l])
								this_idx=recreate_condn_params[condn_term_key][statement_keywd]['rhs_idx_info'][m][k]['idx_breakdown'][l] 						
								if( not (condn_params['indices'][k]==this_idx) ):
									this_idx=re.sub('^\s*','',this_idx)
									this_idx=re.sub('\s*$','',this_idx)
									this_idx_analysis=this_idx.split('[')
									print "\n\t This-idx: "+str(this_idx)+"--"
									if(len(this_idx_analysis)>1):
										print "\n\t ERROR: In the Update condition, in the operand "+str(m)+"\t The index term "+str(this_idx)+" is not dimensional-index-variable but seems to be indexed in itself! "
										print "\n\t This feature is currently not supported by the tool. "
										#idx_cleared=0
										sys.exit()
									#elif( not (isinstance( this_idx,( int,long) ) ) ):
									try:
										int(this_idx)
										#this_idx+=1
									except ValueError:
										int_check=isinstance( this_idx, int )
										#int_check2=int(this_idx)
										print "\n\t ERROR: In the Update condition, in the operand "+str(m)+"\t The index term "+str(this_idx)+" is not dimensional-index-variable but seems to be non-integer term! "+" int-check "+str(int_check)+"--"
										print "\n\t This feature is currently not supported by the tool. "
										sys.exit()										
									else:
										print "\n\t This index term "+str(this_idx)+" is not dimensional-index-variable but seems to NOT be indexed and is an integer in itself! "
									
					else:
						print "\n\t Operand-num "+str(m)+" is NOT same as 'fill_array' "+str(operand_name[0])					
						if(len(operand_name)>2):
							print "\n\t ERROR: In the Update condn's stmt: "+str(j)+" the operand "+str(m)+" is two-dimensional, which is an unexpected variable. "
							sys.exit()
				
				rest_of_the_rhs_res=rest_of_the_rhs( condn_params[condn_term_key][statement_keywd]['rhs_operands'],condn_params[condn_term_key][statement_keywd]['rhs_operators'],condn_params['fill_array'] )		
				print "\n\t Rest_of_the_rhs: "+str(rest_of_the_rhs_res)

	for_loop_not_found=1
	while for_loop_not_found:
		curr_line= src_file_contents[for_loop_line];
		is_for_loop=re.match(r'\s*.*for.*\(.*\;.*\;.*\)',curr_line);
		if is_for_loop:
			print "\n\t Found a for loop in line no: "+str(for_loop_line)+" "+str(curr_line);
			for_loop_not_found=0;
		else:
			for_loop_line+=1;		
