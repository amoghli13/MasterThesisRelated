#include <iostream>
#include <set>
#include <vector>
#include<cstdio>
#include<cstdlib>
using namespace std;
 
typedef vector<int> vi;
typedef pair<int,int> ii;
typedef vector<ii> vii;
typedef vector<vii> vvii;
 
const int MAX = 1001;
const int MAXINT = 1000000000;
 
int n;
vvii G(MAX);
vi D(MAX, MAXINT);

void sortMyNode(int v)
{
    int sz=G[v].size();
   for( int i=0;i<sz;i++ )
    {
	    cout<<"\n\t Node: "<<G[v][i].first<<" edge: "<<G[v][i].second;
	for(int j=i+1;j<sz;j++)
    	{
	    cout<<"\n\t\t -- Node: "<<G[v][j].first<<" edge: "<<G[v][j].second;
	      	if(G[v][j].second < G[v][i].second )
	    	{
	    		ii tmp=G[v][j];
	    		G[v][j]=G[v][i];
	    		G[v][i]=tmp;
	      	}
   	}
    }
    cout<<"\n\t Size: "<<sz<<endl;
    for(int i= 0;i<sz; i++)
    {
    	 cout<<"\n\t Node: "<<G[v][i].first<<" edge "<<G[v][i].second;
    }    
}
 
void Dijkstra(int s,int destination)
{
    /*set<ii> Q;
    D[s] = 0;
    Q.insert(ii(0,s));
 
 
    while(!Q.empty())
    {
        ii top = *Q.begin();
        Q.erase(Q.begin());
        int v = top.second;
        int d = top.first;
	set<ii>  Q_2;
        cout<<"\n\t Top-node: "<<top.second<<" edge-weight: "<<top.first;
 
        for (vii::const_iterator it = G[v].begin(); it != G[v].end(); it++)
        {
            int v2 = it->first;
            int cost = it->second;
	    cout<<"\n\t Considering node: "<<it->first<<" edge-weight: "<<it->second<<" \t v2: "<<v2<<" D[v2] "<<D[v2];                
            if (D[v2] > D[v] + cost)
            {
                if (D[v2] != 1000000000)
                {
                    Q.erase(Q.find(ii(D[v2], v2)));
                    cout<<"\n\t Erasing node: "<<it->first<<" edge-weight: "<<it->second<<"\t v2: "<<v2<<" D[v2] "<<D[v2];    
                }
                D[v2] = D[v] + cost;
                Q.insert(ii(D[v2], v2));
                Q_2.insert(ii(D[v2], v2));                
                cout<<"\n\t ** Inserting node: "<<it->first<<" edge-weight: "<<it->second<<"\t v2: "<<v2<<" D[v2] "<<D[v2];                  
            }
        }
    } */
    
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
		cout<<"\n\t Current size of accepted_size: "<<accepted_nodes_size;
	    for(int i=1;i<accepted_nodes_size;i++)
	    {
	        if( G[accepted_nodes[i]].size() > accepted_nodes_ptr[i] )
	    	if( G[accepted_nodes[i]][accepted_nodes_ptr[i]].second < G[reqd_idx_in_accepted_nodes][accepted_nodes_ptr[reqd_idx_in_accepted_nodes]].second )
	    	{
	    		reqd_idx_in_accepted_nodes=i;
	    		cout<<"\n\t"<<G[accepted_nodes[i]][accepted_nodes_ptr[i]].second << " is lesser than "<<G[accepted_nodes[i-1]][accepted_nodes_ptr[i-1]].second ;
	    		// exit(-1);
	    	}
	    	
	    }
	    	int v=accepted_nodes[reqd_idx_in_accepted_nodes];
		ii tmp_reqd_ptr=G[v][accepted_nodes_ptr[reqd_idx_in_accepted_nodes]];
	    	int v2=tmp_reqd_ptr.first;
	    	int cost=tmp_reqd_ptr.second;
	    	cout<<"\n\t Considering accepted node: "<<v<<" and it's current viable node is: "<<v2<<" and the edge b/n them has a weight of "<<cost;
	    	int cost_compare=D[v]+cost;
	    	if( D[v2] > cost_compare )
	    	{
	    	
		    	if(accepted_nodes_hash[v2]!=1)
		    	{
		    		accepted_nodes_hash[v2]=1;
			    	cout<<"\n\t Viable node: "<<v2<<" is accepted! since D[v2]: "<< D[v2] <<" is greater than "<<cost_compare; //and the edge b/n them has a weight of "<<cost;
			    	sortMyNode(v2); cout<<endl;			    				    	
			    	accepted_nodes.push_back(v2);
			    	accepted_nodes_ptr.push_back(0);
			    	cout<<"\n\t  V2: "<<v2<< " Accepted_nodes_hash[v2] "<<accepted_nodes_hash[v2];
			    	//exit(-1)			    	;
  			}
  			//else
  			accepted_nodes_ptr[reqd_idx_in_accepted_nodes]+=1;
		    	D[v2]=cost_compare;
			cout<<"\n Illi Saddu maadu accepted_nodes_ptr[reqd_idx_in_accepted_nodes] "<<accepted_nodes_ptr[reqd_idx_in_accepted_nodes]<<" and reqd_idx_in_accepted_nodes is "<<reqd_idx_in_accepted_nodes;
		}
		else if ( accepted_nodes_hash[v2]==1 )
		{
			accepted_nodes_ptr[reqd_idx_in_accepted_nodes]+=1;
			cout<<"\n\t Alli saddu maadu! accepted_nodes_ptr[reqd_idx_in_accepted_nodes] "<<accepted_nodes_ptr[reqd_idx_in_accepted_nodes]<<" and reqd_idx_in_accepted_nodes is "<<reqd_idx_in_accepted_nodes;
		}
		else
			cout<<"\n\t Alli illa illu illa saddu maadu! \n";		
		
		
    	if( v2==destination) 
    	destination_notfound=0;
    	cout<<"\n\t Yay! destination_notfound is "<<destination_notfound;
    
    
    }
    
}
 
int main()
{
    int m, s, t = 0;
    printf("\n\t Enter: \n\t\t 1. number of nodes \n\t\t 2. Number of edges \n\t\t 3. Source node \n\t\t 4. Destination node \n\t\t ");
    scanf("%d %d %d %d", &n, &m, &s, &t);
 
    for (int i = 0; i < m; i++)
    {
        int a, b, w = 0;
        scanf("%d %d %d", &a, &b, &w);
        G[a - 1].push_back(ii(b - 1, w));
        G[b - 1].push_back(ii(a - 1, w));
    }
 
    Dijkstra(s - 1,t-1);
 
    printf("\n\n\t Result: %d\n", D[t - 1]);
 
    return 0;
}
