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
import extract_trace_params
 
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
	extract_trace_params.extract_trace_params(src_file_contents)
	
if __name__ == "__main__":
    main()    
