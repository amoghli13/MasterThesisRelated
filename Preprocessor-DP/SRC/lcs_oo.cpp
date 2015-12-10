#include "error_inject_operator.h"
#include<cstdio>
#include <string.h>
#include <set>
template <class T>
double error_inject_operators<T>::error_percent=0.0;


#define MAX(A,B) (((A)>(B))? (A) : (B))
#define percent_error(x) ( ( ( (double) rand()/RAND_MAX) < x ) ? 1 : 0)
#define XOR(x,y) ( (x&&(!y)) || ( (!x) && y ) )

#define size_stack 40
#define size_stack_minus1 (size_stack-1)



error_inject_operators<int>** lengths;
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
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////// xy_tuple_rollback  ////////////////////////////////////////////////////////
int rollback( int start_i,int start_j,error_inject_operators<char>*a,error_inject_operators<char>* b )
{
	start_i-=1;start_j-=1;
	vector< pair<int,int> > check_list;
	set< pair<int,int> > checked_list;
	check_list.push_back(make_pair(start_i,start_j));
	checked_list.insert(make_pair(start_i,start_j) );

	//curr_pos=check_list.begin();
	int current_position_check_list=0;
	int checklist_notempty=1;

	int num_condns=3;
	int test_condn;
	cout<<"\n\t Here at rollback i: "<<start_i<<" j: "<<start_j<<endl;

	while( checklist_notempty )
	{
		int check_i=check_list[current_position_check_list].first;
		int check_j=check_list[current_position_check_list].second;
		cout<<"\n\t Read check_i "<<check_i<<" check_j "<<check_j;
		vector< pair<int,int> >curr_indices;
		
		for( int curr_condn=0;curr_condn<num_condns;curr_condn++ )
		{
			int used_condn=0;
			test_condn=1 ;// NOTICE Fill it suitably! 
			int i,j;
			switch(curr_condn)
			{
				case 0: i=check_i+1;
					  j=check_j+1;
					  test_condn=1;
					 break;
				case 1: i=check_i;
					  j=check_j+1;
					  test_condn=2;
					 break;					 
				case 2: i=check_i+1;
					  j=check_j;
					  test_condn=3;
					 break;					 
			}
			if( i < lena_minus1 && j< lenb_minus1 )
			{
				if (a[i] == b[j])
				{
				   lengths[i+1][j+1] = lengths[i][j] + 1 ;
				   used_condn=1;
				}
				else if (  lengths[i+1][j] > lengths[i][j+1] ) 
				{
				   lengths[i+1][j+1] = lengths[i+1][j] ; 
				   used_condn=2;	   
			   	}    
				else
				{
				   lengths[i+1][j+1] = lengths[i][j+1]; 
				   used_condn=3;	   
				}

				if(used_condn==test_condn)
				{
					pair<int,int> insert_pair;
					insert_pair=make_pair(i,j);
					set< pair<int,int> >::iterator check_this_pair;
					check_this_pair=checked_list.find(insert_pair);
					if( check_this_pair==checked_list.end() )
					{
						check_list.push_back(insert_pair);
						checked_list.insert(insert_pair);
						cout<<"\n\t Pushing in i: "<<i<<" j: "<<j;					
					}
					else
						cout<<"\n\t Already in checklist_vector. i: "<<i<<" j "<<j;
				
					//check_list.insert(make_pair(i,j));				

				}
				cout<<"\n\t I: "<<i<<" J: "<<j<<" a[i]: "<<a[i]<<" b[j] "<<b[j]<<" lengths[i+1][j+1] "<<lengths[i+1][j+1]<<" test_condn "<<test_condn<<" used_condn "<<used_condn;
			}	
		}
		int length=check_list.size();
		cout<<"\n\t Length of vector of pairs : "<<length;
		//cout<<"\n\t check_list[length-1].first "<<check_list[length-1].first<<"\t check_list[length-1].second "<< check_list[length-1].second;
 
		if( current_position_check_list < (length-1) )
		//if(curr_pos!=check_list.end())
		{
			cout<<"\n\t curr_pos is not the end!! \n";
			current_position_check_list++;
			cout<<"\n\t Next-pair: "<<( check_list[current_position_check_list].first)<<", "<<( check_list[current_position_check_list].second);;
 			
		}
		else
		{
			checklist_notempty=0;
		}
 		//cout<<endl;exit(-1);
	}

	cout<<"\n\t Since I am almost done, let me get out of here!! \n";
	exit(-1);
	return 0;



}


////////////////////////////////////////////  stuck_need2exit  ///////////////////////////////////////////////////
void stuck_need2exit(int i,int j,error_inject_operators<char>*a,error_inject_operators<char>* b)
{

	int iprime=i-1;
	int jprime=j-1;
	printf("\n\n\t ------------------- Mostly stuck in a loop at i: %d j: %d !! ----------------\n\n",i,j);
 	cout<<"\n\t a[i-2] "<<a[iprime-2]<<" a[i-1] "<<a[iprime-1]<<" a[i]: "<<a[iprime]<<" a[i]: "<<a[iprime+1];
 	cout<<"\n\t b[i-2] "<<b[iprime-2]<<" b[i-1] "<<b[iprime-1]<<" b[i]: "<<b[iprime]<<" b[i]: "<<b[iprime+1]; 
	cout<<"\n\t lengths[i-2][j-2] "<<lengths[i-2][j-2]<<" lengths[i-2][j-1] "<<lengths[i-2][j-1]<<" lengths[i-2][j] "<<lengths[i-2][j]<<" lengths[i-2][j+1] "<<lengths[i-2][j+1];
	cout<<"\n\t lengths[i-1][j-2] "<<lengths[i-1][j-2]<<" lengths[i-1][j-1] "<<lengths[i-1][j-1]<<" lengths[i-1][j] "<<lengths[i-1][j]<<" lengths[i-1][j+1] "<<lengths[i-1][j+1];	
 
}

////////////////////////////////////////// save_mat_b4_exit /////////////////////////////////////////////////////

void save_mat_b4exit()
{
	int i,j;
	for(i=0;i<lena;i++)
	{
		fprintf(save_mat,"\n\n\t Row: %d \n",i);
		for(j=0;j<lenb;j++)
		{
			fprintf(save_mat,"\t Col: %d %d ",j,lengths[i][j].operand);
			
		}
	}

}

////////////////////////////////////////// Longest Common Subsequence ///////////////////////////////
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
	
	lena++; lenb++; 
        int bufrlen = lena;
        char bufr[bufrlen];
 
	//error_inject_operators<int>** lengths;
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
		cout<<"\n\t I: "<<i<<" J: "<<j<<" a[i]: "<<a[i]<<" b[j] "<<b[j]<<" lengths[i+1][j+1] "<<lengths[i+1][j+1];		
       }
       
    }
	
	printf("\n\t Finished filling the matrix, error-inject-count: %d ops-count: %d \n",error_inject_count,ops_count);

    char result[lena];
    int result_pos=0;
    int result_length=0;
    result_pos--; //This is the index if results were to be accessed as an array.
	int i = lena-2;int j = lenb-2;
    int error_inject_1,error_inject_2;
	
    int ij_considered=0;
	int count_reset_seed=0;

	int accepted_i=lena-1,accepted_j=lenb-1;
	int last_considered_i=0,last_considered_j=0;
    while ( (i>0) && (j>0) ) 
 	{
		ij_considered++;
 
 		int c1_char_comp1= ( a[i-1] == b[j-1] ); 
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
		
		if( ( i < (lena-10) ) && ( j < (lena-10)) ) 
		{
			cout<<"\n\t I: "<<i<<" J: "<<j<<" lena "<<lena;
			 int duh_testing=rollback(i,j,a,b); 
		}
		if( c1_op1 && (!c1_op2) && (c1_op3)  ) 
		{

			//printf("\n\t C1 i: %d j: %d a[i]: %c a[i-1]: %c b[j]: %c and ij_considered: %d lengths[i][j]:%d lengths[i-1][j]:%d lengths[i][j-1]: %d ",i,j,a[i-1],a[i-2],b[j-1],ij_considered,lengths[i][j],lengths[i-1][j],lengths[i][j-1]);			
			last_considered_i=i; last_considered_j=j;	
			i -= 1;
			ij_considered=0;
			
		}
		else if( c2_op1 && (!c1_op2) && (c2_op3) )
		{
		
			last_considered_i=i; last_considered_j=j;		
			//printf("\n\t C2 i: %d j: %d a[i]: %c b[j-1]: %c b[j]: %c and ij_considered: %d lengths[i][j]:%d lengths[i-1][j]: %d lengths[i][j-1]: %d",i,j,a[i-1],b[j-2],b[j-1],ij_considered,lengths[i][j],lengths[i-1][j],lengths[i][j-1]);			
		
			j-= 1;
			ij_considered=0;
		}
		else if( (c1_op2) ) //&& (!c1_op1) && (!c2_op1) )
		{
			//*--result = a[i-1];

			result[result_pos]=clip_str_a[i-1];
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
			//printf("\n\t C3 i: %d j: %d a[i-1]: %c b[j-1]: %c and ij_considered: %d lengths[i][j]: %d result_length: %d lengths[i][j-1]: %d top_of_stack: %d",i,j,a[i-1],b[j-1],ij_considered,lengths[i][j],result_length,lengths[i][j-1],top_of_stack);						
			//fprintf(save_rollback_ops,"\n\t C3 i: %d j: %d a[i-1]: %c b[j-1]: %c and ij_considered: %d lengths[i][j]: %d result_length: %d lengths[i][j-1]: %d top_of_stack: %d",i,j,a[i-1],b[j-1],ij_considered,lengths[i][j],result_length,lengths[i][j-1],top_of_stack);						
			i-=1; j-=1;
			ij_considered=0;
		}
		
		if(ij_considered>2)
		{
			stuck_need2exit(i,j,a,b);
			//printf("\n\n\t Exception! i: %d j: %d a[i-1]: %c b[j-1]: %c lengths[i-1][j]: %d lengths[i][j-1]:%d lengths[i][j]:%d  and ij_considered: %d and results: %s ",i,j,a[i-1],b[j-1],lengths[i-1][j],lengths[i][j-1],lengths[i][j],ij_considered,result);					
			//rollback(last_considered_i,last_considered_j,accepted_i,accepted_j,lengths[i][j],a,b,error_percent);
			xy_tuple fixing_end_tuple;//=rollback(i,j,accepted_i,accepted_j,lengths[i][j],a,b,error_percent);
			if( ( i > (lena-10) ) && ( j > (lena-10)) )  int duh_testing=rollback(i,j,a,b);
			if( !( (fixing_end_tuple.i==0) && (fixing_end_tuple.j==0) && ( top_of_stack <0) ) )
			{
				rollback_tuple_pos rolling_back;//=search_stack(fixing_end_tuple,result_pos);
			
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
			/*printf("\n\t After rollover top-of-stack: %d end-of-stack: %d result_pos: %d",top_of_stack,end_of_stack,result_pos);
			fprintf(save_rollback_ops,"\n\t After rollover top-of-stack: %d end-of-stack: %d result_pos: %d \n\t i: %d j: %d ",top_of_stack,end_of_stack,result_pos,i,j);		
			*/	
		}
	// */
    }

	//save_mat_b4exit();
    

	printf("\n\t Finished tracing the matrix, error-inject-count: %d ops-count: %d \n",error_inject_count,ops_count);
    	printf("\n\t Error-percent is %f\n\t",error_percent);	
	result_length=strlen(result);
	printf("\n\t Result: %s \n\n Result-length: %d \n\n",result, result_length);

//	for(i=0;i<result_length;i++)
//		printf("\n\t i: %d result[i]: %c ",i,result[i]);
	
	//free(result);

	for(int i=0;i<lena;i++)
		delete[] lengths[i];
	delete[] lengths;
	
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

			result=lcs(clip_str_a,clip_str_b,error_percent);
		
           // printf("\n\t LCS-result: %s \n\t Length of result: %d \n",result,(int)strlen(result)); // tsitest
		//	printf("\n\t Len-a: %d Len-b: %d \n\n",(unsigned)strlen(clip_str_a),(unsigned)strlen(clip_str_b));
		if( argc==6 )	fclose(save_mat);
		fclose(save_rollback_ops);
           
        }

 return 0;


return 0;
}
