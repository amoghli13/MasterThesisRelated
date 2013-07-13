
#include<iostream>
#include<cstdlib>
#include<cmath>
#include<vector>
#include<sys/time.h>


#ifndef ERROR_INJECT_OPERATOR_H_
#define ERROR_INJECT_OPERATOR_H_

#endif /*ERROR_INJECT_OPERATOR_H_*/

#define type_of_errors 2

#define asymmetric_error 0
#define symmetric_error 1

#define symmetric_error_max 100000
#define seed_reset 10000

using namespace std;

<<<<<<< HEAD
inline static void seed_count_track();
inline int rand_percent(double error_percent) ;
inline int error_inject(double error_percent);

inline static void seed_count_track()
{
=======

inline static void seed_count_track()
 {
>>>>>>> d0f91f73332b90249e245cb101f7adec22cc4724
   	static int seed_count;
   // cout<<"\t seed_count: "<<seed_count;
    if(seed_count % seed_reset==0)
     {
     	struct timeval t1;
     	gettimeofday(&t1,0);
        srand(t1.tv_sec * t1.tv_usec);
        seed_count=0;
     }
    seed_count++;
}

<<<<<<< HEAD
int seed_count=0;



inline int rand_percent(double error_percent) 
{
	cout<<"\n\t Error_percent: "<<error_percent<<"\n";
	seed_count_track();
	return ( ( ( (double) rand() /RAND_MAX )< error_percent ) ? 1 : 0 ) ; 
}

inline int error_inject(double error_percent)
=======

inline int rand_percent(int error_percent) 
{
	//cout<<"\n\t Error_percent: "<<error_percent<<"\n";
	seed_count_track();
	return ( ( ( rand()% 100 )< error_percent ) ? 1 : 0 ) ; 
}

inline int error_inject(int error_percent)
>>>>>>> d0f91f73332b90249e245cb101f7adec22cc4724
{
	// cout<<"\n\t Error_percent: "<<error_percent<<"\n";
	seed_count_track();
	if(rand_percent(error_percent) )
	{
		int error_choice=rand()%type_of_errors;
		
		int factor=0;
		int error_value;
		
		switch(error_choice)
		{
			case symmetric_error: 
				
				if( rand()% 100 > 50 )
					factor=1;
				error_value= rand()% symmetric_error_max ;
				error_value=factor * error_value;
				return error_value;
			
				break;
			case asymmetric_error:
				int error_value= rand()% symmetric_error_max ;
				
				error_value=error_value;
				return error_value;
				
				break;
				
				
		}
	}
		
	else return 0;
	return 0; // Just to remove the warning!
}

<<<<<<< HEAD
=======

>>>>>>> d0f91f73332b90249e245cb101f7adec22cc4724
// using namespace std;

template <class M>
class error_inject_operators
{

        public:
                M operand;
<<<<<<< HEAD
                static double error_percent;
                
		friend	inline int rand_percent(double error_percent) ;
		friend	inline int error_inject(double error_percent);
                
              //  error_inject_operators(int value,int error_value)
              //  {
              //  	operand=value;
              //  	error_percent=error_value;
              //  }
              
               // error_inject_operators(double error_value)
               // {
                	//operand=value;
                //	error_percent=error_value;
                //}                

                M operator + (error_inject_operators arg)
                {
                      cout<<"\n\t At addition overload!! \n";
                        if( !rand_percent(error_percent) )
=======
                int error_percent;
                
                //error_inject_operators(int value,int error_value)
                //{
                //	operand=value;
                //	error_percent=error_value;
                //}

                M operator + (error_inject_operators arg)
                {
                //      cout<<"\n\t At addition overload!! ";
                        if(1)// !rand_percent(error_percent) )
>>>>>>> d0f91f73332b90249e245cb101f7adec22cc4724
                        {
                                return operand + arg.operand;
                        }
                        else
                        {
<<<<<<< HEAD
                              cout<<"\n\t Introudcing error-at addition overload!! "; 
=======
                //              cout<<"\n\t Introudcing error-at addition overload!! \t random-seed is: "<<random_seed;
>>>>>>> d0f91f73332b90249e245cb101f7adec22cc4724
                                return operand + (M)rand()%RAND_MAX;
                        }
                };
                
                M operator + (M value)
                {
                	return operand + value;
                }

                //error_inject_operators operator = (M arg)
                void operator = (M value)
                {
                        operand=value;
                };

                //error_inject_operators operator = (error_inject_operators arg)
                void operator = (error_inject_operators arg)
                {
                        operand=arg.operand;
                };

               int operator == (M value)
                {
                        if( operand==value)
                                return 1;
                        else
                                return 0;
                };

                int operator == (error_inject_operators arg)
                {
                        if( operand==arg.operand)
                                return 1;
                        else
                                return 0;
                };


                M operator * (error_inject_operators arg)
                {
                        return operand * ( arg.operand + error_inject(error_percent) );
                };

                M operator * (M value)
                {
                        return operand * ( value  + error_inject(error_percent) );
                };       
                
                
                M operator / (error_inject_operators arg)
                {

                        return (double) operand/arg.operand;
                };

                
                M operator / (M value)
                {
                        return (double) operand/value;

                };
                
                

                int operator < (error_inject_operators arg)
                {

                        if(1)// !rand_percent(error_percent) )
                        {
                        return operand < arg.operand;
                        }
                        else
                        {
                   //   cout<<"\n\t Introducing error-at lesser than overload!! \t op_1: "<< operand << "\t op_2: "<<arg.operand;
                                return operand > arg.operand;

                        }
               };

                int operator > (error_inject_operators arg)
                {

                        // cout<<"\n\t At greater-than overload!! ";
                        if( 1) // !rand_percent(error_percent) )
                        {
                        return operand > arg.operand;
                        }
                        else
                        {
                    //  cout<<"\n\t Introducing error-at greater than overload!! \t op_1: "<< operand << "\t op_2: "<<arg.operand;
                                return operand < arg.operand;

                        }
                };
		
				int operator >= (error_inject_operators arg)
				{
					if(1)// !rand_percent(error_percent) )
		                        {
						return (operand >= arg.operand);
					}
					else
					{
					      cout<<"\n\t Introducing error-at greater than or equal to overload!! \t op_1: "<< operand << "\t op_2: "<<arg.operand;				
						return (operand < arg.operand);								
					}
				}
		
				int operator <= (error_inject_operators arg)
				{
					if(1) //!rand_percent(error_percent) )
		                        {
						return (operand <= arg.operand);
					}
					else
					{
					      cout<<"\n\t Introducing error-at lesser than or equal to overload!! \t op_1: "<< operand << "\t op_2: "<<arg.operand;				
						return (operand > arg.operand);								
					}
				}
		
                int operator < (M value)
                {
                        return operand < value;

                };

                int operator > (M value) // Should I also make int or float versions?
                {
                        return operand > value;

                };


                friend ostream & operator << ( ostream &out, error_inject_operators arg )
                {
                        out << arg.operand;
                        return out;

                };

<<<<<<< HEAD
};






=======

};
>>>>>>> d0f91f73332b90249e245cb101f7adec22cc4724
