#include <iostream>
#include <set>
#include <vector>
#include<cstdio>
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
 
void Dijkstra(int s)
{
    set<ii> Q;
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
        
    while(!Q_2.empty())
    {
        ii top = *Q_2.begin();
        Q_2.erase(Q_2.begin());
        int v = top.second;
        int d = top.first;
        
//        cout<<"\n\t ++ Top-node: "<<top.second<<" edge-weight: "<<top.first;
 
        for (vii::const_iterator it = G[v].begin(); it != G[v].end(); it++)
        {
            int v2 = it->first;
            int cost = it->second;
//	    cout<<"\n\t ++ Considering node: "<<it->first<<" edge-weight: "<<it->second;        
        }
    }
        
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
 
    Dijkstra(s - 1);
 
    printf("\n\n\t Result: %d\n", D[t - 1]);
 
    return 0;
}
