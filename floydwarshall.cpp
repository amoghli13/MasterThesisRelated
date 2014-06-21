#include "floydwarshall.h"

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
    
    
    error_inject_operators<bool> Compare;
    error_inject_operators<bool> Compare1;
    error_inject_operators<bool> Compare2;
    error_inject_operators<bool> NeedToRepeat;
    error_inject_operators<bool> NeedToRepeat1;
    
    int RowbySetSize;
    int ColbySetSize;		
    
    	
    for(int i=0;i<NumRows;i++)
    {
     	cout<<"\n i "<<i    ;
	RowbySetSize=(int) (i/SetSize);
    	for(int j=0;j<NumCols;j++)
	{
			//cout<<"\t "<<(UpdateMat[i][j]); 
			//cout<<"\n\t i "<<i<<" j "<<j;
		ColbySetSize=(int) (j/SetSize);
    		temp=new error_inject_operators<int> [j];
		NeedToRepeat1=true;
		// cout<<"\n\t -- (i) "<<(i)<<" j "<<(j)<<" UpdateMat[i][j] "<<UpdateMat[i][j]<<" NeedToRepeat1: "<<NeedToRepeat1<<endl;
    		
    		while(NeedToRepeat1.operand)
    		{
	    		NeedToRepeat=true;
	    		
	    		while(NeedToRepeat.operand)
	    		{
	    		
	    			for(int k=0;k<j;k++)
	    			{
	    				temp[k]=(UpdateMat[i][k]+UpdateMat[k][j]);
	    				cout<<"\n\t k: "<<k<<" temp[k] "<<temp[k]<<" UpdateMat[i][k] "<<(UpdateMat[i][k])<<" UpdateMat[k][j] "<<(UpdateMat[k][j]);
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
	    			
	    			Compare= tmr_equal(diff,diff1);
	    			if( Compare.operand )
	    				NeedToRepeat=false;
	    			else
	    			{
	    				NeedToRepeat=true;    		
	    				cout<<"\n\t i "<<i<<" j "<<j;
	    				cout<<"\n\t Counter1: "<<Counter1<<" Counter2: "<<Counter2;
	    				cout<<"\n\t c1+c2 != CheckpointLength "<<(diff)<<" CheckpointLength: "<<(diff1)<<"\n";
	    			}

	    		}
	    		//cout<<"\n\t -- i "<<i<<" j "<<j<<" MinK "<<MinK<<" UpdateMat[i][j] "<<UpdateMat[i][j];
	    		NeedToRepeat=true;
	    		while(NeedToRepeat.operand)
	    		{
	    		
	    			for(int k=0;k<j;k++)
	    			{
	    				temp[k]=(UpdateMat[i][k]+UpdateMat[k][j]);
	    				cout<<"\n\t k: "<<k<<" temp[k] "<<temp[k]<<" UpdateMat[i][k] "<<(UpdateMat[i][k])<<" UpdateMat[k][j] "<<(UpdateMat[k][j]);
	    			}
	    			
	    			Counter1=0; Counter2=j;MaxK=0;
	    			
	    			for(int k=0;k<j;k++)
	    			{
	    				//cout<<"\n\t k "<<k<<" temp[k] "<<(temp[k])<<" MaxK "<<MaxK;
	    				diff=( MaxK - temp[k]);
	    				if( temp[k] > MaxK )
	    				{
				    		MaxK=temp[k];
				    		Counter1=Counter1+1;
	    				}	
	    			
	    				if((diff < 0) ) //|| (temp[k] < MinK))
	    				{
	    					Counter2=Counter2-1;				
	    				}
	    		
	    			}
	    			diff= Counter1 + Counter2;
	    			diff1=j;
	    			Compare= tmr_equal(diff,diff1);
	    			if( Compare.operand )
	    				NeedToRepeat=false;
	    			else
	    			{
	    				NeedToRepeat=true;			
	    				cout<<"\n\t i "<<i<<" j "<<j;
	    				cout<<"\n\t MaxCounter1: "<<Counter1<<" MaxCounter2: "<<Counter2;
	    				cout<<"\n\t c1+c2 != CheckpointLength "<<(diff)<<" CheckpointLength: "<<(diff1)<<"\n";
	    				//exit(-1);
	    			}

	    		}    		
	    		//cout<<"\n\t -- i "<<i<<" j "<<j<<" MaxK "<<MaxK<<" UpdateMat[i][j] "<<UpdateMat[i][j];

			//MinRowCol=(MaxMinofMatRows.MinMat[RowbySetSize][j]+MaxMinofMatCols.MinMat[i][ColbySetSize]);
			MaxRowCol=(MaxMinofMatRows.MaxMat[RowbySetSize][j]+MaxMinofMatCols.MaxMat[i][ColbySetSize]);
			
 			Compare1= tmr_greater_equal_bool(MaxRowCol,MaxK);
		
			if( Compare1.operand )
			{
				
				NeedToRepeat1=false;
				//cout<<"\n\t F: Compare.operand "<<(Compare.operand)<<" Compare1.operand "<<Compare1.operand;
			}
			else
			{
				NeedToRepeat1=true;
				cout<<"\n\t MaxRowCol: "<<(MaxRowCol)<<" MaxK: "<<MaxK;
				cout<<"\n\t MaxMinofMatRows.MaxMat[RowbySetSize][j]: "<<(MaxMinofMatRows.MaxMat[RowbySetSize][j])<<" MaxMinofMatCols.MaxMat[i][ColbySetSize] "<<(MaxMinofMatCols.MaxMat[i][ColbySetSize]);
				cout<<"\n\t T: Compare.operand "<<(Compare.operand)<<" Compare1.operand "<<Compare1.operand;
				//exit(-1);
			}

			
			cout<<"\n\t -- i "<<i<<" j "<<j<<" MinK "<<MinK<<" MaxK "<<MaxK<<" UpdateMat[i][j] "<<UpdateMat[i][j];
			
	    		diff=tmr_lesser(MinK,UpdateMat[i][j]);
    			UpdateMat[i][j]= diff;
	    		
	    		//cout<<"\n\t -- i "<<i<<" j "<<j<<" MinK "<<MinK<<" diff "<<diff<<" UpdateMat[i][j] "<<UpdateMat[i][j];// <<endl;
		}
		//temp= delete [] error_inject_operators<int>;   			
		delete[] temp;
    	}
    }

}


int main(int argc, char* argv[])
{

   double ErrorPercent=0.0;
   if(argc < 3)
   {
   	cout<<"\n\t ERROR: Required input arguments set-size(int) and error-percent(double) \n\n";
   	exit(-1);
   }

    SetSize=atoi(argv[1]);
    ErrorPercent=atof(argv[2]);
    	
    int m, s, t = 0;
    //printf("\n\t Enter: \n\t\t 1. number of nodes \n\t\t 2. Number of edges \n\t\t 3. Source node \n\t\t 4. Destination node \n\t\t ");
    //scanf("%d %d %d %d", &n, &m, &s, &t);
		
   FILE* ip_file_handle;
   string ip_file="inputs.txt";
   ip_file_handle=fopen(ip_file.c_str(),"r");

    fscanf(ip_file_handle," %d %d %d %d",&n,&m,&s,&t);
    printf("\n\t N: %d M: %d S: %d T: %d ",n,m,s,t);

	error_inject_operators<int>::error_percent=0.0; 
	
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


   // PrintMat(UpdateMat);	
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

    FindMaxMin(UpdateMat,MaxMinofMatRows,MaxMinofMatCols);
    
 /*   for(int i=0;i<NumRowsbySetSize;i++)
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
*/

    cout<<"\n\n";
    FloydWarshall(UpdateMatPristine,0.00,MaxMinofMatRows,MaxMinofMatCols);
    FloydWarshall(UpdateMat,ErrorPercent,MaxMinofMatRows,MaxMinofMatCols);
   // PrintMat(UpdateMat);	
    CompareMat(UpdateMat,UpdateMatPristine,0.0);
    cout<<"\n\n";
 
    return 0;
}
