
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
#define max(a,b) (a>b?a:b)
using namespace std;


inline static void seed_count_track()
 {
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
int seed_count=0;

inline int rand_percent(double error_percent) 
{
	//cout<<"\n\t Error_percent: "<<error_percent; //<<" and duh: "<<duh<<"\n";
	seed_count_track();
	return ( ( ( (double)rand()/RAND_MAX)< error_percent ) ? 1 : 0 ) ; 
}

inline int error_inject(double error_percent)
{
	// cout<<"\n\t Error_percent: "<<error_percent<<"\n";
	seed_count_track();
	if(rand_percent(error_percent) )
	{
		int error_choice=rand()%type_of_errors;
		
		int factor=1;
		int error_value;
		
		switch(error_choice)
		{
			case symmetric_error: 
				
				if( rand()% 100 > 50 )
					factor=-1;
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


// using namespace std;

template <class M>
class error_inject_operators
{

        public:
                M operand;
                static double error_percent;

                M operator + (error_inject_operators arg)
                {
                      	//cout<<"\n\t At addition overload with another arg of class M \n ";
                        if(!rand_percent(error_percent) )
                        {
                                return operand + arg.operand;
                        }
                        else
                        {
                             	 cout<<"\n\t Introudcing error-at addition overload with another arg of class M";
                                return operand + ( arg.operand + ( (M)rand()%RAND_MAX ) );
                        }
                };
                
                M operator + (M value)
                {
                	//cout<<"\n\t At addition overload with value! \n";
                	if(!rand_percent(error_percent))
                	{
                		return operand + value;
                	}
                	else
                	{
                		cout<<"\n\t Introducing error-at addition overload with value ";
                		return operand + ( value + ( (M) rand() %RAND_MAX ) );
                	}
                }
                
                M operator - (error_inject_operators arg)
                {
                      	//cout<<"\n\t At addition overload with another arg of class M \n ";
                        if(!rand_percent(error_percent) )
                        {
                                return operand - arg.operand;
                        }
                        else
                        {
                             	 cout<<"\n\t Introudcing error-at subtraction overload with another arg of class M";
                                return operand - ( arg.operand + ( (M)rand()%RAND_MAX ) );
                        }
                };
                
                M operator - (M value)
                {
                	//cout<<"\n\t At addition overload with value! \n";
                	if(!rand_percent(error_percent))
                	{
                		return operand - value;
                	}
                	else
                	{
                		cout<<"\n\t Introducing error-at subtraction overload with value ";
                		return operand - ( value + ( (M) rand() %RAND_MAX ) );
                	}
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
			if(!rand_percent(error_percent))
		   	{
		                if( operand ==value)
		                        return 1;
		                else
		                        return 0;
                         }
                         else
                         {
                         	cout<<"\n\t Introducing error at op==(M value) ";
		                if( operand == (value + ( (M) rand() %RAND_MAX ) ) )
		                        return 1;
		                else
		                        return 0;                         
                         
                         }
                };

                int operator == (error_inject_operators arg)
                {
			if(!rand_percent(error_percent))
		   	{
		                if( operand ==arg.operand)
		                        return 1;
		                else
		                        return 0;
                         }
                         else
                         {
                         	cout<<"\n\t Introducing error at op==(arg) ";                         
		                if( operand == (arg.operand + ( (M) rand() %RAND_MAX ) ) )
		                        return 1;
		                else
		                        return 0;                         
                         
                         }
                };
                
                
               int operator != (M value)
                {
			if(!rand_percent(error_percent))
		   	{
		                if( operand !=value)
		                        return 1;
		                else
		                        return 0;
                         }
                         else
                         {
                         	cout<<"\n\t Introducing error at op!=(M value) ";                         
		                if( operand != (value + ( (M) rand() %RAND_MAX ) ) )
		                        return 1;
		                else
		                        return 0;                         
                         
                         }
                };

                int operator != (error_inject_operators arg)
                {
			if(!rand_percent(error_percent))
		   	{
		                if( operand !=arg.operand)
		                        return 1;
		                else
		                        return 0;
                         }
                         else
                         {
                         	cout<<"\n\t Introducing error at op!=(arg) ";                         
		                if( operand != (arg.operand + ( (M) rand() %RAND_MAX ) ) )
		                        return 1;
		                else
		                        return 0;                         
                         
                         }
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

                        if( !rand_percent(error_percent) )
                        {
                       	 	return operand < arg.operand;
                        }
                        else
                        {
                   	   cout<<"\n\t Introducing error-at lesser than overload!! \t op_1: "<< operand << "\t op_2: "<<arg.operand;
                                return operand < (arg.operand+ ( (M) rand() %RAND_MAX ) );

                        }
               };

                int operator > (error_inject_operators arg)
                {

                        // cout<<"\n\t At greater-than overload!! ";
                        if( !rand_percent(error_percent) )
                        {
	                        return operand > arg.operand;
                        }
                        else
                        {
                    		  cout<<"\n\t Introducing error-at greater than overload!! \t op_1: "<< operand << "\t op_2: "<<arg.operand;
                                return operand > ( arg.operand + ( (M) rand() %RAND_MAX ) );

                        }
                };
		
				int operator >= (error_inject_operators arg)
				{
					if( !rand_percent(error_percent) )
		                        {
						return (operand >= arg.operand);
					}
					else
					{
					      cout<<"\n\t Introducing error-at greater than or equal to overload!! \t op_1: "<< operand << "\t op_2: "<<arg.operand;				
						return (operand >= ( arg.operand + ( (M) rand() %RAND_MAX ) ));								
					}
				}
		
				int operator <= (error_inject_operators arg)
				{
					if(!rand_percent(error_percent) )
		                        {
						return (operand <= arg.operand);
					}
					else
					{
					      cout<<"\n\t Introducing error-at lesser than or equal to overload!! \t op_1: "<< operand << "\t op_2: "<<arg.operand;				
						return (operand <= ( arg.operand + ( (M) rand() %RAND_MAX ) ) );								
					}
				}
		
                int operator < (M value)
                {
                        if(!rand_percent(error_percent))
                        	return operand < value;
                        else
                        {
                         	cout<<"\n\t Introducing error at op < (M value) ";                        	
                        	return operand < ( value + (M)  ( rand() % RAND_MAX) ) ;
                        }

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
};
