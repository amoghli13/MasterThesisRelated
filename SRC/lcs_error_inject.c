
//Author: Amoghavarsha Suresh

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#define MAX(A,B) (((A)>(B))? (A) : (B))
#define percent_error(x) ( ( ( (double) rand()/RAND_MAX) < x ) ? 1 : 0)
#define XOR(x,y) ( (x&&(!y)) || ( (!x) && y ) )

#define size_stack 40
#define size_stack_minus1 (size_stack-1)

int  **lengths;
int lena,lenb;
FILE* save_mat;
int error_inject_count;
int ops_count;

typedef struct
{
	int i;
	int j;
}xy_tuple;

typedef struct
{
	xy_tuple my_tuple;
	char	my_char;
}tuple_chars;

typedef struct
{
	xy_tuple my_tuple;
	int result_position_adjust;
}rollback_tuple_pos;

tuple_chars* accepted_tuple_chars;
int top_of_stack;
int end_of_stack;
int count_stack;

rollback_tuple_pos search_stack(xy_tuple fixing_end_tuple,int result_pos)
{
	int i;
	printf("\n\t ALERT: Top-of-stack: %d and looking to rollback around i: %d j: %d and result_pos: %d ",top_of_stack,fixing_end_tuple.i,fixing_end_tuple.j,result_pos);
	int result_position_adjust=0;
	rollback_tuple_pos return_tuple_pos;	
	for(i=top_of_stack,result_position_adjust=0;i!=end_of_stack;i=((i+size_stack_minus1)%size_stack),result_position_adjust++)
	{
			//printf("\n\t i: %d x: %d y: %d char: %c",i,accepted_tuple_chars[i].my_tuple.x,accepted_tuple_chars[i].my_tuple.y,accepted_tuple_chars[i].my_char);
		if( ( accepted_tuple_chars[i].my_tuple.i >= fixing_end_tuple.i ) && (accepted_tuple_chars[i].my_tuple.j >= fixing_end_tuple.j) )
		{

			return_tuple_pos.my_tuple.i=accepted_tuple_chars[i].my_tuple.i;
			return_tuple_pos.my_tuple.j=accepted_tuple_chars[i].my_tuple.j;		
			return_tuple_pos.result_position_adjust=result_position_adjust;	
			end_of_stack=(top_of_stack+1)%size_stack;	
			printf("\n\t Should rollback to this point *STACK SUGGESTIONS* i: %d x: %d y: %d char: %c result_position_adjust: %d",i,accepted_tuple_chars[i].my_tuple.i,accepted_tuple_chars[i].my_tuple.j,accepted_tuple_chars[i].my_char,result_position_adjust);				
			return return_tuple_pos;
		}
		accepted_tuple_chars[i].my_tuple.i=0;
		accepted_tuple_chars[i].my_tuple.j=0;
		top_of_stack=((top_of_stack+size_stack_minus1)%size_stack);
	}

	return_tuple_pos.my_tuple.i=accepted_tuple_chars[end_of_stack].my_tuple.i;	
	return_tuple_pos.my_tuple.j=accepted_tuple_chars[end_of_stack].my_tuple.j;		
	return_tuple_pos.result_position_adjust=size_stack;		
	printf("\n\t FATAL: stack overflow!!! :D \n");
	top_of_stack=-1;end_of_stack=0;
return return_tuple_pos;

}


int search_in_row(const char* a,const char* b,int xfix,double error_percent,int curr_row_i,int start_j)
{
	int i,j;
	i=curr_row_i;
	const char* x,*y;
	int error_inject;
	x=(a+curr_row_i-1);
	int lengths_ixfix=lengths[i][xfix-1]; // NOTE: lengths_ixfix has value of lengths[i][xfix-1]
	for(j=start_j,y=(b+start_j-1);( (j<xfix) && ( 1 ) );j++,y++)
	{
		error_inject=percent_error(error_percent);
		int before_lenghts=lengths[i][j];
		if ( XOR( (*x == *y), error_inject ) )
		{
				lengths[i][j] = lengths[i-1][j-1] +1;
			ops_count++;
		}
		
		else
		{
			int ml = MAX(lengths[i-1][j], lengths[i][j-1]);
			lengths[i][j] = ml;
		}			

		printf("\n\t fixing_func i:%d j: %d x: %c y: %c lengths[i][j]: %d before_lenghts: %d error_inject: %d ",i,j,*x,*y,lengths[i][j],before_lenghts,error_inject);

	}
	
	
// NOTE: lengths[i][j]=lengths_ixfix		
// Assumption is that lengths[i][j] was faulty (j=yfix),and it was corrected in the above for-loop, hence lengths[i][j] being equal to lengths[i-1][j+1] implies lengths[i][j]  was propogated in lengths[i][j+1]. 
	error_inject=percent_error(error_percent);
	int need2search,column_search;
	if( XOR( ( *x==*(b+xfix-1)),error_inject) )
	{
		lengths[i][xfix]=lengths[i-1][xfix-1]+1;
		need2search=1;
		column_search=xfix+1;
		printf("\n\t ^^fixing_func i:%d j: %d x: %c y: %c lengths[i][j]: %d error_inject: %d ",i,xfix,*x,*(b+xfix-1),lengths[i][xfix],error_inject);

	}
	else 
	{
		if( lengths_ixfix > lengths[i-1][xfix] ) //
		{
			lengths[i][xfix]=lengths[i-1][xfix];
			need2search=1;
			column_search=xfix+1;
			printf("\n\t ^^fixing_func^^ i:%d j: %d x: %c y: %c lengths[i-1][xfix]: %d lengths_ixfix: %d error_inject: %d ",i,xfix,*x,*(b+xfix-1),lengths[i-1][xfix],lengths_ixfix,error_inject);				
		}
		else
		{
			need2search=0;
			printf("\n\t ^^fixing_func** i:%d j: %d x: %c y: %c lengths[i-1][xfix]: %d lengths_ixfix: %d error_inject: %d ",i,xfix,*x,*(b+xfix-1),lengths[i-1][xfix],lengths_ixfix,error_inject);				
		}
		
	}
	
	int char_comp=0;
	int lengths_comp=0;
	while( (need2search) && (column_search < lenb) )
	{
		char_comp=( *x==*(b+column_search-1) );
		lengths_comp=(lengths[i][column_search-1]==lengths[i-1][column_search] );
		if ( (!lengths_comp) && (!char_comp) )
		{
			printf("\n\t --fixing_func-y- I: %d J: %d lengths[i-1][j]: %d and x: %c y: %c",i,column_search,lengths[i-1][column_search],*x,*(b+column_search-1));
			lengths[i][column_search]=lengths[i-1][column_search];			
			column_search++;
		}
		else
		{
			printf("\n\t ++fixing_func+y+ I: %d J: %d lengths[i-1][j]: %d and x: %c y: %c",i,column_search,lengths[i-1][column_search],*x,*(b+column_search-1));
			
			need2search=0;
		}
		xfix=column_search;
	}

return xfix;

}



xy_tuple rollback(int start_i,int start_j,int stop_i,int stop_j,int lengths_value,const char* a,const char* b,double error_percent)
{
	int i,j;
	const char *x,*y;
	xy_tuple fixing_end_tuple;
	printf("\n\t Start_i: %d Start_j: %d Stop_i: %d Stop_j: %d ",start_i,start_j,stop_i,stop_j);
 	int fixing_x_state=0;int fixing_y_state=0;	
	if( (start_i < (lena-1) ) && ( start_j < (lenb-1) ) )
	{
			printf("\n\t Start_i-1: %d Start_j+1: %d x: %c y:%c lengths[start_i-1][start_j+1]: %d ",(start_i-1),(start_j+1),*(a+start_i-2),*(b+start_j),lengths[start_i-1][start_j+1]);
			printf("\n\t Start_i+1: %d Start_j-1: %d x: %c y:%c lengths[start_i+1][start_j-1]: %d ",(start_i+1),(start_j-1),*(a+start_i),*(b+start_j-2),lengths[start_i+1][start_j-1]);
	
		// For rolling back, the following set of events should be checked.
		// If If (start_i,start_j) is where the fault was injected, 
		// 1. lengths[][] at the point (start_i-1,start_j+1) is equal or greater than lengths[][] at (start_i,start_j). If so, then no need to go forward.
		// 2. lengths[start_i][start_j]=lengths[start_i][start_j+k] k=0 to l (should find it out!) and should then determine, whether lengths[start_i][start_j+k+1] be influenced by lengths[start_i][start_j] and should find, lengths[start_i][start_j+k+m] , where 'm' ends 
		// 3. Same should be done for searching in start_i direction.
		x=(a+start_i-1);y=(b);
		int lengths_start_ij=lengths[start_i][start_j];
		int xfix,yfix;
		if( ! (lengths_start_ij==lengths[start_i-1][start_j+1] ) )
		{
			int column_search=start_j+1;
			int row_search=start_i;
			int char_comp=0;
			int length_comp=0;
		
			int nolonger_need2search=1;
			while( (nolonger_need2search) && ( column_search < lenb ) )
			{
				char_comp=( *(x)==*(b+column_search-1) );
				length_comp= (lengths[start_i][start_j]<=lengths[start_i-1][column_search]);
				if(  ( (!length_comp) && (!char_comp) ) )
				{
					printf("\n\t *--* I: %d J: %d lengths[i-1][j]: %d and x: %c y: %c",start_i-1,column_search,lengths[start_i-1][column_search],*x,*(b+column_search-1));
					column_search++;
				}
				else
				{
					printf("\n\t *++* I: %d J: %d lengths[i-1][j]: %d and x: %c y: %c",start_i-1,column_search,lengths[start_i-1][column_search],*x,*(b+column_search-1));
				
					nolonger_need2search=0;
				}
			}
			xfix=column_search;
	
		}
		else
		{
			xfix=start_j+1;
		}
		if( ! ( lengths_start_ij==lengths[start_i+1][start_j-1] ) )
		{
			int column_search=start_j-1;
			int row_search=start_i+1;
			int char_comp=0;
			int length_comp=0;
			x=a; y=b;
			int nolonger_need2search=1;
			while( (nolonger_need2search) && ( ( column_search < lenb ) && ( row_search < lena ) ) )
			{
				char_comp=( *(x+row_search-1)==*(y+column_search) );
				length_comp=(lengths[start_i][start_j]<=lengths[row_search][column_search]);
				if(  ( (!length_comp) && (!char_comp) ) )
				{
					printf("\n\t |--| I: %d J: %d lengths[i][j]: %d and x: %c y: %c",row_search,column_search,lengths[row_search][column_search],*(x+row_search-1),*(y+column_search));
					row_search++;
				}
				else
				{
					printf("\n\t |++| I: %d J: %d lengths[i][j]: %d and x: %c y: %c",row_search,column_search,lengths[row_search][column_search],*(x+row_search-1),*(y+column_search));
				
					nolonger_need2search=0;
				}
			}
			yfix=row_search;
		}
		else
		{
			yfix=start_i+1;
	 	}

		if(1)//xfix<lenb)
		{
			fixing_x_state=1;
			for(i=start_i;i<yfix-1;i++)	
			{
				x=(a+i-1);
				int lengths_ixfix=lengths[i][xfix-1]; // NOTE: lengths_ixfix has value of lengths[i][xfix-1]
				printf("\n\t Starting fixing dir-x in row: %d and yfix: %d ",i,yfix);
				int error_inject;
				for(j=start_j,y=(b+start_j-1);j<xfix;j++,y++)
				{
					error_inject=percent_error(error_percent);
					int before_lenghts=lengths[i][j];
					if ( XOR( (*x == *y), error_inject ) )
					{
						lengths[i][j] = lengths[i-1][j-1] +1;
						ops_count++;
					}
			
					else
					{
						int ml = MAX(lengths[i-1][j], lengths[i][j-1]);
						lengths[i][j] = ml;
					}			

					printf("\n\t fixing i:%d j: %d x: %c y: %c lengths[i][j]: %d before_lenghts: %d error_inject: %d ",i,j,*x,*y,lengths[i][j],before_lenghts,error_inject);

				}
	
		
		// NOTE: lengths[i][j]=lengths_ixfix		
		// Assumption is that lengths[i][j] was faulty (j=yfix),and it was corrected in the above for-loop, hence lengths[i][j] being equal to lengths[i-1][j+1] implies lengths[i][j]  was propogated in lengths[i][j+1]. 
				error_inject=percent_error(error_percent);
				int need2search,column_search;
				if( XOR( ( *x==*(b+xfix-1)),error_inject) )
				{
					lengths[i][xfix]=lengths[i-1][xfix-1]+1;
					need2search=1;
					column_search=xfix+1;
					printf("\n\t ^^fixing i:%d j: %d x: %c y: %c lengths[i][j]: %d error_inject: %d ",i,xfix,*x,*(b+xfix-1),lengths[i][xfix],error_inject);

				}
				else 
				{
					if( lengths_ixfix > lengths[i-1][xfix] ) //
					{
						lengths[i][xfix]=lengths[i-1][xfix];
						need2search=1;
						column_search=xfix+1;
						printf("\n\t ^^fixing^^ i:%d j: %d x: %c y: %c lengths[i-1][xfix]: %d lengths_ixfix: %d error_inject: %d ",i,xfix,*x,*(b+xfix-1),lengths[i-1][xfix],lengths_ixfix,error_inject);				
					}
					else
					{
						need2search=0;
						printf("\n\t ^^fixing** i:%d j: %d x: %c y: %c lengths[i-1][xfix]: %d lengths_ixfix: %d error_inject: %d ",i,xfix,*x,*(b+xfix-1),lengths[i-1][xfix],lengths_ixfix,error_inject);				
					}
		
				}
		
				int char_comp=0;
				int lengths_comp=0;
				while( (need2search) && (column_search < lenb) )
				{
					char_comp=( *x==*(b+column_search-1) );
					lengths_comp=(lengths[i][column_search-1]==lengths[i-1][column_search] );
					if ( (!lengths_comp) && (!char_comp) )
					{
						printf("\n\t --fixing-y- I: %d J: %d lengths[i][j]: %d and x: %c y: %c",i-1,column_search,lengths[i-1][column_search],*x,*(b+column_search-1));
						lengths[i][column_search]=lengths[i-1][column_search];
						column_search++;
					}
					else
					{
						printf("\n\t ++fixing+y+ I: %d J: %d lengths[i][j]: %d and x: %c y: %c",i-1,column_search,lengths[i-1][column_search],*x,*(b+column_search-1));
				
						need2search=0;
					}
					xfix=column_search;
				}
			}

			printf("\n\t Ending fixing dir-x in row: %d and yfix: %d and fixing_x_state: %d",i,yfix,fixing_x_state);	
		
		}
		else
		{
			fixing_x_state=0;
			printf("\n\t NOT fixing dir-x. xfix: %d and yfix: %d ,lenb: %d \n",xfix,yfix,lenb);
		}
		if(1)//yfix<lena)
		{

			fixing_y_state=1;
			int need2search=0;
			int row_search=yfix;
			int lengths_yfix=lengths[row_search-1][start_j];
			int yfix_x; // x-value corresponding to yfix
			yfix_x=start_j+1;
		
			if( *(a+row_search-2)==*(b+start_j-1) )
				lengths[row_search-1][start_j]=lengths[row_search-1][start_j-1]+1;
			else
				lengths[row_search-1][start_j]=MAX(lengths[row_search-2][start_j],lengths[row_search-1][start_j-1]);

			int fixing_notcomplete=1;			
	 		while( (fixing_notcomplete) && (row_search < (lena-1)) )
			{
				while( (!need2search) && (yfix_x < xfix ) )
				{
					printf("\n\t Starting fixing dir-y. Probing in row: %d ( might need to fix row: %d ) xfix: %d and yfix_x: %d ",row_search,row_search-1,xfix,yfix_x);
					x=a+row_search-1;// Char corresponding to 'yfix'
					y=b+yfix_x-1;
					if(*x==*y ) 
					{
						need2search=1; // For 'row_search' but until now 'row_search-1' row has not been dealt yet.
						lengths[row_search][yfix_x]=lengths[row_search-1][yfix_x-1]+1;
						printf("\n\t &&%%fixing i: %d j:%d x: %c y: %c lengths[row_search][yfix_x]: %d ",row_search,yfix_x,*x,*y,lengths[row_search][yfix_x]);
					}
					else 
					{
						lengths_yfix=lengths[row_search-1][yfix_x];
						if( *(a+row_search-2)==*(b+start_j) )
						{
							lengths[row_search-1][yfix_x]=lengths[row_search-2][yfix_x-1]+1;
						}
						else
						{
							lengths[row_search-1][yfix_x]=MAX( lengths[row_search-2][yfix_x],lengths[row_search-1][yfix_x-1] );
						}
		
						if( lengths_yfix > lengths[row_search][yfix_x-1]  )
						{
							need2search=1;
							lengths[row_search][yfix_x]=MAX(lengths[row_search-1][yfix_x],lengths[row_search][yfix_x-1]);
							printf("\n\t &&%%--fixing-- i: %d j: %d x: %c y: %c lengths[row_search][yfix_x-1]: %d ",row_search,yfix_x,*x,*y,lengths[row_search][yfix_x-1]);
						}
						else
						{
							need2search=0;
							printf("\n\t &&%%++fixing++ i: %d j: %d x: %c y: %c lengths[row_search][yfix_x-1]: %d ",row_search,yfix_x,*x,*y,lengths[row_search][yfix_x-1]);
							yfix_x++;
						}
			
					}
				}
				if(need2search && (xfix < lenb ) ) // Should be careful about xfix<lenb, might cause problems to those cases in the bottom of the matrix.
				{
					printf("\n\t -- Going to searching in row: %d and xfix is %d and yfix_x: %d \n",row_search-1,xfix,yfix_x);
					 xfix=search_in_row(a,b,xfix,error_percent,row_search-1,yfix_x);
					printf("\n\t -- Returned from searching in row: %d and xfix is %d and yfix_x: %d \n",row_search-1,xfix,yfix_x);
					row_search++;				
					need2search=0;				
					//exit(-1);
				}	 
				else
					fixing_notcomplete=0;
			}	
			if(fixing_notcomplete)
			{
				fixing_y_state=0;
			}
			else
				fixing_y_state=1;
			printf("\n\t End of fixing dir-y. xfix: %d and yfix: %d ,lenb: %d and fixing_y_state: %d \n",xfix,yfix,lenb,fixing_y_state);
			if(xfix>=lenb) xfix=lenb-1;
			if(yfix>=lena) xfix=lena-1;			
			fixing_end_tuple.j=xfix;
			fixing_end_tuple.i=yfix;	
		
		}
		else
		{
			fixing_y_state=0;
			printf("\n\t NOT fixing dir-y. xfix: %d and yfix: %d ,lenb: %d \n",xfix,yfix,lenb);
		}
	}
	if( (!fixing_x_state) && (!fixing_y_state) )
	{
		for(i=start_i,x=(a+start_i-1);i<(lena);i++,x++)
		{
			for(j=start_j,y=(b+start_j-1);j<(lenb);j++,y++)
			{
				printf("\n\t ** I: %d J: %d lengths[i][j]: %d and x: %c y: %c",i,j,lengths[i][j],*x,*y);
			
				int error_inject=percent_error(error_percent);
					 
				if(error_inject)
				{
					error_inject_count++;
				}
		 
				ops_count++; 
				if ( XOR( (*x == *y), error_inject ) )
				{
					lengths[i][j] = lengths[i-1][j-1] +1;
					ops_count++;
				}
			
				else
				{
					int ml = MAX(lengths[i-1][j], lengths[i][j-1]);
					lengths[i][j] = ml;
				}
			
				printf("\n\t -- I: %d J: %d lengths[i][j]: %d \n",i,j,lengths[i][j]); 
			}
		} 
		printf("\n\t #$#$#$ Fixing x: %d y: %d \n",fixing_end_tuple.j,fixing_end_tuple.i);
		fixing_end_tuple.i=0;		fixing_end_tuple.j=0;
		return fixing_end_tuple;
		
	}// printf("\n\n");exit(-1);
	
	printf("\n\t &&&&& Fixing x: %d y: %d \n",fixing_end_tuple.j,fixing_end_tuple.i);
	return fixing_end_tuple;
}


void stuck_need2exit(int i,int j,const char *a,const char *b)
{

	int iprime=i-1;
	int jprime=j-1;
	printf("\n\n\t ------------------- Mostly stuck in a loop at i: %d j: %d !! ----------------\n\n",i,j);
	printf(" \n\t a[i-2] : %c a[i-1]: %c a[i]: %c a[i+1]: %c ",a[iprime-2],a[iprime-1],a[iprime],a[iprime+1]);
	printf(" \n\t b[j-2] : %c a[j-1]: %c b[j]: %c a[j+1]: %c \n\n",b[jprime-2],b[jprime-1],b[jprime],b[jprime+1]);
	printf("\n\t Lengths[i-2][j-2]: %d , Lengths[i-2][j-1]: %d , Lengths[i-2][j]: %d, Lengths[i-2][j+1]: %d \n",lengths[i-2][j-2],lengths[i-2][j-1],lengths[i-2][j],lengths[i-2][j+1]);
	printf("\n\t Lengths[i-1][j-2]: %d , Lengths[i-1][j-1]: %d , Lengths[i-1][j]: %d, Lengths[i-1][j+1]: %d \n",lengths[i-1][j-2],lengths[i-1][j-1],lengths[i-1][j],lengths[i-1][j+1]);
//	printf("\n\t Lengths[i][j-2]: %d , Lengths[i][j-1]: %d , Lengths[i][j]: %d, Lengths[i][j+1]: %d \n",lengths[i][j-2],lengths[i][j-1],lengths[i][j],lengths[i][j+1]);
//	printf("\n\t Lengths[i+1][j-2]: %d , Lengths[i+1][j-1]: %d , Lengths[i+1][j]: %d, Lengths[i+1][j+1]: %d \n",lengths[i+1][j-2],lengths[i+1][j-1],lengths[i+1][j],lengths[i+1][j+1]);

}

void save_mat_b4exit()
{
	int i,j;
	for(i=0;i<lena;i++)
	{
		fprintf(save_mat,"\n\n\t Row: %d \n",i);
		for(j=0;j<lenb;j++)
		{
			fprintf(save_mat,"\t Col: %d %d ",j,lengths[i][j]);
		}
	}

}
 
char* lcs(const char *str_1,const char *str_2,double error_percent) 
{
	const char* a=str_1;
	const char* b=str_2;
    // int lena = strlen(a)+1;     int lenb = strlen(b)+1;
		lena=strlen(a)+1; lenb=strlen(b)+1;
 //printf("\n\t Length of string-1: %d string-1: %s \n\t Length of string-2: %d string-2: %s \n",lena,a,lenb,b);
    int bufrlen = lena;
    char bufr[bufrlen];

    int i,j;
    const char *x, *y;
    int *la = calloc(lena*lenb, sizeof( int));
    //int  **lengths = malloc( lena*sizeof( int*));
	lengths = malloc( lena*sizeof( int*));
    for (i=0; i<lena; i++) lengths[i] = la + i*lenb;

    int ops_count=0;
 
    for (i=0,x=a; *x; i++, x++)
    {
         struct timeval t1;
         gettimeofday(&t1, NULL);
         srand(t1.tv_usec * t1.tv_sec);
         int error_inject;
        for (j=0,y=b; *y; j++,y++ )
        {
			error_inject=percent_error(error_percent);
				 
			if(error_inject)
			{
				error_inject_count++;
			}
	 
			ops_count++; 
			if ( XOR( (*x == *y), error_inject ) )
			{
				lengths[i+1][j+1] = lengths[i][j] +1;
				ops_count++;
			}
			
			else
			{
				int ml = MAX(lengths[i+1][j], lengths[i][j+1]);
				lengths[i+1][j+1] = ml;
			}
       }
    }
	
	printf("\n\t Finished filling the matrix, error-inject-count: %d ops-count: %d \n",error_inject_count,ops_count);
   // result = bufr+bufrlen;
   char result[lena];
    int result_pos=0;//lena-1;
   // *--result = '\0';
   //result[result_pos]='\0';
	int result_length=0;
	result_pos--; //This is the index if results were to be accessed as an array.
	i = lena-1; j = lenb-1;
    int error_inject_1,error_inject_2;
	
    int ij_considered=0;
	int count_reset_seed=0;

	int accepted_i=lena-1,accepted_j=lenb-1;
	int last_considered_i=0,last_considered_j=0;
    while ( (i>0) && (j>0) ) 
 	{
		ij_considered++;
		/*if(count_reset_seed%lena==0)
		{
		     struct timeval t1;
		     gettimeofday(&t1, NULL);
		     srand(t1.tv_usec * t1.tv_sec);
		}*/
         
/*		error_inject_1=percent_error(error_percent); error_inject_2=percent_error(error_percent); if(error_inject_1) { error_inject_count++; }if(error_inject_2){ error_inject_count++;}*/
		ops_count++;
 
		int c1_char_comp1= ( *(a+i-1) == *(b+j-1) );//;(a[i-1]==b[j-1]); 
		int c1_char_comp2= (a[i-1]==b[j-1]); int c1_char_comp3=(a[i-1]==b[j-1]);
 		int c1_op1=(lengths[i][j] == lengths[i-1][j] ); 
 		int c1_op2; // This is also used in Comp1.
 		int c1_op3=( lengths[i][j-1] <= lengths[i-1][j] );
 		int c2_op1= ( lengths[i][j]==lengths[i][j-1] ); // There is no need for this to be always calculated if C1 is successful.
 		int c2_op3=( lengths[i][j-1] >= lengths[i-1][j] );
 		
		// The following if-else branch implements TMR, there should not be an instance where c1_op2 
		if( ( c1_char_comp1==c1_char_comp2 ) ) 
		{
			c1_op2=c1_char_comp1;
		}
		else if( ( c1_char_comp1==c1_char_comp3 ) )
		{
			c1_op2=c1_char_comp1;		
		}
		else if( ( c1_char_comp2==c1_char_comp3 ) )
		{
			c1_op2=c1_char_comp2;
		}
		
		// ==> For valid/rightful calculation 
		
		// ------------------- Initialization ---------------------
		// Let c1_op1= lengths[i][j]==lengths[i-1][j] c2= TMR based comparison for a[iprime] and b[jprime]
		// c1_op2 is always right (cos of TMR)
		// c1_op3= ( lengths[i][j-1] > lengths[i-1][j] )
		// c1_op4= lengths[i][j]==lengths[i][j-1]
		
		// 1. if c1_op2=1  ==> lengths[i][j]=lengths[i-1][j-1]+1.
		//				==>  lengths[i][j]= a.( lengths[i][j-1] || lengths[i][j-1] + 1 ) 
		//				             \\      b. ( lengths[i-1][j] || (lengths[i-1][j]+1) )
		
		// 2. So if, lengths[i-1][j]!=lengths[i][j]
				// ==> a. c1_op2==1.
				// ==> b. lengths[i][j-1] > lengths[i-1][j] ( ~ diff=1)
		// 3. lengths[i][j-1]!=lengths[i][j]
				// ==> a. c1_op2=1 ( or c2_op2) 
				// ==> b. lengths[i][j-1] < lengths[i-1][j] ( ~ diff=1)
				
		//  -----------------------------------------------------				
		// Following measures can be used to account for each case shown above.
		
		// 1. No need to bother mostly.
		// 2. a should be checked! , if a is true, thats enough. if a is not true, then b can cause this condition! 
		// 3. Same as 2!
		
		//----------- Boolean realization ---------------------
		// 1. No need to bother.
		// 2. If ( lengths[i-1][j]== lengths[i][j]) then if( (c1_op2==0)  && ( c1_op3==1 ) ) ==> Correct operation / else rollback until latest update. 
			// c1_op3 can be affected, since lengths[i][j] or lengths[i][j-1] can be affected by a wrong computation in the process of computing one of the dependent result.
		// 3. if (  lengths[i][j]== lengths[i][j-1] ) then (  ( c1_op2==0) && (  c2_op3==1 ) )
		// Would be checked only if 2 is false (  ie., c1_op1=0, c1_op2=0 || c1_op3=0 )
			//	a. If (c1_op4)=0, check if c1_op3
			
 
		if( c1_op1 && (!c1_op2) && (c1_op3)  ) 
		{

			printf("\n\t C1 i: %d j: %d a[i]: %c a[i-1]: %c b[j]: %c and ij_considered: %d lengths[i][j]:%d lengths[i-1][j]:%d lengths[i][j-1]: %d ",i,j,a[i-1],a[i-2],b[j-1],ij_considered,lengths[i][j],lengths[i-1][j],lengths[i][j-1]);			
			last_considered_i=i; last_considered_j=j;	
			i -= 1;
			ij_considered=0;
			
		}
		else if( c2_op1 && (!c1_op2) && (c2_op3) )
		{
		
			last_considered_i=i; last_considered_j=j;		
			printf("\n\t C2 i: %d j: %d a[i]: %c b[j-1]: %c b[j]: %c and ij_considered: %d lengths[i][j]:%d lengths[i-1][j]: %d lengths[i][j-1]: %d",i,j,a[i-1],b[j-2],b[j-1],ij_considered,lengths[i][j],lengths[i-1][j],lengths[i][j-1]);			
		
			j-= 1;
			ij_considered=0;
		}
		else if( (c1_op2) ) //&& (!c1_op1) && (!c2_op1) )
		{
			//*--result = a[i-1];

			result[result_pos]=a[i-1];
			result_pos++;			
			//*(result+result_pos)=a[i-1];
			result_length++;
			last_considered_i=i; last_considered_j=j;	
			accepted_i=i;accepted_j=j;

			top_of_stack=(top_of_stack+1)%(size_stack);
			end_of_stack=((top_of_stack+1)%size_stack);			
			accepted_tuple_chars[top_of_stack].my_tuple.i=i;
			accepted_tuple_chars[top_of_stack].my_tuple.j=j;			
			//accepted_tuple_chars[top_of_stack].my_char=a[i-1];
			//printf("\n\t C3 i: %d j: %d a[i-1]: %c b[j-1]: %c and ij_considered: %d lengths[i][j]: %d result_length: %d lengths[i][j-1]: %d top_of_stack: %d results: %s",i,j,a[i-1],b[j-1],ij_considered,lengths[i][j],result_length,lengths[i][j-1],top_of_stack,result);						
			i-=1; j-=1;
			ij_considered=0;
		}
		
		if(ij_considered>2)
		{
			stuck_need2exit(i,j,a,b);
			//printf("\n\n\t Exception! i: %d j: %d a[i-1]: %c b[j-1]: %c lengths[i-1][j]: %d lengths[i][j-1]:%d lengths[i][j]:%d  and ij_considered: %d and results: %s ",i,j,a[i-1],b[j-1],lengths[i-1][j],lengths[i][j-1],lengths[i][j],ij_considered,result);					
			//rollback(last_considered_i,last_considered_j,accepted_i,accepted_j,lengths[i][j],a,b,error_percent);
			xy_tuple fixing_end_tuple=rollback(i,j,accepted_i,accepted_j,lengths[i][j],a,b,error_percent);
			//printf("\n\n\t -- Last considered i: %d j: %d \n\t Need to rollback the area until accepted-i: %d j: %d ",last_considered_i,last_considered_j,accepted_i,accepted_j);
			
			if( !( (fixing_end_tuple.i==0) && (fixing_end_tuple.j==0) && ( top_of_stack <0) ) )
			{
				rollback_tuple_pos rolling_back=search_stack(fixing_end_tuple,result_pos);
			
				last_considered_i=i;last_considered_j=j;	
 
				printf("\n\t &&&&& Should find accepted point in trace from i: %d & j: %d ",fixing_end_tuple.i,fixing_end_tuple.j);
			
				if( (rolling_back.my_tuple.i) && (rolling_back.my_tuple.j) )
				{	i=rolling_back.my_tuple.i-1;
					j=rolling_back.my_tuple.j-1;
					// result=result+rolling_back.result_position_adjust-2;
					result_pos-=rolling_back.result_position_adjust;
					printf("\n\t Rollback suggestion from stack---> i: %d j: %d and result_pos: %d ",i,j,result_pos);
				}
				else
				{
					i=lena-1; //rolling_back.my_tuple.y-1;
					j=lenb-1;//rolling_back.my_tuple.x-1;
					// result=bufr+bufrlen-1;
					//result_pos=bufrlen-1;
					result_pos=0;
					printf("\n\t Rollback suggestion from stack has been changed to ---> i: %d j: %d result_pos: %d ",i,j,result_pos);
				}	
				
				ij_considered=0;
			}
			else
			{
				i=lena-1; //rolling_back.my_tuple.y-1;
				j=lenb-1;//rolling_back.my_tuple.x-1;
				// result=bufr+bufrlen-1;
				//result_pos=bufrlen-1;
				result_pos=0;
			}
			printf("\n\t After rollover top-of-stack: %d end-of-stack: %d result_pos: %d",top_of_stack,end_of_stack,result_pos);
		}
        count_reset_seed++;
    }

	//save_mat_b4exit();
    

	printf("\n\t Finished tracing the matrix, error-inject-count: %d ops-count: %d \n",error_inject_count,ops_count);
    	printf("\n\t Error-percent is %f\n\t",error_percent);	
	result_length=strlen(result);
	printf("\n\t Result: %s \n\n Result-length: %d \n\n",result, result_length);

	for(i=0;i<result_length;i++)
		printf("\n\t i: %d result[i]: %c ",i,result[i]);
	
	//free(result);
	free(la);
	free(lengths);
	//return strdup(result);
}

int main(int argc, char *argv[])
{
        int size1,size2;
        size_t r1,r2;
        FILE *fp1,*fp2;

		size1=0;size2=0;//r1=0;r2=0;
 
 // ******* Global variables being initialized **********
 
 error_inject_count=0;
 ops_count=0;
 accepted_tuple_chars=( tuple_chars* ) calloc( size_stack,sizeof(tuple_chars) );
 top_of_stack=-1;
 end_of_stack=0;
 int init_stack=0;
 for( init_stack=0;init_stack<size_stack;init_stack++ )
 	accepted_tuple_chars[init_stack].my_char='-';
 //top_of_stack=accepted_tuple_chars;
 // *************
 
 
 
        if(argc<5)
        {
                printf("\n\t ERROR:  Expected inputs \n\t\t 1.Two files. \n\t\t 2. String-length. \n\t\t 3. Error-percent. \n\t\t 4. Output file name (optional) \n\n\t ------ Kindly provide appropriate inputs -------- \n\n");
                exit(-1);
        }
		

        double error_percent=atof(argv[4]);
		int string_length=atoi(argv[3]);

	if(argc==6)
	{
		char* filename=argv[6];
		save_mat=fopen(argv[6],"w");
	}

        if(error_percent<0)
        {
                printf("\n\t Error: Error-percent should be greater than or equal to 0\n");
                exit(-1);
        }

        fp1=fopen(argv[1],"r");
        if(fp1!=NULL)
        {
                // printf("\n File %s located successfully! \n",argv[1]);
                fseek(fp1,0,2);
                size1=ftell(fp1)/sizeof(char) ;
                fseek(fp1,0,0);
                // printf("\n Size of file %s is %d bytes\n",argv[1],size1);

        }
        else
        {
                printf("\n\t %s's file read failed! \n",argv[1]);
                exit(-1);
        }

        fp2=fopen(argv[2],"r");
        if(fp2!=NULL)
        {
                // printf("\n File %s located successfully! \n",argv[2]);
                fseek(fp2,0,2);
                size2=ftell(fp2)/sizeof(char);
                fseek(fp2,0,0);
                // printf("\n Size of file %s is %d bytes\n",argv[2],size2);

        }

        else
        {
                printf("\n\t %s's file read failed! \n",argv[2]);
                exit(-1);
        }

        //char str_a[size1],str_b[size2];
        char *str_a,*str_b;
         str_a=(char *)malloc(size1*sizeof(char));
         str_b=(char *)malloc(size2*sizeof(char));

        r1=fread(str_a,sizeof(char),size1,fp1);
        if(r1!=size1)
        {
                printf("\n\t File %s was read until %d bytes \n", argv[1], (int)r1);
                exit(-1);
        }
        // else printf("\n File %s completely read \n",argv[1]);

        r2=fread(str_b,sizeof(char),size2,fp2);
        if(r2!=size2)
        {
                printf("\n\t File %s was read until %d bytes \n", argv[2], (int)r2);
                exit(-1);
        }
        // else printf("\n File %s completely read \n",argv[2]);


         printf("\n\t Reading files done. Size of file1: %d and size of file2: %d \n",size1,size2);
        /********************************************************** Reading two files-ends ***************************************/
        {
                      
			char *result;
			//result=(char *)malloc(string_length*sizeof(char)); 
			// Since maximum size that could be expected is definitely less than or equal to string_length of strings being compared.
			char *clip_str_a,*clip_str_b;
			
			clip_str_a=(char *)malloc(string_length*sizeof(char)); 
			clip_str_b=(char *)malloc(string_length*sizeof(char)); 

			strncpy(clip_str_a,str_a,string_length);
			strncpy(clip_str_b,str_b,string_length);
			
			//printf("\n\t String-length: %d \n",string_length);
			printf("\n\t String-length: %d Characters: \n\t String1: %s \n\t String2: %s\n",string_length,clip_str_a,clip_str_b);

			result=lcs(clip_str_a,clip_str_b,error_percent);
		
           // printf("\n\t LCS-result: %s \n\t Length of result: %d \n",result,(int)strlen(result)); // tsitest
		//	printf("\n\t Len-a: %d Len-b: %d \n\n",(unsigned)strlen(clip_str_a),(unsigned)strlen(clip_str_b));
            return 0;
        }
	if( argc==6 )	fclose(save_mat);
}
