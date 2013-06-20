#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include <math.h>
//#define percent_error(x) ( ( (rand()/RAND_MAX) < x ) ? 1 : 0)
//#define percent_error(x) ( (rand() < x ) ? 1 : 0)
int error_percent(int error_percent_rep );

//int error_percent_func(int error_percent_rep )
int error_percent_func(double error_percent_rep )
{

int error_injection_count=0;
double rand1;
int decision1;
int max_iter=10000;
double error_percent_rep_double= ( (double)  error_percent_rep) ;///RAND_MAX);
for(int iter=0;iter<max_iter;iter++)
{

	rand1= ( (double) rand()/RAND_MAX);
	//decision1=0;
	decision1= ( rand1 < error_percent_rep_double );
	//if( percent_error(error_percent_rep) )
	if(decision1==1)
	{
		error_injection_count++;
		//printf("\n\t ALERT: rand1: %f is less than error_percent_rep: %f, error_injection_count: %d, iter: %d ",rand1,error_percent_rep_double,error_injection_count,iter);
		
	}

}
   printf("\nFor error_percent_rep: %f, error_injection_count: %d out-of-max: %d" , error_percent_rep, error_injection_count, max_iter);
   return 0;
}

int main(int argc, char* argv[])
{
         struct timeval t1; gettimeofday(&t1, NULL); srand(t1.tv_usec * t1.tv_sec);
//#define percent_error(x) ( ( (rand()/RAND_MAX) < x ) ? 1 : 0)
double error_percent;
double error_percent_rep;
error_percent=0.001;
error_percent_rep=error_percent;
//error_percent_rep=(int) ( 1 / (error_percent ));
//error_percent_rep= (int) ( RAND_MAX/error_percent_rep );


//error_percent_rep=100;
printf("\n\t Trial-1: Error-percent : %f and error_percent_rep: %f ",error_percent,error_percent_rep);
error_percent_func(error_percent_rep);
printf("\n\n");

return 0;
}
