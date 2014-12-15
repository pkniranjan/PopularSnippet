// simple2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <process.h>
#include "stdafx.h"
#include <vector>
#include <Windows.h>
#include <tchar.h>
#define MAX 20
using namespace std;

struct edge_type;
struct vertex_type
{
	edge_type* firstedge;
	vertex_type* nextvertex;
	int vertexnum;
};

 struct edge_type
{
	vertex_type* endpoint;
	edge_type* next;
	int wt;
};

class MyGraph
{
	int n; //number rof vertices
	edge_type* firstedge[MAX];	//direct pointer to each vertex
public:
	MyGraph()
	{
		for(int i=0;i<MAX;i++)
		{
			firstedge[i] = NULL;
		}
		n=0;
	}
	void AddVertex(int vertex);
	void ConnectVertex(int vertex,int targetvertex,int edgewt=0);
	void DFS();
	void Visit(int Vertex,bool* bVisitedArray);
	void BFS();
	void TopoSortDFS();
	void Sort(int v, int& place,bool* bVisited, int* arr);
};
void MyGraph::AddVertex(int vertexnum)
{	//vertex node will be duplicated, edges will connect 
	if(firstedge[vertexnum] != NULL)
		return;

		vertex_type* tV = new vertex_type();
		edge_type* tE	= new edge_type();
		
		tE->endpoint	= tV;
		tE->wt			= 0;
		tE->next		= NULL;
		firstedge[vertexnum] = tE;


		tV->vertexnum = vertexnum;
		tV->firstedge = tE;
		tV->nextvertex = NULL;

		n++;
}
void MyGraph::ConnectVertex(int vertex,int targetvertex,int edgewt)
{
	if(firstedge[vertex] == NULL || firstedge[targetvertex] == NULL) // connecting vertex doesn't exist
		return;

	edge_type* tE = firstedge[vertex];

	while(tE->next!=NULL)
	{
		tE = tE->next;
	}
	edge_type* tempE = new edge_type();
	tempE->next = NULL;
	tempE->wt = edgewt;
	tempE->endpoint = firstedge[targetvertex]->endpoint;
	tE->next = tempE;
}

void MyGraph::DFS()
{
	bool* bVisited = new bool[MAX];
	for(int i=0;i<MAX;i++)
		bVisited[i] = false;

	for(int v=0;v<MAX;v++)
	{
		if(bVisited[v] == false)
		{
			Visit(v,bVisited);
		}
	}
}
void MyGraph::Visit(int Vertex, bool* bVisitedArray)
{
	if(Vertex >= n)
		return;
	bVisitedArray[Vertex] = true;
	cout<<firstedge[Vertex]->endpoint->vertexnum<<"\t";
	edge_type* e = firstedge[Vertex];
	
	e= e->next;

	while(e != NULL)
	{
		if(bVisitedArray[e->endpoint->vertexnum] == false)
		{
			Visit(e->endpoint->vertexnum, bVisitedArray);
		}
		e = e->next;
	}
}
void MyGraph::BFS()
{

	vector<int> Q;

	bool* bVisited = new bool[MAX];
	for(int i=0;i<MAX;i++)
		bVisited[i] = false;

	for(int v=0;v<n;v++)
	{
		if(bVisited[v] == false)
		{
			Q.push_back(firstedge[v]->endpoint->vertexnum);
			while(!Q.empty())
			{
				int CurrVertex = Q.back();
				Q.pop_back();

				bVisited[v] = true;
				cout<<CurrVertex<<"\t";

				edge_type* e = firstedge[v];
				e= e->next;
				while(e != NULL)
				{
					if(bVisited[e->endpoint->vertexnum] == false)
					{
						Q.push_back(e->endpoint->vertexnum);
						bVisited[e->endpoint->vertexnum] = true;
					}
					e = e->next;
				}
			}
		}
	}
}
void MyGraph::TopoSortDFS()
{

	bool* bVisited = new bool[MAX];
	int* SortedGraph = new int[n];

	for(int i=0;i<MAX;i++)
		bVisited[i] = false;

	int place;
	place= n; //last member of G

	for(int v=0;v<n;v++)
	{
		if(bVisited[v] == false)
		{
			Sort(v,place,bVisited,SortedGraph);
		}
	}
	for(int v=0;v<n;v++)
	{
		cout<<SortedGraph[v]<<"\t";
	}
}
void MyGraph::Sort(int v, int& place,bool* bVisited, int* arr)
{
	vertex_type* w; //one vertex adjacent to v;
	edge_type* p;
	bVisited[v] = true;
	p= firstedge[v];
	p = p->next;

	while(p)
	{
		w = p->endpoint;
		if(bVisited[w->vertexnum] == false)
			Sort(w->vertexnum, place,bVisited,arr);
		p=p->next;
	}
	arr[place] = v;
	place --;
}
/***************************************************/
void ShortestPath(Graph* G)
{
	final[MAX];
	AT;
	
	final[0] = true;
	for(v=1;v<n;v++)
		D[v] = AT[0][v];

	for(all v in G)
	{
		min = INFINITY;
		for(all w adjancent to v)
		{
			if(final[w] == false && D[w]<min)
			{
				v=w;
				min= D[w];
			}
		}
		final[v] = true;
	}
	for(int all w in G except first)
	{
		if(final[w]==false && (min+cost[v][j] < D[j]))
			D[j] = min+cost[v][j];
	}
}
/***************************************************/
primMST(graph *g, int start)
{
	int i; //counter 
	edgenode *p;		// temporary pointer
	bool intree[MAXV];	// is the vertex in the tree yet
	int distance[MAXV]; // distance vertex is from start
	int v;				// current vertex to process
	int w;				// candidate next vertex
	int weight;			// edge weight
	int dist;			//best current distance from start


	//for each vertex in graph
 //  set min_distance of vertex to 8
 //  set parent of vertex to null
 //  set minimum_adjacency_list of vertex to empty list

   //set is_in_Q of vertex to true
	//set min_distance of initial vertex to zero
	//add to minimum-heap Q all vertices in graph, keyed by min_distance

	for (i=1; i<= G->nvertices; i++)
	{
		intree[i] = FALSE;
		distance[i] = MAXINT;
		parent[i] = -1;
	}

	distance[start] = 0;
	v = start;

	//this loop initializes yhe distance vector and sets parent and wieght from parent = distance
	//disconnected nodes are left out
	while (intree[v] == FALSE) 
	{
		intree[v] = TRUE;
		p = g->edges[v];
		while (p ! = NULL) //this loop is like BFS for a level. it find out nearest node w to v
		{
			w = p->EndPoint;
			weight = p->weight;
			if ((distance[w] > weight) && (intree[w] == FALSE))
			{
				distance[w] = weight;
				parent[w] = v;
			}
			p = p->next;
		}
		v = 1;
		dist = MAXINT;
		for (i=1; i<= g->nvertices; i++)
		{
			if ((intree[i] == FALSE) && (dist > distance[i])) 
			{
				dist = distance[i];
				v = i;
			}
		}
	}
}