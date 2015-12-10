import sys,re

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
					pass_new_operator='+' #find_plus_operator[0]
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
					print "\n\t Yes an operator has been found! pass_new_operand: "+str(pass_new_operand)+" pass_new_operator "+str(pass_new_operator);

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


