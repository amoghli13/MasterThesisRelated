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
template <class N>
int error_inject_operators<N>::fault_model=0;
int n;
int NumRows;
int NumCols;
int SetSize;
int NumRowsbySetSize;
int NumColsbySetSize;
int FaultModel;

struct MaxMin
{
public:
	error_inject_operators<int>** MinMat;
	error_inject_operators<int>** MaxMat;
};

inline error_inject_operators<bool> tmr_equal( error_inject_operators<int> a1,error_inject_operators<int> a2);
inline error_inject_operators<int> tmr_lesser( error_inject_operators<int> a1,error_inject_operators<int> a2);
inline error_inject_operators<int> tmr_greater( error_inject_operators<int> a1,error_inject_operators<int> a2);

void PrintMat(error_inject_operators<int>** UpdateMat)
{
    for(int i=0;i<NumRows;i++)
    {
        cout<<"\n\n";
		for(int j=0;j<NumCols;j++)
			cout<<"\t "<<(UpdateMat[i][j]); 
    }

return;
} 

void CompareMat(error_inject_operators<int>** UpdateMat,error_inject_operators<int>** UpdateMatPristine,double ErrorPercent)
{
   error_inject_operators<int>::error_percent=ErrorPercent;
   cout<<"\n\t ErrorPercent "<<(error_inject_operators<int>::error_percent)<<endl;
   cout<<"\n\n\n";
   cout<<"\n\n\n\t Following items were not equal to the pristine result ";
   for(int i=0;i<NumRows;i++)
   {
 		for(int j=0;j<NumCols;j++)
		{
			if( UpdateMat[i][j] !=UpdateMatPristine[i][j] )
			cout<<"\n\t i "<<i<<" j "<<j<<" UpdateMat[i][j] "<<(UpdateMat[i][j])<<" UpdateMatPristine[i][j] "<<(UpdateMatPristine[i][j]);  
		}
    }

return;

}


void CompareMatDegradation(error_inject_operators<int>** UpdateMat,error_inject_operators<int>** UpdateMatPristine,double ErrorPercent)
{
   error_inject_operators<int>::error_percent=ErrorPercent;
   cout<<"\n\t ErrorPercent "<<(error_inject_operators<int>::error_percent)<<endl;
   cout<<"\n\n\n";
   cout<<"\n\n\n\t Following items were not equal to the pristine result ";
   long int AccumVal=0;
   long int AccumPristineVal=0;
   long int AllAccum=0;
   long int AllAccumPristine=0;
   int Count=0;
   int temp;
   for(int i=0;i<NumRows;i++)
   {   
                for(int j=0;j<NumCols;j++)
                {  
			//cout<<"\n\t i "<<i<<" j "<<j<<" UpdateMat "<<(UpdateMat[i][j])<<" UpdateMatPristine "<<(UpdateMatPristine[i][j]); 
			AllAccumPristine+=(UpdateMatPristine[i][j].operand);
			AllAccum+=abs(UpdateMat[i][j].operand);
			
                        if( UpdateMat[i][j] !=UpdateMatPristine[i][j] )
                        {
			        Count++;
                                temp= abs(abs(UpdateMat[i][j].operand)-abs(UpdateMatPristine[i][j].operand) );
                                AccumVal+=abs(UpdateMat[i][j].operand);
                                AccumPristineVal+=(UpdateMatPristine[i][j].operand);
				cout<<"\n\t i "<<i<<" j "<<j<<" UpdateMat[i][j] "<<(UpdateMat[i][j])<<" UpdateMatPristine[i][j] "<<(UpdateMatPristine[i][j])<<" abs(abs(Mat)-abs(MatPristine)) "<<temp<<" AcumMatPristine: "<<(AccumPristineVal)<<" AcumMat: "<<(AccumVal)<<endl;  
			}
                }   
    } 
double DiffAccum,PercentDiffAccum;
DiffAccum=abs(AccumVal)-abs(AccumPristineVal);
PercentDiffAccum=(double) ( DiffAccum/AccumPristineVal);
double AllDiffAccum,AllPercentDiffAccum;
AllDiffAccum=abs(AllAccum)-abs(AllAccumPristine);
AllPercentDiffAccum=(double) ( AllDiffAccum / AllAccumPristine);
cout<<"\n\n DiffAccum "<<DiffAccum<<" PercentDiffAccum "<<PercentDiffAccum;
cout<<"\n\n AllDiffAccum "<<AllDiffAccum<<" AllPercentDiffAccum "<<AllPercentDiffAccum;
return;

}



void FindMaxMin(error_inject_operators<int>** Mat,struct MaxMin& MaxMinofMatRows,struct MaxMin& MaxMinofMatCols,double ErrorPercent)
{

		error_inject_operators<int>::error_percent=ErrorPercent;
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
			
				MaxCounter1=0;MaxCounter2=SetSize;
				MaxVal=0;
			
			
				for(int k=start;(k < bound);k++)
				{
				    diff= MaxVal-Mat[k][j];
				   //  cout<<"\n\t 1. i: "<<i<<" j: "<<j<<" k: "<<k<<" Mat[k][j]: "<<Mat[k][j]<<" MaxVal "<<MaxVal;
					if(MaxVal < Mat[k][j])
					{
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
					//	cout<<"\n\t i "<<i<<" j "<<j;
					//	cout<<"\n\t Counter1: "<<MaxCounter1<<" Counter2: "<<MaxCounter2;
					//	cout<<"\n\t c1+c2 != CheckpointLength "<<(diff)<<" CheckpointLength: "<<(diff1)<<"\n";
				}
	
			}

			MaxMinofMatRows.MaxMat[i][j]=MaxVal;				
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
			
				MaxCounter1=0;MaxCounter2=SetSize;
				MaxVal=0;
			
			
				for(int k=start;(k < bound);k++)
				{
				    diff= MaxVal-Mat[i][k];
				   // cout<<"\n\t 2. i: "<<i<<" j: "<<j<<" k: "<<k<<" Mat[i][k]: "<<Mat[i][k]<<" MaxVal "<<MaxVal;
					if(MaxVal < Mat[i][k])
					{
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
						//cout<<"\n\t i "<<i<<" j "<<j;
						//cout<<"\n\t Counter1: "<<MaxCounter1<<" Counter2: "<<MaxCounter2;
						//cout<<"\n\t c1+c2 != CheckpointLength "<<(diff)<<" CheckpointLength: "<<(diff1)<<"\n";
				}
	
			}

			MaxMinofMatCols.MaxMat[i][j]=MaxVal;
		//	cout<<"\n\t Row "<<i<<" Col "<<j<<" MaxVal "<<MaxVal<<" MaxMinofMatCols.MaxMat[i][j] "<<(MaxMinofMatCols.MaxMat[i][j])<<endl;
		
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
	 	//exit(-1);
 	}
 	
 	//cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" result "<<result<<endl;
 	//exit(-1);
 	return result;
}

inline error_inject_operators<int> tmr_add( error_inject_operators<int> a1,error_inject_operators<int> a2)
{
 	error_inject_operators<int> res1,res2,res3;
 	res1= (a1 + a2);
 	res2= (a1 + a2);
 	res3= (a1 + a2);
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
 		cout<<"\n\t FATAL tmr_equal op1 "<<a1<<" op2 "<<a2;
	 	cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" are not equal and hence, exitting! \n ";
	 	//exit(-1);
 	}
 	
 	//cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" result "<<result<<endl;
 	//exit(-1);
 	return result;
}


inline error_inject_operators<bool> tmr_equal( error_inject_operators<int> a1,int a2)
{
 	error_inject_operators<bool> res1,res2,res3;
 	res1= (a1.operand == a2);
 	res2= (a1.operand == a2);
 	res3= (a1.operand == a2);
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
	 	//exit(-1);
 	}
 	
 	//cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" result "<<result<<endl;
 	//exit(-1);
 	return result;
}

inline error_inject_operators<int> tmr_lesser( error_inject_operators<int> a1,error_inject_operators<int> a2)
{
 	error_inject_operators<int> res1,res2,res3;

 	if(a1.ABSL(a2) )
 		res1=a1;
 	else
 		res1=a2;
 
 	if(a1.ABSL(a2) )
 		res2=a1;
 	else
 		res2=a2;

 	if(a1.ABSL(a2) )
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
	 	//exit(-1);
 	}
 	
 	// cout<<"\n\t TMR_L: a1 "<<a1<<" a2 "<<a2<<" res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" result "<<result;
 	return result;
}

inline error_inject_operators<int> tmr_greater( error_inject_operators<int> a1,error_inject_operators<int> a2)
{
 	error_inject_operators<int> res1,res2,res3;
 
	if(a1.ABSG(a2) )
 		res1=a1;
 	else
 		res1=a2;
 
 	if(a1.ABSG(a2) )
 		res2=a1;
 	else
 		res2=a2;

 	if(a1.ABSG(a2) )
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
	 	//exit(-1);
 	}
 	
 	// cout<<"\n\t TMR_L: a1 "<<a1<<" a2 "<<a2<<" res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" result "<<result;
 	return result;
}

inline error_inject_operators<bool> tmr_lesser_equal_bool( error_inject_operators<int> a1,error_inject_operators<int> a2)
{
 	error_inject_operators<bool> res1,res2,res3;
 	res1= a1.ABSLE(a2);
 	res2= a1.ABSLE(a2);
 	res3= a1.ABSLE(a2);
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
	 	//exit(-1);
 	}
 	
 	// cout<<"\n\t TMR_LB: a1 "<<a1<<" a2 "<<a2<<" res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" result "<<result;
 	return result;
}

inline error_inject_operators<bool> tmr_greater_equal_bool( error_inject_operators<int> a1,error_inject_operators<int> a2)
{
 	error_inject_operators<bool> res1,res2,res3;
 	res1= a1.ABSGE(a2);
 	res2= a1.ABSGE(a2);
 	res3= a1.ABSGE(a2);
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
	 	//exit(-1);
 	}
 	
 	// cout<<"\n\t TMR_GB: a1 "<<a1<<" a2 "<<a2<<" res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" result "<<result;
 	return result;
}
