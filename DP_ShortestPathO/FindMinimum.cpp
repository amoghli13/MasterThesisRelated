#include "error_inject_operator.h"
#define INT_MAX 2147483640

template <class M>
double error_inject_operators<M>::error_percent=0.000;

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


int main()
{

	error_inject_operators<int>::error_percent=0.000;
	cout<<"\n\t Well! Hello there :-D error-percent: "<<(error_inject_operators<int>::error_percent)<<"\n\n";
	
	
	error_inject_operators<int>** Mat;
	int L=500;
	int W=500;
	
	Mat=new error_inject_operators<int>*[L];
	
	for(int i=0;i<L;i++)
		Mat[i]= new error_inject_operators<int>[W];
	
	int MatSize= W*L; 

    	struct timeval t1;
     	gettimeofday(&t1,0);
        srand(t1.tv_sec * t1.tv_usec);


	for(int i=0;i<L;i++)
		for(int j=0;j<W;j++)
			Mat[i][j]= rand() % MatSize;

	for(int i=0;i<L;i++)
	{
		//cout<<"\n\t ";
		for(int j=0;j<W;j++)
		{
			//cout<<"\t "<<Mat[i][j];	
			
		}
	}


	error_inject_operators<int>** MinMat;
	error_inject_operators<int>** MinMatPristine;
	int CheckpointLength= 10; 
	
	
	MinMat=new error_inject_operators<int>*[L];
	MinMatPristine=new error_inject_operators<int>*[L];
	
	int MinW= (int) ( ((W-1)/CheckpointLength) +1 );
	
	cout<<"\n\t MinW: "<<MinW<<" L: "<<L;
	for(int j=0;j<L;j++)
	{
		MinMat[j]=new error_inject_operators<int>[MinW];
		MinMatPristine[j]=new error_inject_operators<int>[MinW];
	}
		

    cout<<"\n MinMat --no-error-inject \n"		;
	for(int i=0;i<L;i++)
	{
		cout<<"\n\t ";
		for(int j=0;j<MinW;j++)
		{
			MinMatPristine[i][j]= INT_MAX;
			for(int k=0,l=(j*CheckpointLength);( (k<CheckpointLength) && (l<W) );k++,l++)
			{
				//cout<<"\n\t i "<<i<<" j "<<j<<" k "<<k<<" l "<<l<<" MinMat[i][j] "<<(MinMat[i][j]);
				if( Mat[i][l] < MinMatPristine[i][j] )
					MinMatPristine[i][j]=Mat[i][l];
			}
			cout<<"\t "<<MinMatPristine[i][j];
		}

	}
	error_inject_operators<int>::error_percent=0.001;
	cout<<"\n\t Well! Hello there :-D error-percent: "<<(error_inject_operators<int>::error_percent)<<"\n\n";
	error_inject_operators<int> c1;
	error_inject_operators<int>  c2;
	error_inject_operators<int> temp;
	error_inject_operators<int> temp1;
	error_inject_operators<bool> temp2;	
	error_inject_operators<int> temp3;	
	error_inject_operators<int> tempCheckpointLength;
	tempCheckpointLength=CheckpointLength;
	error_inject_operators<bool> NeedToRepeat;
	NeedToRepeat=true;
				
	for(int i=0;i<L;i++)
	{
		cout<<"\n\t ";
		for(int j=0;j<MinW;j++)
		{
		    NeedToRepeat=true;
		    
			while(NeedToRepeat.operand)
			{
				MinMat[i][j]= INT_MAX;
				c1=0;
				c2=CheckpointLength;

				for(int k=0,l=(j*CheckpointLength);( (k<CheckpointLength) && (l<W) );k++,l++)
				{
					// cout<<"\n\t i "<<i<<" j "<<j<<" k "<<k<<" l "<<l<<" MinMat[i][j] "<<(MinMat[i][j]);
					// cout<<"\n\t -- MinMat[i][j] "<<(MinMat[i][j])<<" Mat[i][l] "<<(Mat[i][l]);
					 temp=(MinMat[i][j]-Mat[i][l]);
					 temp1=(MinMat[i][j]);
					if( Mat[i][l] < MinMat[i][j] )
						{
							//cout<<"\n\t Min: "<<(MinMat[i][j])<<" Mat "<<(Mat[i][l]);
							MinMat[i][j]=Mat[i][l];
							c1=c1+1;
							
						}

					 
					 if((temp>0) || (Mat[i][l] < temp1) )
					 {
					 	c2=c2-1;
					 	//cout<<"\n\t temp "<<temp<<" Mat[i][l]: "<<(Mat[i][l])<<" temp1 "<<(temp1);
					 }
				}

				 // cout<<"\n\n\t ** c1: "<<c1<<" c2: "<<c2<<"\n\n";
				 temp3=c1+c2;
				 temp2=tmr_equal(temp3,tempCheckpointLength);
				 if( (c1+c2)== CheckpointLength )
				 	NeedToRepeat=false;
				 else
				 	{
				 		NeedToRepeat=true;
				 		cout<<"\n\t i "<<i<<" j "<<j;
				 		cout<<"\n\n\t *++* c1: "<<c1<<" c2: "<<c2<<"\n\n";
				 		cout<<"\n\t c1+c2 != CheckpointLength "<<(c1+c2)<<" CheckpointLength: "<<(CheckpointLength)<<"\n";
				 		// exit(-1);
				 	}
				 	
			}
			
			//cout<<"\t "<<MinMat[i][j];
		}

	}

	error_inject_operators<int>::error_percent=0.00;

	for(int i=0;i<L;i++)
	{
		cout<<"\n\t ";
		for(int j=0;j<MinW;j++)
		{
			cout<<"\t "<<(MinMat[i][j])	;
		}
	}	
	
	for(int i=0;i<L;i++)
	{
		for(int j=0;j<MinW;j++)
		{
			if(MinMatPristine[i][j]!= MinMat[i][j])
				cout<<"\n\t i "<<i<<j<<" MinMatPristine[i][j]: "<<(MinMatPristine[i][j])<<" MinMat[i][j] "<<(MinMat[i][j])<<endl;
			//cout<<"\t "<<(MinMat[i][j])	;
		}
	}	
	cout<<"\n\n";
return 0;
}
