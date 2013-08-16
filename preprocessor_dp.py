# Comments section

"""

#Pending
	1.  The parameter extraction code/logic available for '+' operator should be repeated for '-','*','/'


"""
# Actual code
import getopt, sys,re
import insert_checkpoints
import break_statement 
import extract_condn_params
import recreate_condns
 


					
			
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
	
	condn_params=extract_condn_params.extract_condn_params(src_file_contents)
	#summarize_condns(condn_params)
	recreate_condn_params=recreate_condns.recreate_condns(condn_params,src_file_contents)			    
	insert_checkpoints.insert_checkpoints(condn_params,recreate_condn_params,src_file_contents)
	
if __name__ == "__main__":
    main()    
