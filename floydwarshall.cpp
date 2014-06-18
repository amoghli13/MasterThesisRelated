#include <iostream>
#include <set>
#include <vector>
#include<cstdio>
#include<cstdlib>
#include "error_inject_operator.h"
using namespace std;
 
typedef vector<int> vi;
typedef pair<int,error_inject_operators<int> > ii;
typedef vector<ii> vii;
typedef vector<vii> vvii;
 
const int MAX = 1001;
const int MAXINT = 1000000000;

#define INTMAX 10000 
 
template <class M>
double  error_inject_operators<M>::error_percent=0.0;
int n;

void FloydWarshall(error_inject_operators<int>** UpdateMat,double ErrorPercent)
{
	error_inject_operators<int>::error_percent=ErrorPercent;
	
    for(int i=0;i<n;i++)
    {
        
		for(int j=0;j<n;j++)
		{
			//cout<<"\t "<<(UpdateMat[i][j]); 
			for(int k=0;k<j;k++)
				if( (UpdateMat[i][j]) > (UpdateMat[i][k]+UpdateMat[k][j]) )
				{
					cout<<"\n\t i "<<i<<" j "<<j<<" k "<<k<<" LHS: "<<(UpdateMat[i][j])<<" is greater than RHS: "<<(UpdateMat[i][k])<<" , "<<(UpdateMat[k][j]);
					UpdateMat[i][j]= (UpdateMat[i][k]+UpdateMat[k][j]); 
				}				
				
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
        printf("\n %d %d %d %d\n", a, b, w,d);
 
		if(d)		      
			UpdateMat[a][b]=w;
		else
			UpdateMat[b][a]=w;
      
    }
    
    for(int i=0;i<n;i++)
    {
        cout<<"\n";
		for(int j=0;j<n;j++)
			cout<<"\t "<<(UpdateMat[i][j]); 
	}
    cout<<"\n\n";
    FloydWarshall(UpdateMat,0.0);
    cout<<"\n\n";exit(-1);
 
    return 0;
}
