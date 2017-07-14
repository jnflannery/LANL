#ifndef MAYBE_GRAPH_CPP
#define MAYBE_GRAPH_CPP

#include "Graph.cpp"
#include "maybegraph.h"
#include "molecule.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <set>
using namespace std;


//MaybeVertex
struct MaybeVertex
{
	int id;
	vector<int> neighbors;
	vector<int> maybeNeighbors;

	Vertex MaybeToVertex(){
		Vertex v = {id, neighbors};
		return v;
	}

	bool operator==(const MaybeVertex& a) const
	{
		set<int> n1(neighbors.begin(), neighbors.end());
		set<int> n2(a.neighbors.begin(), a.neighbors.end());
		if (n1.size() != n2.size()) return false;
		return (n1 == n2);
	}
	bool operator<(const Vertex& a) const
	{
		return (id < a.id);
	}
};


//Class Graph
class MaybeGraph : public Graph
{
private:
	int V;
	vector<MaybeVertex> vertices;
public:
	MaybeGraph(int V)
	{
		this->V = V;
		for (int i = 0; i < V + 1; ++i) {
			MaybeVertex v;
			v.id = i;
			this->vertices.push_back(v);
		}
	}
	MaybeGraph()
	{
		this->V = 0;
	}

	//Get number of vertices
	int GetNumberOfVertices() {
		return V;
	}
	//get all vertices
	vector<MaybeVertex> getAllVertices(){
		return vertices;
	}
	//Add MaybeEdge to Graph
	void addMaybeEdge(MaybeVertex a, MaybeVertex b)
	{
		a.maybeNeighbors.push_back(b.id);
		b.maybeNeighbors.push_back(a.id);
	}
	void addMaybeEdge(int a, int b)
	{
		vertices[a].maybeNeighbors.push_back(b);
		vertices[b].maybeNeighbors.push_back(a);
	}

	//Print the graph
	void printGraph()
	{ 
		int v;
		for (v = 1 ; v <= V; ++v)
		{
			cout << "Adjacency list of vertex " << v << ":" << endl;
			if (vertices[v].neighbors.size() > 0) {
				cout << "Edges: " << endl;
				for (vector<int>::iterator it = vertices[v].neighbors.begin(); it != vertices[v].neighbors.end(); ++it)  {
					cout << *it << " ";
				}
				cout << endl;
				cout << "Maybe: " << endl;
				for (vector<int>::iterator it = vertices[v].maybeNeighbors.begin(); it != vertices[v].maybeNeighbors.end(); ++it)  {
					cout << *it << " ";
				}
			}
			cout << endl;
		}
	}

	Graph MaybeToGraph(){
		;
	}

	// compare graphs
	bool operator==(const MaybeGraph& g) const
	{
		int size = vertices.size();
		if (size != g.vertices.size()) return false;
		for (int k = 0; k < size; ++k) {
			if (!(vertices[k] == g.vertices[k])) return false;
		}
		return true;
	}
};

#endif