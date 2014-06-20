#include "floydwarshall.h"

void FloydWarshall(error_inject_operators<int>** UpdateMat,double ErrorPercent)
{
	error_inject_operators<int>::error_percent=ErrorPercent;
	
    for(int i=0;i<n;i++)
    {
     	// cout<<"\n\n"   ;
		for(int j=0;j<n;j++)
		{
			//cout<<"\t "<<(UpdateMat[i][j]); 
			//cout<<"\n\t i "<<i<<" j "<<j;
			error_inject_operators<int>* temp;
			error_inject_operators<int>	Counter1;
			error_inject_operators<int>	Counter2;
			error_inject_operators<int>	MinK;
			error_inject_operators<int> diff;
			error_inject_operators<int> diff1;
			temp=new error_inject_operators<int> [j];
			error_inject_operators<bool> NeedToRepeat;
			NeedToRepeat=true;
			
			while(NeedToRepeat.operand)
			{
			
				for(int k=0;k<j;k++)
				{
					temp[k]=(UpdateMat[i][k]+UpdateMat[k][j]);
					// cout<<"\n\t k: "<<k<<" temp[k] "<<temp[k];
				}
				
				Counter1=0; Counter2=j;MinK=INTMAX;
				for(int k=0;k<j;k++)
				{
					diff=( MinK - temp[k]);
					if( temp[k] < MinK )
					{
						MinK=temp[k];
						Counter1=Counter1+1;
					}	
				
					if((diff > 0) ) //|| (temp[k] < MinK))
					{
						Counter2=Counter2-1;				
					}
			
				}
				diff= Counter1 + Counter2;
				diff1=j;
				error_inject_operators<bool> EqualCompare;
				EqualCompare= tmr_equal(diff,diff1);
				if( EqualCompare.operand )
					NeedToRepeat=false;
				else
				{
					NeedToRepeat=true;			
					cout<<"\n\t i "<<i<<" j "<<j;
					cout<<"\n\t Counter1: "<<Counter1<<" Counter2: "<<Counter2;
					cout<<"\n\t c1+c2 != CheckpointLength "<<(diff)<<" CheckpointLength: "<<(diff1)<<"\n";
				}

			}
			diff=tmr_lesser(MinK,UpdateMat[i][j]);
			// cout<<"\n\t -- i "<<i<<" j "<<j<<" UpdateMat[i][j] "<<UpdateMat[i][j]<<" MinK "<<MinK<<" TMR_Lesser "<<diff;
			if(diff.operand)
				UpdateMat[i][j]= MinK;
			
				
			
			//	if( (UpdateMat[i][j]) > (UpdateMat[i][k]+UpdateMat[k][j]) )
			//	{
					//cout<<"\n\n\t *** i "<<i<<" j "<<j<<" k "<<k<<" LHS: "<<(UpdateMat[i][j])<<" is greater than RHS: "<<(UpdateMat[i][k])<<" , "<<(UpdateMat[k][j]);
			//		UpdateMat[i][j]= (UpdateMat[i][k]+UpdateMat[k][j]); 
			//	}				
				
		}
	}
	cout<<"\n\n";
    for(int i=0;i<n;i++)
    {
        cout<<"\n";
		for(int j=0;j<n;j++)
			cout<<"\t "<<(UpdateMat[i][j]); 
	}

}

 
int main()
{

    int m, s, t = 0;
    printf("\n\t Enter: \n\t\t 1. number of nodes \n\t\t 2. Number of edges \n\t\t 3. Source node \n\t\t 4. Destination node \n\t\t ");
    //scanf("%d %d %d %d", &n, &m, &s, &t);
		
   FILE* ip_file_handle;
   string ip_file="inputs.txt";
   ip_file_handle=fopen(ip_file.c_str(),"r");

    fscanf(ip_file_handle," %d %d %d %d",&n,&m,&s,&t);
    printf("\n\t N: %d M: %d S: %d T: %d ",n,m,s,t);

	error_inject_operators<int>::error_percent=0.0; 
	
	error_inject_operators<int>** UpdateMat;
	UpdateMat=new error_inject_operators<int>*[n];
	
	for(int i=0; i<n;i++)
		UpdateMat[i]=new error_inject_operators<int> [n];
	
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
			UpdateMat[i][j]= INTMAX ;
	
	
    for (int i = 0; i < m; i++)
    {
        int a, b, w , d= 0;
        //scanf("%d %d %d", &a, &b, &w);
        fscanf( ip_file_handle,"%d %d %d %d", &a, &b, &w,&d);
        // printf("\n %d %d %d %d\n", a, b, w,d);
 
		if(d)		      
			UpdateMat[a][b]=w;
		else
			UpdateMat[b][a]=w;
      
    }
    
    
    struct MaxMin MaxMinofMatRows;
    struct MaxMin MaxMinofMatCols;
    
	int SetSize=3;
	// Ensure SetSize to be multiple of Rows and Cols.
    for(int i=0;i<n;i++)
    {
        cout<<"\n";
		for(int j=0;j<n;j++)
			cout<<"\t "<<(UpdateMat[i][j]); 
    }

    int NumRows=n;
    int NumCols=n;

    int NumRowsbySetSize= ( (int) ( (NumRows-1)/SetSize  ) + 1) ;
    int NumColsbySetSize= ( (int) ( (NumCols-1)/SetSize  ) + 1) ;
    
	MaxMinofMatRows.MinMat= new error_inject_operators<int>* [NumRowsbySetSize];
	MaxMinofMatRows.MaxMat= new error_inject_operators<int>* [NumRowsbySetSize];

	MaxMinofMatCols.MinMat= new error_inject_operators<int>* [NumRows];
	MaxMinofMatCols.MaxMat= new error_inject_operators<int>* [NumRows];
	cout<<"\n\t SetSize: "<<SetSize<<endl;
	
	for(int i=0;i<NumRowsbySetSize;i++)
	{
		MaxMinofMatRows.MinMat[i]=new error_inject_operators<int> [NumCols];
		MaxMinofMatRows.MaxMat[i]=new error_inject_operators<int> [NumCols];	
	}
    	
    	for(int i=0;i<NumRows;i++)
	{
		MaxMinofMatCols.MinMat[i]=new error_inject_operators<int> [NumColsbySetSize];
		MaxMinofMatCols.MaxMat[i]=new error_inject_operators<int> [NumColsbySetSize];
	}	

    FindMaxMin(UpdateMat,NumRows,NumCols,SetSize,MaxMinofMatRows,MaxMinofMatCols);
    
    for(int i=0;i<NumRowsbySetSize;i++)
    {
    	for(int j=0;j<NumCols;j++)
    	{
    	
    		cout<<"\n\t Row: "<<i<<" Col: "<<j<<" Max[Row][Col]: "<<(MaxMinofMatRows.MaxMat[i][j])<<" Min[Row][Col]: "<<(MaxMinofMatRows.MinMat[i][j]);
    	}
    
    }
    cout<<"\n\n";
    for(int i=0;i<NumRows;i++)
    {
    	for(int j=0;j<NumColsbySetSize;j++)
    	{
    	
    		cout<<"\n\t Row: "<<i<<" Col: "<<j<<" Max[Row][Col]: "<<(MaxMinofMatCols.MaxMat[i][j])<<" Min[Row][Col]: "<<(MaxMinofMatCols.MinMat[i][j]);
    	}
    
    }


    cout<<"\n\n";
    FloydWarshall(UpdateMat,0.001);
    cout<<"\n\n";exit(-1);
 
    return 0;
}
