// A C / C++ program for Bellman-Ford's single source shortest path algorithm.
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>
#include <iostream>
 
using namespace std; 
// a structure to represent a weighted edge in graph
struct Edge
{
    int src, dest, weight;
};
 
// a structure to represent a connected, directed and weighted graph
struct Graph
{
    // V-> Number of vertices, E-> Number of edges
    int V, E;
 
    // graph is represented as an array of edges.
    struct Edge* edge;
};
 
// Creates a graph with V vertices and E edges
struct Graph* createGraph(int V, int E)
{
    struct Graph* graph = (struct Graph*) malloc( sizeof(struct Graph) );
    graph->V = V;
    graph->E = E;
 
    graph->edge = (struct Edge*) malloc( graph->E * sizeof( struct Edge ) );
 
    return graph;
}
 
// A utility function used to print the solution
void printArr(int dist[], int n)
{
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < n; ++i)
        printf("%d \t\t %d\n", i, dist[i]);
}
 
// The main function that finds shortest distances from src to all other
// vertices using Bellman-Ford algorithm.  The function also detects negative
// weight cycle
void BellmanFord(struct Graph* graph, int src)
{
    int V = graph->V;
    int E = graph->E;
    int dist[V];
 
    // Step 1: Initialize distances from src to all other vertices as INFINITE
    for (int i = 0; i < V; i++)
    {
        dist[i]   = 100*100;// INT_MAX-100;
        cout<<"\n\t 0. dist[i]: %d "<<dist[i];
    }
    dist[src] = 0;
 
    // Step 2: Relax all edges |V| - 1 times. A simple shortest path from src
    // to any other vertex can have at-most |V| - 1 edges
    for (int i = 1; i <= V-1; i++)
    {
    	cout<<"\n\t dist[i]: %d "<<dist[i];
        for (int j = 0; j < E; j++)
        {
            int u = graph->edge[j].src;
            int v = graph->edge[j].dest;
            int weight = graph->edge[j].weight;
            //printf("\n\t 1. Distance-- u: %d d[u]: %d v: %d d[v]: %d weight: %d ",u,dist[u],v,dist[v],weight);
            if (dist[u] + weight < dist[v])
                {
                	dist[v] = dist[u] + weight;
                	// printf("\n\t 2. Distance-- u: %d d[u]: %d v: %d d[v]: %d weight: %d ",u,dist[u],v,dist[v],weight);
                } 
                
        }
    }
 
    // Step 3: check for negative-weight cycles.  The above step guarantees
    // shortest distances if graph doesn't contain negative weight cycle.  
    // If we get a shorter path, then there is a cycle.
    for (int i = 0; i < E; i++)
    {
        int u = graph->edge[i].src;
        int v = graph->edge[i].dest;
        int weight = graph->edge[i].weight;
        if (dist[u] + weight < dist[v])
            printf("Graph contains negative weight cycle");
    }
 	printf("\n\n");
    printArr(dist, V);
 
    return; 
}
 
// Driver program to test above functions
int main()
{
    /* Let us create the graph given in above example */
    int V = 5;  // Number of vertices in graph
    int E = 8;  // Number of edges in graph
    struct Graph* graph = createGraph(V, E);
    
    int NumVertices=6;int PerVertexEdges=3;
    int NumEdges=( PerVertexEdges * NumVertices );
    struct Graph* graphFlexi=createGraph(NumVertices,NumEdges);
    
    struct timeval time;
    gettimeofday(&time,NULL);
    double t1= (time.tv_sec)+(time.tv_usec/1000000.0);
    srand(t1);
    
    int i=0;int currVertex=0; int temp=0;
    for(i=0;i<NumEdges;i++)
    {
    	graphFlexi->edge[i].src = currVertex; 
    	temp = ( ( (rand() % NumVertices ) + (rand() %NumVertices ) +1 ) %NumVertices);
    	while( temp == currVertex) 
    		temp= ( ( (rand() % NumVertices ) + (rand() %NumVertices ) +1 ) %NumVertices);
    		
    	graphFlexi->edge[i].dest = temp;
    	graphFlexi->edge[i].weight = ( rand() % NumVertices );
    
    	if((i+1)%3==0)
    		currVertex+=1;
    	
    }
    
    for(i=0;i<NumEdges;i++)
    	printf("\n\t Edge: src: %d destination: %d weight: %d ",graphFlexi->edge[i].src,graphFlexi->edge[i].dest,graphFlexi->edge[i].weight);
    printf("\n\n");
 
    // add edge 0-1 (or A-B in above figure)
    graph->edge[0].src = 0;
    graph->edge[0].dest = 1;
    graph->edge[0].weight = -1;
 
    // add edge 0-2 (or A-C in above figure)
    graph->edge[1].src = 0;
    graph->edge[1].dest = 2;
    graph->edge[1].weight = 4;
 
    // add edge 1-2 (or B-C in above figure)
    graph->edge[2].src = 1;
    graph->edge[2].dest = 2;
    graph->edge[2].weight = 3;
 
    // add edge 1-3 (or B-D in above figure)
    graph->edge[3].src = 1;
    graph->edge[3].dest = 3;
    graph->edge[3].weight = 2;
 
    // add edge 1-4 (or A-E in above figure)
    graph->edge[4].src = 1;
    graph->edge[4].dest = 4;
    graph->edge[4].weight = 2;
 
    // add edge 3-2 (or D-C in above figure)
    graph->edge[5].src = 3;
    graph->edge[5].dest = 2;
    graph->edge[5].weight = 5;
 
    // add edge 3-1 (or D-B in above figure)
    graph->edge[6].src = 3;
    graph->edge[6].dest = 1;
    graph->edge[6].weight = 1;
 
    // add edge 4-3 (or E-D in above figure)
    graph->edge[7].src = 4;
    graph->edge[7].dest = 3;
    graph->edge[7].weight = -3;
 
    BellmanFord(graph, 0);
   // printf("\n\n\n");     BellmanFord(graphFlexi, 0);
 
    return 0;
}

