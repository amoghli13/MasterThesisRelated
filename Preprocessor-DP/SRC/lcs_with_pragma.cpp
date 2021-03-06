#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include<vector>
#include<utility>

#include "error_inject_operator.h" 
using namespace std;

#define MAX(A,B) (((A)>(B))? (A) : (B))
/*
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
    # condn_term_key=cond+str(condition-number)
    #		*brace_start
    #		*brace_end
    #		*num_statements
    #		*statement_keywd - the method break_statement returns all the info about the STATEMENT to this key. 
    #		       statement_keywd='statement'+str(condn_params[condn_term_key]['num_statements'])
    #		*operation_count
    #		*is_else_condn
    #		*condn_line
    #		*condn_unrolled - the method break_statement returns all the info about the CONDITION to this key.
    #			*condn
  */
  
  class condns
  {
  
  public:
  	char* condn;// condition
  	int num_statements; 
  	char*  stmts; // statements
  	int test_condn; //
  };
 
// template <class M>
 class rollback_stats
 {
 
 public:
 	int num_condns;
 	int operation_count;
 	condns my_condns;
 };

int rollback(int ip_i, int ip_j,rollback_stats my_rollback_stats)
{

	vector< pair<int,int> > check_list;
	check_list.push_back(make_pair(ip_i,ip_j));

	vector< pair<int,int> >::iterator curr_pos;
	curr_pos=check_list.begin();
	int checklist_notempty=1;

	while( checklist_notempty )
	{
		int i=(*curr_pos).first;
		int j=(*curr_pos).second;

		for( int curr_condn=0;curr_condn<num_condns;curr_condn++ )
		{
			int used_condn=0;
			test_condn=1 ;// NOTICE Fill it suitably! 
			if (a[i] == b[j])
			{
			   lengths[i+1][j+1] = lengths[i][j] + 1 ;
			   used_condn=1
			}
		      #pragma dynamic_prog solve cond 2            
			else if (  lengths[i+1][j] > lengths[i][j+1] ) 
			{
			   lengths[i+1][j+1] = lengths[i+1][j] ; //+ non_sense[i];
			   used_condn=2		   
		   	}
		      #pragma dynamic_prog solve cond 3   	    
			else
			{
			   lengths[i+1][j+1] = lengths[i][j+1]; //find_this[i]] + lengths[i- find_this[i]][j];            
			   used_condn=3		   
			}

			if(used_condn==test_condn)
			{
				check_list.push_back(make_pair(i,j));
			}
		}
		
		if(curr_pos!=check_list.end)
			curr_pos++
		else
			checklist_notempty=0	
	}


	return 0;
}


 
char * lcs(const char *a,const char * b) 
{
    int lena = strlen(a)+1;
    int lenb = strlen(b)+1;
 
    int bufrlen = 40;
    char bufr[40], *result;
 
    int i,j;
    const char *x, *y;
    int *la = (int *)calloc(lena*lenb, sizeof( int));
    int  **lengths = (int **)malloc( lena*sizeof( int*));
    for (i=0; i<lena; i++) lengths[i] = la + i*lenb;
 
 #pragma dynamic_prog mat dimensions 2
 #pragma dynamic_prog solve num_conditions 3
 
 
    for (i=0,x=a; *x; i++, x++) {
        for (j=0,y=b; *y; j++,y++ ) {
      #pragma dynamic_prog solve cond 1
            if (a[i] == b[j]) {
               lengths[i+1][j+1] = lengths[i][j] + 1 ;
            }
      #pragma dynamic_prog solve cond 2            
            else if (  lengths[i+1][j] > lengths[i][j+1] ) 
            {
               lengths[i+1][j+1] = lengths[i+1][j] ; //+ non_sense[i];
   	    }
      #pragma dynamic_prog solve cond 3   	    
            else
            {
               lengths[i+1][j+1] = lengths[i][j+1]; //find_this[i]] + lengths[i- find_this[i]][j];            
            }
        }
    }
   #pragma dynamic_prog solve cond 4
    result = bufr+bufrlen;
    *--result = '\0';
    i = lena-1; j = lenb-1;
    while ( (i>0) && (j>0) ) {
        if (lengths[i][j] == lengths[i-1][j])  i -= 1;
        else if (lengths[i][j] == lengths[i][j-1]) j-= 1;
        else {
//			assert( a[i-1] == b[j-1]);
            *--result = a[i-1];
            i-=1; j-=1;
        }
    }
    #pragma dynamic_prog solve cond 5
    free(la); free(lengths);
    return strdup(result);
}

int main()
{
	rollback_stats my_rollback_stats;
    printf("%s\n", lcs("thisisatest", "testing123testing")); // tsitest
    return 0;
}

