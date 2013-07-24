#include "error_inject_operator.h"
#include<cstdio>
#include <string.h>

template <class T>
double error_inject_operators<T>::error_percent=0.0;


#define MAX(A,B) (((A)>(B))? (A) : (B))
#define percent_error(x) ( ( ( (double) rand()/RAND_MAX) < x ) ? 1 : 0)
#define XOR(x,y) ( (x&&(!y)) || ( (!x) && y ) )

#define size_stack 40
#define size_stack_minus1 (size_stack-1)



int  **lengths;
int lena,lenb;
int lena_minus1,lenb_minus1;
FILE* save_mat;
FILE* save_rollback_ops;
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

//char* lcs(error_inject_operators<char>* str_a,error_inject_operators<char>* str_b)
char* lcs(const char* clip_str_a,const char* clip_str_b,double error_percent) 
{
	error_inject_operators<char>* a;
	error_inject_operators<char>* b;	
	a=new error_inject_operators<char>[lena];
	b=new error_inject_operators<char>[lenb];
			
	for(int i=0;i<lena;i++)
	{
		a[i]=clip_str_a[i];
	}
	for(int i=0; i< lenb; i++)
	{
		b[i]=clip_str_b[i];				
	}
	
    // int lena = strlen(a)+1;     int lenb = strlen(b)+1;
	lena++; lenb++; //=strlen(a)+1; lenb=strlen(b)+1;
 //printf("\n\t Length of string-1: %d string-1: %s \n\t Length of string-2: %d string-2: %s \n",lena,a,lenb,b);
    int bufrlen = lena;
    char bufr[bufrlen];

      // int *la = calloc(lena*lenb, sizeof( int));
    	// int  **lengths = malloc( lena*sizeof( int*));
	// lengths = malloc( lena*sizeof( int*));
	// for (i=0; i<lena; i++) lengths[i] = la + i*lenb;
	
	error_inject_operators<int>** lengths;
	lengths= new error_inject_operators<int>*[lena];
	
	for(int i=0;i<lena;i++)
	{
		lengths[i]=new error_inject_operators<int>[lenb];
		for(int j=0;j<lenb;j++)
			lengths[i][j]=0;
	}

    int ops_count=0;
	error_inject_operators<char> x,y; 
    for (int i=0; i<(lena_minus1); i++)
    {
	x=a[i];
        for (int j=0;j<lenb_minus1; j++ )
        {
	 	y=b[j];
		if ( x==y )
		{
			lengths[i+1][j+1] = lengths[i][j] +1;
		}
		else if(lengths[i+1][j] > lengths[i][j+1]) 
		{
			lengths[i+1][j+1] = lengths[i+1][j];
		}
		else
		{
			lengths[i+1][j+1]=lengths[i][j+1];
		}
       }
    }
	
	printf("\n\t Finished filling the matrix, error-inject-count: %d ops-count: %d \n",error_inject_count,ops_count);

/*    char result[lena];
    int result_pos=0;
    int result_length=0;
    result_pos--; //This is the index if results were to be accessed as an array.
	int i = lena-1;int j = lenb-1;
    int error_inject_1,error_inject_2;
	
    int ij_considered=0;
	int count_reset_seed=0;

	int accepted_i=lena-1,accepted_j=lenb-1;
	int last_considered_i=0,last_considered_j=0;
/*    while ( (i>0) && (j>0) ) 
 	{
		ij_considered++;
		//if(count_reset_seed%lena==0)
		//{
		   //  struct timeval t1;
		   //  gettimeofday(&t1, NULL);
		   //  srand(t1.tv_usec * t1.tv_sec);
		//}
         

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
			printf("\n\t C3 i: %d j: %d a[i-1]: %c b[j-1]: %c and ij_considered: %d lengths[i][j]: %d result_length: %d lengths[i][j-1]: %d top_of_stack: %d",i,j,a[i-1],b[j-1],ij_considered,lengths[i][j],result_length,lengths[i][j-1],top_of_stack);						
			fprintf(save_rollback_ops,"\n\t C3 i: %d j: %d a[i-1]: %c b[j-1]: %c and ij_considered: %d lengths[i][j]: %d result_length: %d lengths[i][j-1]: %d top_of_stack: %d",i,j,a[i-1],b[j-1],ij_considered,lengths[i][j],result_length,lengths[i][j-1],top_of_stack);						
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
			fprintf(save_rollback_ops,"\n\t After rollover top-of-stack: %d end-of-stack: %d result_pos: %d \n\t i: %d j: %d ",top_of_stack,end_of_stack,result_pos,i,j);			
		}
        count_reset_seed++;
    }

	//save_mat_b4exit();
    

	printf("\n\t Finished tracing the matrix, error-inject-count: %d ops-count: %d \n",error_inject_count,ops_count);
    	printf("\n\t Error-percent is %f\n\t",error_percent);	
	result_length=strlen(result);
	printf("\n\t Result: %s \n\n Result-length: %d \n\n",result, result_length);

//	for(i=0;i<result_length;i++)
//		printf("\n\t i: %d result[i]: %c ",i,result[i]);
	
	//free(result);
	free(la);
	free(lengths);
	//return strdup(result);
	*/
}






int main(int argc,char* argv[])
{

        int size1,size2;
        size_t r1,r2;
        FILE *fp1,*fp2;
       string save_rollback_ops_fname="save_rollback_ops.txt";
	save_rollback_ops=fopen(save_rollback_ops_fname.c_str(),"w");
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

			error_inject_operators<char>::error_percent=0.00000;
			error_inject_operators<int>::error_percent=0.00;			
			error_inject_operators<char>* error_inject_str_a;
			error_inject_operators<char>* error_inject_str_b;	
			lena=string_length;lenb=string_length;	
			lena_minus1=lena-1; lenb_minus1=lenb-1;
			error_inject_str_a=new error_inject_operators<char>[lena];
			error_inject_str_b=new error_inject_operators<char>[lenb];			
			
			for(int i=0;i<string_length;i++)
			{
				error_inject_str_a[i]=clip_str_a[i];
				error_inject_str_b[i]=clip_str_b[i];				
			}


			
			//printf("\n\t String-length: %d \n",string_length);
			printf("\n\t String-length: %d Characters: \n\t String1: %s \n\t String2: %s\n",string_length,clip_str_a,clip_str_b);

			//result=lcs(clip_str_a,clip_str_b,error_percent);
		
           // printf("\n\t LCS-result: %s \n\t Length of result: %d \n",result,(int)strlen(result)); // tsitest
		//	printf("\n\t Len-a: %d Len-b: %d \n\n",(unsigned)strlen(clip_str_a),(unsigned)strlen(clip_str_b));
		if( argc==6 )	fclose(save_mat);
		fclose(save_rollback_ops);
           
        }

 return 0;


return 0;
}
