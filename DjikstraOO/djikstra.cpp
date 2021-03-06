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

 
template <class M>
double  error_inject_operators<M>::error_percent=0.0;

 
int n;
vvii G(MAX);
vi D(MAX, MAXINT);

void sortMyNode(int v)
{
    int sz=G[v].size();
   for( int i=0;i<sz;i++ )
    {
	 //   cout<<"\n\t Node: "<<G[v][i].first<<" edge: "<<G[v][i].second;
	for(int j=i+1;j<sz;j++)
    	{
	    //cout<<"\n\t\t -- Node: "<<G[v][j].first<<" edge: "<<G[v][j].second;
	      	if(G[v][j].second < G[v][i].second )
	    	{
	    		ii tmp=G[v][j];
	    		G[v][j]=G[v][i];
	    		G[v][i]=tmp;
	      	}
   	}
    }
    /* cout<<"\n\t Size: "<<sz<<endl;
    for(int i= 0;i<sz; i++)
    {
    	 cout<<"\n\t Node: "<<G[v][i].first<<" edge "<<G[v][i].second;
    }  */  
}
 
 inline error_inject_operators<int> tmr_lesser( error_inject_operators<int> a1,int a2)
 {
 	error_inject_operators<int> res1,res2,res3;
 	res1= a1 < a2;
 	res2= a1 < a2;
 	res3= a1 < a2;
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
	 	cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" are not equal and hence, exitting! \n ";
	 	exit(-1);
 	}
 	
 	cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" result "<<result;
 	return result;
 }
 
 
void Dijkstra(int s,int destination)
{
    
    vector<int> accepted_nodes;
    vector<int> accepted_nodes_ptr(MAX);    
    vector<int> accepted_nodes_hash(MAX);        
    D[s]=0;
    accepted_nodes.push_back(s);
    accepted_nodes_ptr[s]=0; // first-> the pointer/index position in above 
    accepted_nodes_hash[s]=1;
    int v=(*accepted_nodes.begin());
    sortMyNode(v);
    int destination_notfound=1;
    while(destination_notfound)
    {
    	int accepted_nodes_size=accepted_nodes.size();
	int reqd_idx_in_accepted_nodes=0;
		// cout<<"\n\t Current size of accepted_size: "<<accepted_nodes_size;
	    for(int i=1;i<accepted_nodes_size;i++)
	    {
	    	if( accepted_nodes_ptr[i] )
	    	if(  G[accepted_nodes[i]][accepted_nodes_ptr[i]].second < G[accepted_nodes[i]][accepted_nodes_ptr[i]-1].second  )
	    	{
	    		error_inject_operators<int> tmp;
	    		tmp=G[accepted_nodes[i]][accepted_nodes_ptr[i]].second;
	    		G[accepted_nodes[i]][accepted_nodes_ptr[i]].second=G[accepted_nodes[i]][accepted_nodes_ptr[i]-1].second;
	    		G[accepted_nodes[i]][accepted_nodes_ptr[i]-1].second=tmp;
	    	
	    	}
	        if( G[accepted_nodes[i]].size() > accepted_nodes_ptr[i] )
	    	if( G[accepted_nodes[i]][accepted_nodes_ptr[i]].second < G[reqd_idx_in_accepted_nodes][accepted_nodes_ptr[reqd_idx_in_accepted_nodes]].second )
	    	{
	    		reqd_idx_in_accepted_nodes=i;
	    		cout<<"\n\t"<<G[accepted_nodes[i]][accepted_nodes_ptr[i]].second << " is lesser than "<<G[accepted_nodes[i-1]][accepted_nodes_ptr[i-1]].second ;
    	    		cout<<"\n\t v: "<<accepted_nodes[i]<<"\t v2: "<<G[accepted_nodes[i]][accepted_nodes_ptr[i]].first;
	    		// exit(-1);
	    	}
	    }

    	    		cout<<"\n\t v: "<<accepted_nodes[reqd_idx_in_accepted_nodes]<<"\t v2: "<<G[accepted_nodes[reqd_idx_in_accepted_nodes]][accepted_nodes_ptr[reqd_idx_in_accepted_nodes]].first;
	    	int v=accepted_nodes[reqd_idx_in_accepted_nodes];
		ii tmp_reqd_ptr=G[v][accepted_nodes_ptr[reqd_idx_in_accepted_nodes]];
		
		if( accepted_nodes_ptr[reqd_idx_in_accepted_nodes] )
		{
			error_inject_operators<int> duh;
			duh=G[v][accepted_nodes_ptr[reqd_idx_in_accepted_nodes]-1].second;
			if( tmp_reqd_ptr.second < duh )
			{
				//trace_fault( v, accepted_nodes,accepted_nodes_ptr,reqd_idx_in_accepted_nodes );
				cout<<"\n\t FATAL edge with weight "<<tmp_reqd_ptr.second<<" was decided to be smaller than "<<duh;
			} 
		}

	    	int v2=tmp_reqd_ptr.first;
	    	error_inject_operators<int> cost;
	    	cost=tmp_reqd_ptr.second;
	    	cout<<"\n\t Considering accepted node: "<<v<<" and it's current viable node is: "<<v2<<" and the edge b/n them has a weight of "<<cost;
	    	error_inject_operators<int> cost_compare;
	    	cost_compare=cost+D[v];
	   
	    	//if( (D[v2] > cost_compare ) 
	    	if( tmr_lesser( cost_compare,D[v2])==1 )
	    	{
	    	
		    	if(accepted_nodes_hash[v2]!=1)
		    	{
		    		accepted_nodes_hash[v2]=1;
			    	cout<<"\n\t Viable node: "<<v2<<" is accepted! since D[v2]: "<< D[v2] <<" is greater than "<<cost_compare; //and the edge b/n them has a weight of "<<cost;
			    	sortMyNode(v2); cout<<endl;			    				    	
			    	accepted_nodes.push_back(v2);
			    	accepted_nodes_ptr.push_back(0);
			    	//cout<<"\n\t  V2: "<<v2<< " Accepted_nodes_hash[v2] "<<accepted_nodes_hash[v2];
			    	//exit(-1)			    	;
  			}
  			//else
  			accepted_nodes_ptr[reqd_idx_in_accepted_nodes]+=1;
		    	D[v2]=cost_compare.operand;
			//cout<<"\n Illi Saddu maadu accepted_nodes_ptr[reqd_idx_in_accepted_nodes] "<<accepted_nodes_ptr[reqd_idx_in_accepted_nodes]<<" and reqd_idx_in_accepted_nodes is "<<reqd_idx_in_accepted_nodes;
		}
		else if (D[v2]==0)//(accepted_nodes_hash[v2]==1 )
		{
			cout<<"\n\t Viable node: "<<v2<<" is accepted! since D[v2]: "<< D[v2] <<" is NOT greater than "<<cost_compare; //and the edge b/n them has a weight of "<<cost;
			accepted_nodes_ptr[reqd_idx_in_accepted_nodes]+=1;
			//exit(-1);
			//cout<<"\n\t Alli saddu maadu! accepted_nodes_ptr[reqd_idx_in_accepted_nodes] "<<accepted_nodes_ptr[reqd_idx_in_accepted_nodes]<<" and reqd_idx_in_accepted_nodes is "<<reqd_idx_in_accepted_nodes;
		}
		else
			cout<<"\n\t Viable node: "<<v2<<" D[v2]: "<< D[v2] <<" "<<cost_compare; //and the edge b/n them has a weight of "<<cost;
		
		
    	if( v2==destination) 
    	destination_notfound=0;
    	cout<<"\n\t Yay! destination_notfound is "<<destination_notfound;
    
    
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
    for (int i = 0; i < m; i++)
    {
        int a, b, w = 0;
        //scanf("%d %d %d", &a, &b, &w);
        fscanf( ip_file_handle,"%d %d %d", &a, &b, &w);
        printf("\n %d %d %d\n", a, b, w);
        error_inject_operators<int> tmp;
        tmp=w;
        G[a - 1].push_back(ii(b - 1, tmp));
        G[b - 1].push_back(ii(a - 1, tmp));
    }
    //cout<<endl;exit(-1);
    Dijkstra(s - 1,t-1);
 
    printf("\n\n\t Result: %d\n", D[t - 1]);
 
    return 0;
}
