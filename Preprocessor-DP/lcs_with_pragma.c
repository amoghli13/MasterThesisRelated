#include <string.h>
#include <stdlib.h>
#include <stdio.h>
 
#define MAX(A,B) (((A)>(B))? (A) : (B))
 
char * lcs(const char *a,const char * b) 
{
    int lena = strlen(a)+1;
    int lenb = strlen(b)+1;
 
    int bufrlen = 40;
    char bufr[40], *result;
 
    int i,j;
    const char *x, *y;
    int *la = calloc(lena*lenb, sizeof( int));
    int  **lengths = malloc( lena*sizeof( int*));
    for (i=0; i<lena; i++) lengths[i] = la + i*lenb;

 #pragma dynamic_prog mat dimensions 2 i,j
 #pragma dynamic_prog mat size 100, 500
 #pragma dynamic_prog solve num_conditions 3
 #pragma dynamic_prog mat array lengths
// 45651132
//                lengths[i+1][j+1] = lengths[i+1-duh2-duh[i]][j] + duh[i] ; 
 
    for (i=0,i<lena;i++)
   {
         x=a[i];
 #pragma dynamic_prog inner_loop_solve open
 
         for (j=0; j < lenb ;y++ ) 
        {
           y=b[j];
      #pragma dynamic_prog solve cond 1
            if (a[i] == b[j])
            {
               lengths[i][j+1] = lengths[i+1][j] +1 ;
            }
      #pragma dynamic_prog solve cond 2            
            else if (  lengths[i+1][j] > lengths[i][j+1])  )
            {
               lengths[i+1][j+1] = lengths[i+1]][j] ;
   	    }
      #pragma dynamic_prog solve cond 3   	    
            else
            {
               lengths[i+1][j+1] = lengths[i][j+1];
            }
        }
 #pragma dynamic_prog inner_loop_solve close
    }

 #pragma dynamic_prog trace num_conditions 3
    result = bufr+bufrlen;
    *--result = '\0';
    i = lena-1; j = lenb-1;
    #pragma dynamic_prog inner_loop_trace open
    while ( (i>0) && (j>0) )
    {
       #pragma dynamic_prog trace cond 1 solve cond 1
        if (lengths[i][j] == lengths[i-1][j]) 
        {
         	i= i-1;
        }
        #pragma dynamic_prog trace cond 2 solve cond 1
        else if (lengths[i][j] == lengths[i][j-1])
        {
        	 j=j-1;
        }
         #pragma dynamic_prog trace cond 3 solve cond 1
        else
        {
        #pragma dynamig prog trace accept choice
             *--result = a[i-1];
            i-=1; j-=1;
        }
	#pragma dynamic_prog inner_loop_trace close
    }


    free(la); free(lengths);
    return strdup(result);
}

int main()
{
    printf("%s\n", lcs("thisisatest", "testing123testing")); // tsitest
    return 0;
}

