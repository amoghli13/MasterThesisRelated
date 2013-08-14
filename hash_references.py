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
    

############## Method: recreate_condns

#return -> recreate_condn_params={}
# num_condns
# cond_term_key= cond+str(condn_num)
#	*num_statements
# statement_keywd=statement+str(statement_num)
# 	*rhs_operands_indices=[]
# 	*lhs_operand_indices=[]
#	*index_translation=[]


