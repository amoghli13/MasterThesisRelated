#include "floydwarshall.h"

void TMRFloydWarshall(error_inject_operators<int>** UpdateMat,double ErrorPercent)
{

	error_inject_operators<int>::error_percent=ErrorPercent;
	error_inject_operators<int>* temp;
	error_inject_operators<int>	MinK;
	
    for(int i=0;i<NumRows;i++)
    {
     	cout<<"\n i "<<i    ;
		//RowbySetSize=(int) (i/SetSize);
    	for(int j=0;j<NumCols;j++)
		{
		
			temp=new error_inject_operators<int> [j];
			for(int k=0;k<j;k++)
			{
				temp[k]=tmr_add(UpdateMat[i][k],UpdateMat[k][j]);
			}
			MinK=INTMAX;
			
			for(int k=0;k<j;k++)
			{
				MinK=tmr_lesser(MinK,temp[k]);
			}
			UpdateMat[i][j]=MinK;
			delete[] temp;
		}
	}

return;
}

void PristineFloydWarshall(error_inject_operators<int>** UpdateMat,double ErrorPercent)
{

	error_inject_operators<int>::error_percent=0.0;
	error_inject_operators<int>* temp;
	error_inject_operators<int>	MinK;

    for(int i=0;i<NumRows;i++)
    {
     	cout<<"\n i "<<i    ;
		
    	for(int j=0;j<NumCols;j++)
		{

			temp=new error_inject_operators<int> [j];
			for(int k=0;k<j;k++)
			{
				temp[k]=UpdateMat[i][k]+UpdateMat[k][j];
			}
			MinK=INTMAX;

			for(int k=0;k<j;k++)
			{
				if(MinK>temp[k])
					MinK=temp[k];
			}
			UpdateMat[i][j]=MinK;
			delete[] temp;
		}
	}

return;
}	 



void FloydWarshall(error_inject_operators<int>** UpdateMat,double ErrorPercent,struct MaxMin& MaxMinofMatRows,struct MaxMin& MaxMinofMatCols)
{
	error_inject_operators<int>::error_percent=ErrorPercent;

    cout<<"\n\t ErrorPercent "<<(error_inject_operators<int>::error_percent)<<endl;
    error_inject_operators<int>* temp;
    error_inject_operators<int>	Counter1;
    error_inject_operators<int>	Counter2;
    error_inject_operators<int>	MinK;
    error_inject_operators<int>	MaxK;
    error_inject_operators<int> diff;
    error_inject_operators<int> diff1;
    
    error_inject_operators<int> MinRowCol;
    error_inject_operators<int> MaxRowCol;
    error_inject_operators<int> SetMaxFoundPrevIter;
    
    error_inject_operators<bool> Compare;
    error_inject_operators<bool> Compare1;
    error_inject_operators<bool> Compare2;
    error_inject_operators<bool> NeedToRepeatMin;
    error_inject_operators<bool> NeedToRepeatMax;
    error_inject_operators<bool> NeedToRepeatforSet;
    
    int RowbySetSize;
    int ColbySetSize;		
    
    	
    for(int i=0;i<NumRows;i++)
    {
     	cout<<"\n i "<<i    ;
		RowbySetSize=(int) (i/SetSize);
    	for(int j=0;j<NumCols;j++)
		{

			ColbySetSize=(int) (j/SetSize);
    		temp=new error_inject_operators<int> [j];
			NeedToRepeatMin=true;

    		int JbySetSize= (int) ( ( (j-1)/SetSize)  + 1 );
    		
 	    		for(int l=0;l< JbySetSize ; l++)
	    		{
	    			int JStart=( l* SetSize);
	    			int JBound= ( (l+1)*SetSize ) ;
	    			if(JBound>j) 
	    				JBound=j;

	    			NeedToRepeatforSet=true;
	    			SetMaxFoundPrevIter=0;
	    			while(NeedToRepeatforSet.operand)
	    			{
						NeedToRepeatMin=true;
						MaxK=0;MinK=INTMAX;
						while(NeedToRepeatMin.operand)
						{
							//cout<<"\n\t i "<<i<<" j "<<j<<" l "<<l<<" JStart "<<JStart<<" JBound "<<JBound;
							for(int k=JStart;k<JBound;k++)
							{
								temp[k]=(UpdateMat[i][k]+UpdateMat[k][j]);
								//cout<<"\n\t i: "<<i<<" j: "<<j<<" k: "<<k<<" temp[k] "<<temp[k]<<" UpdateMat[i][k] "<<(UpdateMat[i][k])<<" UpdateMat[k][j] "<<(UpdateMat[k][j]);
							}
						
							Counter1=0; Counter2=SetSize;MinK=INTMAX;
							for(int k=JStart;k<JBound;k++)
							{
								diff=( MinK - temp[k]);
								if( temp[k].ABSL(MinK) )
								{
									MinK=temp[k];
									Counter1=Counter1+1;
								}	
						
								//if(diff.ABSG(0) ) //|| (temp[k] < MinK))
								if( diff > 0 )
								{
										Counter2=Counter2-1;    			
								}
					
							}
							diff= Counter1 + Counter2;
							diff1=SetSize;
						
							Compare= tmr_equal(diff,diff1);
							if( Compare.operand )
								NeedToRepeatMin=false;
							else
							{
								NeedToRepeatMin=true;    		
								//cout<<"\n\t i "<<i<<" j "<<j;
								//cout<<"\n\t Counter1: "<<Counter1<<" Counter2: "<<Counter2;
								//cout<<"\n\t c1+c2 != CheckpointLength, c1+c2:  "<<(diff)<<" CheckpointLength: "<<(diff1)<<"\n";
							}

						}

					NeedToRepeatMax=true;
					while(NeedToRepeatMax.operand)
					{
					
						Counter1=0; Counter2=SetSize;MaxK=0;
						
						for(int k=JStart;k<JBound;k++)
						
						{
							//cout<<"\n\t k "<<k<<" temp[k] "<<(temp[k])<<" MaxK "<<MaxK;
							diff=( MaxK - temp[k]);
							if( temp[k].ABSG(MaxK) )
							{
								MaxK=temp[k];
								Counter1=Counter1+1;
							}	
						
							//if( diff.ABSL(0) ) //|| (temp[k] < MinK))
							if( diff < 0 )
							{
								Counter2=Counter2-1;				
							}
					
						}
						diff= Counter1 + Counter2;
						diff1=SetSize;
						Compare= tmr_equal(diff,diff1);
						if( Compare.operand )
							NeedToRepeatMax=false;
						else
						{
							NeedToRepeatMax=true;			
							//cout<<"\n\t i "<<i<<" j "<<j;
							//cout<<"\n\t MaxCounter1: "<<Counter1<<" MaxCounter2: "<<Counter2;
							//cout<<"\n\t c1+c2 != CheckpointLength, c1+c2: "<<(diff)<<" CheckpointLength: "<<(diff1)<<"\n";
							//exit(-1);
						}

					}

					//MaxRowCol=(MaxMinofMatRows.MaxMat[RowbySetSize][j]+MaxMinofMatCols.MaxMat[i][ColbySetSize]);
					MaxRowCol=(MaxMinofMatRows.MaxMat[l][j]+MaxMinofMatCols.MaxMat[i][l]);
			
		 			Compare1= tmr_greater_equal_bool(MaxRowCol,MaxK);
		
					if( Compare1.operand )
					{
				
						NeedToRepeatforSet=false;
						//cout<<"\n\t F: Compare.operand "<<(Compare.operand)<<" Compare1.operand "<<Compare1.operand;
					}
					else
					{
						NeedToRepeatforSet=true;
						SetMaxFoundPrevIter=MaxK;
						//cout<<"\n\t MaxRowCol: "<<(MaxRowCol)<<" MaxK: "<<MaxK<<" SetMaxFoundPrevIter "<<SetMaxFoundPrevIter;
						//cout<<"\n\t MaxMinofMatRows.MaxMat[RowbySetSize][j]: "<<(MaxMinofMatRows.MaxMat[RowbySetSize][j])<<" MaxMinofMatCols.MaxMat[i][ColbySetSize] "<<(MaxMinofMatCols.MaxMat[i][ColbySetSize]);
						//cout<<"\n\t l: "<<l<<" RowbySetSize "<<RowbySetSize<<" ColbySetSize "<<(ColbySetSize);
						//cout<<"\n\t MaxMinofMatRows.MaxMat[l][j]: "<<(MaxMinofMatRows.MaxMat[l][j])<<" MaxMinofMatCols.MaxMat[i][l] "<<(MaxMinofMatCols.MaxMat[i][l]);
						//cout<<"\n\t T: Compare.operand "<<(Compare.operand)<<" Compare1.operand "<<Compare1.operand;
					}

					
				}
				
			}

				 //if(j%50==0)
				 //{
				 //	cout<<"\n\t -- i "<<i<<" j "<<j<<" MinK "<<MinK<<" MaxK "<<MaxK<<" UpdateMat[i][j] "<<UpdateMat[i][j];
				 //}
				 diff=tmr_lesser(MinK,UpdateMat[i][j]);
				 UpdateMat[i][j]= diff;
				 //cout<<"\n\t -- i "<<i<<" j "<<j<<" MinK "<<MinK<<" MaxK "<<MaxK<<" UpdateMat[i][j] "<<UpdateMat[i][j];
				
			delete[] temp;
    	}
    }

}


int main(int argc, char* argv[])
{

   double ErrorPercent=0.0;
   if(argc < 4)
   {
   	cout<<"\n\t ERROR: Required input arguments set-size(int) , error-percent(double) and fault model(int) \n\n";
   	exit(-1);
   }

    SetSize=atoi(argv[1]);
    ErrorPercent=atof(argv[2]);
    FaultModel=atoi(argv[3]);
	
    int m, s, t = 0;
  		
   FILE* ip_file_handle;
   string ip_file="inputs.txt";
   ip_file_handle=fopen(ip_file.c_str(),"r");

    fscanf(ip_file_handle," %d %d %d %d",&n,&m,&s,&t);
    printf("\n\t N: %d M: %d S: %d T: %d ",n,m,s,t);

	error_inject_operators<int>::error_percent=0.0; 
	error_inject_operators<int>::fault_model=FaultModel;

	error_inject_operators<int>** UpdateMat;
	error_inject_operators<int>** UpdateMatPristine;
	UpdateMat=new error_inject_operators<int>*[n];
	UpdateMatPristine=new error_inject_operators<int>*[n];
	
	for(int i=0; i<n;i++)
	{
		UpdateMat[i]=new error_inject_operators<int> [n];
		UpdateMatPristine[i]=new error_inject_operators<int> [n];
	}
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			UpdateMat[i][j]= INTMAX ;
			UpdateMatPristine[i][j]= INTMAX ;	
		}
	}
	
	
    for (int i = 0; i < m; i++)
    {
        int a, b, w , d= 0;
        //scanf("%d %d %d", &a, &b, &w);
        fscanf( ip_file_handle,"%d %d %d %d", &a, &b, &w,&d);
        // printf("\n %d %d %d %d\n", a, b, w,d);
 
		if(d)		      
		{
			UpdateMat[a][b]=w;
			UpdateMatPristine[a][b]=w;
		}
		else
		{
			UpdateMat[b][a]=w;
			UpdateMatPristine[b][a]=w;
		}
      
    }
    
    
    struct MaxMin MaxMinofMatRows;
    struct MaxMin MaxMinofMatCols;
 
    NumRows=n;
    NumCols=n;
   // SetSize=10; // Ensure SetSize to be multiple of Rows and Cols.


 //   PrintMat(UpdateMat);	
    NumRowsbySetSize= ( (int) ( (NumRows-1)/SetSize  ) + 1) ;
    NumColsbySetSize= ( (int) ( (NumCols-1)/SetSize  ) + 1) ;
    
//	MaxMinofMatRows.MinMat= new error_inject_operators<int>* [NumRowsbySetSize];
	MaxMinofMatRows.MaxMat= new error_inject_operators<int>* [NumRowsbySetSize];

//	MaxMinofMatCols.MinMat= new error_inject_operators<int>* [NumRows];
	MaxMinofMatCols.MaxMat= new error_inject_operators<int>* [NumRows];
	cout<<"\n\t SetSize: "<<SetSize<<endl;
	
	for(int i=0;i<NumRowsbySetSize;i++)
	{
		//MaxMinofMatRows.MinMat[i]=new error_inject_operators<int> [NumCols];
		MaxMinofMatRows.MaxMat[i]=new error_inject_operators<int> [NumCols];	
	}
    	
    for(int i=0;i<NumRows;i++)
	{
		//MaxMinofMatCols.MinMat[i]=new error_inject_operators<int> [NumColsbySetSize];
		MaxMinofMatCols.MaxMat[i]=new error_inject_operators<int> [NumColsbySetSize];
	}	

    // FindMaxMin(UpdateMat,MaxMinofMatRows,MaxMinofMatCols,ErrorPercent);
    
     cout<<"\n\n";
    //FloydWarshall(UpdateMatPristine,0.00,MaxMinofMatRows,MaxMinofMatCols);
     //TMRFloydWarshall(UpdateMat,ErrorPercent);
     PristineFloydWarshall(UpdateMat,ErrorPercent);
   //  FloydWarshall(UpdateMat,ErrorPercent,MaxMinofMatRows,MaxMinofMatCols);
   // PrintMat(UpdateMat);	
   // CompareMat(UpdateMat,UpdateMatPristine,0.0);
    cout<<"\n\n";
 
    return 0;
}
