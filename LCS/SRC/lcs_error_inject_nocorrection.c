
//Author: Amoghavarsha Suresh

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#define MAX(A,B) (((A)>(B))? (A) : (B))
#define percent_error(x) ( ( ( (double) rand()/RAND_MAX) < x ) ? 1 : 0)
#define XOR(x,y) ( (x&&(!y)) || ( (!x) && y ) )

int  **lengths;
int lena,lenb;
FILE* save_mat;
int error_inject_count;
int ops_count;

void rollback(int start_i,int start_j,int stop_i,int stop_j,int lengths_value,const char* a,const char* b,double error_percent)
{
	int i,j;
	const char *x,*y;
	int error_inject;
	printf("\n\t Start_i: %d Start_j: %d Stop_i: %d Stop_j: %d ",start_i,start_j,stop_i,stop_j);
	for(i=start_i,x=(a+start_i-1);i<=(stop_i);i++,x++)
	{
		for(j=start_j,y=(b+start_j-1);j<=(stop_j);j++,y++)
		{
			printf("\n\t ** I: %d J: %d lengths[i][j]: %d and x: %c y: %c",i,j,lengths[i][j],*x,*y);
			
			error_inject=percent_error(error_percent);
				 
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
	//exit(-1);

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
	printf("\n\t Lengths[i][j-2]: %d , Lengths[i][j-1]: %d , Lengths[i][j]: %d, Lengths[i][j+1]: %d \n",lengths[i][j-2],lengths[i][j-1],lengths[i][j],lengths[i][j+1]);
	printf("\n\t Lengths[i+1][j-2]: %d , Lengths[i+1][j-1]: %d , Lengths[i+1][j]: %d, Lengths[i+1][j+1]: %d \n",lengths[i+1][j-2],lengths[i+1][j-1],lengths[i+1][j],lengths[i+1][j+1]);

	 
	//exit(-1);
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
 
char * lcs(const char *a,const char *b,double error_percent) 
{
    // int lena = strlen(a)+1;     int lenb = strlen(b)+1;
		lena=strlen(a)+1; lenb=strlen(b)+1;
 //printf("\n\t Length of string-1: %d string-1: %s \n\t Length of string-2: %d string-2: %s \n",lena,a,lenb,b);
    int bufrlen = lena;
    char bufr[bufrlen], *result;

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
    result = bufr+bufrlen;
    *--result = '\0';
	i = lena-1; j = lenb-1;
     while ( (i>0) && (j>0) ) 
 	{
 
 		if(  (lengths[i][j] == lengths[i-1][j]) )
		{
 			i -= 1;	
		}
		else if( (lengths[i][j] == lengths[i][j-1] ) )
		{
 			j-= 1;
 		}
		else
		{
			*--result = a[i-1];
 			i-=1; j-=1;
 		}
    	}

	//save_mat_b4exit();
    
	free(la);
    free(lengths);
	printf("\n\t Finished tracing the matrix, error-inject-count: %d ops-count: %d \n",error_inject_count,ops_count);
    printf("\n\t Error-percent is %f\n\t",error_percent);	
	printf("\n\t Lena: %d Lenb: %d ",lena,lenb);
    return strdup(result);
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
 
 // *************
 
 
 
        if(argc<5)
        {
               // printf("\n\t ERROR:  Expected inputs \n\t\t 1. Output file name \n\t\t 2.Two files. \n\t\t 3. String-length. \n\t\t 4. Error-percent. \n\t ------ Kindly provide appropriate inputs -------- \n\n");
		 printf("\n\t ERROR:  Expected inputs \n\t\t 1.Two files. \n\t\t 2. String-length. \n\t\t 3. Error-percent. \n\t\t 4. Output file name- Optional \n\t ------ Kindly provide appropriate inputs -------- \n\n");               
                exit(-1);
        }
		
		char* filename;
		if(argc==6)
		{
			filename=argv[5];
			save_mat=fopen(filename,"w");
		}
        double error_percent=atof(argv[4]);
		int string_length=atoi(argv[3]);

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
		
            printf("\n\t LCS-result: %s \n\t Length of result: %d \n",result,(int)strlen(result)); // tsitest
			printf("\n\t Len-a: %d Len-b: %d \n\n",(unsigned)strlen(clip_str_a),(unsigned)strlen(clip_str_b));
            return 0;
        }
		fclose(save_mat);
}
