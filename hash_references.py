##### Temp-reference remove it soon

# Return hash: return_params
#	*eqn_params
#	*lhs_operand
#	*lhs_operand_indices
#	*rhs_num_operands
#	*rhs_num_operators
#	*rhs_operands
#	*rhs_operators
#	*rhs_operands_indices


    # Keys for return (dictionary!)
    # num_condns
    # fill_array
    # condn_term_key=cond+str(condition-number)
    #		*brace_start
    #		*brace_end
    #		*num_statements
    #		*statement_keywd - the method break_statement returns all the info about the STATEMENT to this key. 
    #		       statement_keywd='statement'+str(condn_params[condn_term_key]['num_statements'])
    #		* search_line
    #		*operation_count
    #		*is_else_condn
    #		*condn_line
    #		*condn_unrolled - the method break_statement returns all the info about the CONDITION to this key.
    #			*condn
    #		*my_condn
    
    
						rhs_idx_plus_split=rhs_idx.split('+');rhs_idx_minus_split=rhs_idx.split('-'); # Neglect these for now! rhs_idx_mul_split=rhs_idx.split('*');rhs_idx_div_split=rhs_idx.split('/')
						#rhs_idx_excess=0
						rhs_idx_breakdown=[]
						if( len(rhs_idx_plus_split) > 1 ): # ASSUMPTION: There is only one operator possible between iterative variable i and rest.
							#indexed_variable_notfound=1
							for m in range( len(rhs_idx_plus_split) ):
								# check_indexed_variable=rhs_idx_plus_split[m].split('[') # It should not matter whether the other term is indexed variable or not! :-o
								check_minus_variable=rhs_idx_plus_split[m].split('-')								
								if( len(check_minus_variable)>1):
									for n in range( len(check_minus_variable) ):
										rhs_idx_breakdown.append(check_minus_variable[n])
									print "\n\t Index also has a minus term !! "									
								else:
									rhs_idx_breakdown.append(rhs_idx_plus_split[m])
						elif( len(rhs_idx_minus_split) > 1 ): # ASSUMPTION: There is only one operator possible between iterative variable i and rest.
							#indexed_variable_notfound=1
							for m in range( len(rhs_idx_minus_split) ):
								# check_indexed_variable=rhs_idx_plus_split[m].split('[') # It should not matter whether the other term is indexed variable or not! :-o
								check_plus_variable=rhs_idx_plus_split[m].split('-')								
								if( len(check_plus_variable)>1):
									for n in range( len(check_plus_variable) ):
										rhs_idx_breakdown.append(check_plus_variable[n])
									print "\n\t Index also has a plus term !! "									
								else:
									rhs_idx_breakdown.append(rhs_idx_plus_split[m])									
						else:
							rhs_idx_breakdown.append(rhs_idx_plus_split[0])    
