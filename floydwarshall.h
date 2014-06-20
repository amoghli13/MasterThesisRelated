#include <iostream>
#include <set>
#include <vector>
#include<cstdio>
#include<cstdlib>
#include "error_inject_operator.h"
using namespace std;
  
const int MAX = 1001;
const int MAXINT = 1000000000;

#define INTMAX 10000 
 
template <class M>
double  error_inject_operators<M>::error_percent=0.0;
int n;
int NumRows;
int NumCols;
int SetSize;
int NumRowsbySetSize;
int NumColsbySetSize;

struct MaxMin
{
public:
	error_inject_operators<int>** MinMat;
	error_inject_operators<int>** MaxMat;
};

inline error_inject_operators<bool> tmr_equal( error_inject_operators<int> a1,error_inject_operators<int> a2);
inline error_inject_operators<int> tmr_lesser( error_inject_operators<int> a1,error_inject_operators<int> a2);


void PrintMat(error_inject_operators<int>** UpdateMat)
{
    for(int i=0;i<NumRows;i++)
    {
        cout<<"\n";
		for(int j=0;j<NumCols;j++)
			cout<<"\t "<<(UpdateMat[i][j]); 
    }

return;
} 

void FindMaxMin(error_inject_operators<int>** Mat,struct MaxMin& MaxMinofMatRows,struct MaxMin& MaxMinofMatCols)
{


		error_inject_operators<int> MaxCounter1;error_inject_operators<int> MaxCounter2;	
		error_inject_operators<int> MinCounter1;error_inject_operators<int> MinCounter2;	
		error_inject_operators<int> MinVal;error_inject_operators<int> MaxVal;
		error_inject_operators<int> diff;
		error_inject_operators<int> diff1;
		
		bool NeedToRepeat=true;

			
	for(int i=0;i<NumRowsbySetSize;i++)
	{

		for(int j=0;j<NumCols;j++)
		{
			int start=(SetSize*i);
			int bound= start+SetSize;
			if(bound > NumRows)
				bound=NumRows;
		
			MaxVal=0;
		
			MaxCounter1=0;MaxCounter2=SetSize;
			// cout<<"\n\t i: "<<i<<" j "<<j<<" start "<<start<<" bound "<<bound<<" SetSize "<<SetSize<<endl;
			NeedToRepeat=true;
			while(NeedToRepeat)
			{
			
				MinCounter1=0;MinCounter2=SetSize;
				MinVal=INTMAX;
			
			
				for(int k=start;(k < bound);k++)
				{
				    diff= MinVal-Mat[k][j];
				    // cout<<"\n\t i: "<<i<<" k "<<k<<" Mat[k][j]: "<<Mat[k][j]<<" MinVal "<<MinVal;
					if(MinVal>Mat[k][j])
					{
						// cout<<"\n\t MinVal: "<<MinVal<<" Mat[i][k] "<<Mat[i][k];
						MinVal=Mat[k][j];
						MinCounter1=MinCounter1+1;
					
					}
				
					if(diff>0)
					{
						MinCounter2=MinCounter2-1;
					}
				
			
				}
				diff= MinCounter1 + MinCounter2;
				diff1=SetSize;
				error_inject_operators<bool> EqualCompare;
				EqualCompare= tmr_equal(diff,diff1);
				if( EqualCompare.operand )
						NeedToRepeat=false;
				else
				{
						NeedToRepeat=true;			
						cout<<"\n\t i "<<i<<" j "<<j;
						cout<<"\n\t Counter1: "<<MinCounter1<<" Counter2: "<<MinCounter2;
						cout<<"\n\t c1+c2 != CheckpointLength "<<(diff)<<" CheckpointLength: "<<(diff1)<<"\n";
				}
	
			}

			NeedToRepeat=true;
			while(NeedToRepeat)
			{
			
				MaxCounter1=0;MaxCounter2=SetSize;
				MaxVal=0;
			
			
				for(int k=start;(k < bound);k++)
				{
				    diff= MaxVal-Mat[k][j];
				    // cout<<"\n\t i: "<<i<<" k "<<k<<" Mat[k][j]: "<<Mat[k][j]<<" MaxVal "<<MaxVal;
					if(MaxVal < Mat[k][j])
					{
						// cout<<"\n\t MaxVal: "<<MaxVal<<" Mat[i][k] "<<Mat[i][k];
						MaxVal=Mat[k][j];
						MaxCounter1=MaxCounter1+1;
					
					}
				
					if(diff < 0)
					{
						MaxCounter2=MaxCounter2-1;
					}
				
			
				}
				diff= MaxCounter1 + MaxCounter2;
				diff1=SetSize;
				error_inject_operators<bool> EqualCompare;
				EqualCompare= tmr_equal(diff,diff1);
				if( EqualCompare.operand )
						NeedToRepeat=false;
				else
				{
						NeedToRepeat=true;			
						cout<<"\n\t i "<<i<<" j "<<j;
						cout<<"\n\t Counter1: "<<MaxCounter1<<" Counter2: "<<MaxCounter2;
						cout<<"\n\t c1+c2 != CheckpointLength "<<(diff)<<" CheckpointLength: "<<(diff1)<<"\n";
				}
	
			}

			MaxMinofMatRows.MaxMat[i][j]=MaxVal;	
			MaxMinofMatRows.MinMat[i][j]=MinVal;		
			//cout<<"\n\t Row "<<i<<" Col "<<j<<" MinVal "<<MinVal<<" MaxMinofMatRows.MinMat[i][j] "<<(MaxMinofMatRows.MinMat[i][j])<<endl;
			//cout<<"\n\t Row "<<i<<" Col "<<j<<" MaxVal "<<MaxVal<<" MaxMinofMatRows.MaxMat[i][j] "<<(MaxMinofMatRows.MaxMat[i][j])<<endl;
		
		}
	}

	for(int i=0;i<NumRows;i++)
	{

		for(int j=0;j<NumColsbySetSize;j++)
		{
			int start=(SetSize*j);
			int bound= start+SetSize;
			if(bound > NumCols)
				bound=NumCols;
		
			MaxVal=0;
		
			MaxCounter1=0;MaxCounter2=SetSize;
			// cout<<"\n\t i: "<<i<<" j "<<j<<" start "<<start<<" bound "<<bound<<" SetSize "<<SetSize<<endl;
			NeedToRepeat=true;
			while(NeedToRepeat)
			{
			
				MinCounter1=0;MinCounter2=SetSize;
				MinVal=INTMAX;
			
			
				for(int k=start;(k < bound);k++)
				{
				    diff= MinVal-Mat[i][k];
				    // cout<<"\n\t i: "<<i<<" k "<<k<<" Mat[k][j]: "<<Mat[k][j]<<" MinVal "<<MinVal;
					if(MinVal>Mat[i][k])
					{
						// cout<<"\n\t MinVal: "<<MinVal<<" Mat[i][k] "<<Mat[i][k];
						MinVal=Mat[i][k];
						MinCounter1=MinCounter1+1;
					
					}
				
					if(diff>0)
					{
						MinCounter2=MinCounter2-1;
					}
			
				}
				diff= MinCounter1 + MinCounter2;
				diff1=SetSize;
				error_inject_operators<bool> EqualCompare;
				EqualCompare= tmr_equal(diff,diff1);
				if( EqualCompare.operand )
						NeedToRepeat=false;
				else
				{
						NeedToRepeat=true;			
						cout<<"\n\t i "<<i<<" j "<<j;
						cout<<"\n\t Counter1: "<<MinCounter1<<" Counter2: "<<MinCounter2;
						cout<<"\n\t c1+c2 != CheckpointLength "<<(diff)<<" CheckpointLength: "<<(diff1)<<"\n";
				}
	
			}

			NeedToRepeat=true;
			while(NeedToRepeat)
			{
			
				MaxCounter1=0;MaxCounter2=SetSize;
				MaxVal=0;
			
			
				for(int k=start;(k < bound);k++)
				{
				    diff= MaxVal-Mat[i][k];
				    // cout<<"\n\t i: "<<i<<" k "<<k<<" Mat[k][j]: "<<Mat[k][j]<<" MaxVal "<<MaxVal;
					if(MaxVal < Mat[i][k])
					{
						// cout<<"\n\t MaxVal: "<<MaxVal<<" Mat[i][k] "<<Mat[i][k];
						MaxVal=Mat[i][k];
						MaxCounter1=MaxCounter1+1;
					
					}
				
					if(diff < 0)
					{
						MaxCounter2=MaxCounter2-1;
					}
				
			
				}
				diff= MaxCounter1 + MaxCounter2;
				diff1=SetSize;
				error_inject_operators<bool> EqualCompare;
				EqualCompare= tmr_equal(diff,diff1);
				if( EqualCompare.operand )
						NeedToRepeat=false;
				else
				{
						NeedToRepeat=true;			
						cout<<"\n\t i "<<i<<" j "<<j;
						cout<<"\n\t Counter1: "<<MaxCounter1<<" Counter2: "<<MaxCounter2;
						cout<<"\n\t c1+c2 != CheckpointLength "<<(diff)<<" CheckpointLength: "<<(diff1)<<"\n";
				}
	
			}

			MaxMinofMatCols.MaxMat[i][j]=MaxVal;	
			MaxMinofMatCols.MinMat[i][j]=MinVal;		
			//cout<<"\n\t Row "<<i<<" Col "<<j<<" MinVal "<<MinVal<<" MaxMinofMatCols.MinMat[i][j] "<<(MaxMinofMatCols.MinMat[i][j])<<endl;
			//cout<<"\n\t Row "<<i<<" Col "<<j<<" MaxVal "<<MaxVal<<" MaxMinofMatCols.MaxMat[i][j] "<<(MaxMinofMatCols.MaxMat[i][j])<<endl;
		
		}
	}

return;

}

inline error_inject_operators<bool> tmr_equal( error_inject_operators<int> a1,error_inject_operators<int> a2)
{
 	error_inject_operators<bool> res1,res2,res3;
 	res1= (a1.operand == a2.operand);
 	res2= (a1.operand == a2.operand);
 	res3= (a1.operand == a2.operand);
 	error_inject_operators<bool> result;
 	if(  res1 == res2 )
 	{
 		result=res1;
 	}
 	else if (res2 == res3) 
 	{
 		result=res2; 	
 	}
 	else if (res1 == res3)
 	{
 		result=res3; 	
 	}
 	else
 	{
 		cout<<"\n\t FATAL tmr_equal op1 "<<a1<<" op2 "<<a2;
	 	cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" are not equal and hence, exitting! \n ";
	 	exit(-1);
 	}
 	
 	//cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" result "<<result<<endl;
 	//exit(-1);
 	return result;
}

inline error_inject_operators<int> tmr_lesser( error_inject_operators<int> a1,error_inject_operators<int> a2)
{
 	error_inject_operators<int> res1,res2,res3;
 
 	if(a1 < a2)
 		res1=a1;
 	else
 		res1=a2;
 
 	if(a1 < a2)
 		res2=a1;
 	else
 		res2=a2;

 	if(a1 < a2)
 		res3=a1;
 	else
 		res3=a2;

 	error_inject_operators<int> result;
 	if(  res1 == res2 )
 	{
 		result=res1;
 	}
 	else if (res2 == res3) 
 	{
 		result=res2; 	
 	}
 	else if (res1 == res3)
 	{
 		result=res3; 	
 	}
 	else
 	{
 		cout<<"\n\t FATAL tmr_greater op1 "<<a1<<" op2 "<<a2;
	 	cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" are not equal and hence, exitting! \n ";
	 	exit(-1);
 	}
 	
 	//cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" result "<<result<<endl;
 	return result;
}

inline error_inject_operators<bool> tmr_lesser_bool( error_inject_operators<int> a1,error_inject_operators<int> a2)
{
 	error_inject_operators<bool> res1,res2,res3;
 	res1= a1 < a2;
 	res2= a1 < a2;
 	res3= a1 < a2;
 	error_inject_operators<bool> result;
 	if(  res1 == res2 )
 	{
 		result=res1;
 	}
 	else if (res2 == res3) 
 	{
 		result=res2; 	
 	}
 	else if (res1 == res3)
 	{
 		result=res3; 	
 	}
 	else
 	{
 		cout<<"\n\t FATAL tmr_greater op1 "<<a1<<" op2 "<<a2;
	 	cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" are not equal and hence, exitting! \n ";
	 	exit(-1);
 	}
 	
 	//cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" result "<<result<<endl;
 	return result;
}

inline error_inject_operators<bool> tmr_greater_bool( error_inject_operators<int> a1,error_inject_operators<int> a2)
{
 	error_inject_operators<bool> res1,res2,res3;
 	res1= a1 > a2;
 	res2= a1 > a2;
 	res3= a1 > a2;
 	error_inject_operators<bool> result;
 	if(  res1 == res2 )
 	{
 		result=res1;
 	}
 	else if (res2 == res3) 
 	{
 		result=res2; 	
 	}
 	else if (res1 == res3)
 	{
 		result=res3; 	
 	}
 	else
 	{
 		cout<<"\n\t FATAL tmr_greater op1 "<<a1<<" op2 "<<a2;
	 	cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" are not equal and hence, exitting! \n ";
	 	exit(-1);
 	}
 	
 	//cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" result "<<result<<endl;
 	return result;
}
