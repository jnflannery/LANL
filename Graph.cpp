#ifndef GRAPH_CPP
#define GRAPH_CPP
#include <iostream>
#include <vector>
#include <string>
#include "molecule.h"
#include <fstream>
#include <set>
using namespace std;

//Vertex
struct Vertex
{
	int id;
	vector<int> neighbors;
	vector<int> maybeNeighbors;

	bool operator==(const Vertex& a) const
	{
		set<int> vertexA(neighbors.begin(), neighbors.end());
		set<int> vertexB (a.neighbors.begin(), a.neighbors.end());
		if (vertexA.size() != vertexB.size())
			return false;
		return ((id == a.id) && (vertexA==vertexB));

	}
	bool operator<(const Vertex& a) const
	{
		return (id < a.id);
	}
};

//Class Graph
class Graph
{
private:
	int V;
	vector<Vertex> vertices;
public:
	Graph(int V)
	{
		this->V = V;
		for (int i = 0; i < V + 1; ++i) {
			Vertex v = { i };
			this->vertices.push_back(v);
		}
	}
	Graph()
	{
		this->V = 0;
	}
	Graph(vector<Vertex> verts){
		this->V = verts.size();		
		this->vertices = verts;
	}
	//Add MaybeEdge to Graph
	void addMaybeEdge(Vertex a, Vertex b)
	{
		a.maybeNeighbors.push_back(b.id);
		b.maybeNeighbors.push_back(a.id);
	}
	void addMaybeEdge(int a, int b)
	{
		vertices[a].maybeNeighbors.push_back(b);
		vertices[b].maybeNeighbors.push_back(a);
	}

	//get vertex by id
	Vertex getVertex(int id){
		return vertices[id];
	}

	//get all vertices
	vector<Vertex> getAllVertices(){
		return vertices;
	}

	//Print a vertex (using LAMMPS IDs)
	void printVertex(int id){
		cout << "Adjacency list of vertex " << id << ":" << endl;
		if (vertices[id].neighbors.size() > 0) {
			set<int> n1(vertices[id].neighbors.begin(), vertices[id].neighbors.end());
			for (set<int>::iterator it = n1.begin(); it != n1.end(); ++it)  {
				cout << *it << " ";
			}
		}
		cout << "\n";
		if (vertices[id].maybeNeighbors.size() > 0) {
			set<int> n1(vertices[id].neighbors.begin(), vertices[id].neighbors.end());
			cout << "Potential neighbors of " << id << ":" << endl;
			for (set<int>::iterator it = n1.begin(); it != n1.end(); ++it)  {
				cout << *it << " ";
			}
		}
		cout << "\n";
	}
	void printVertex(Vertex v){
		printVertex(v.id);
}
	//Get number of vertices
	int GetNumberOfVertices() {
		return V;
	}

	//Add Edge to Graph
	void addEdge(Vertex a, Vertex b)
	{
		a.neighbors.push_back(b.id);
		b.neighbors.push_back(a.id);
	}
	void addEdge(int a, int b)
	{
		vertices[a].neighbors.push_back(b);
		vertices[b].neighbors.push_back(a);
	}

	//Print the graph
	void printGraph()
	{
		int v;
		for (v = 1 ; v <= V; ++v)
		{
			cout << "Adjacency list of vertex " << v << ":" << endl;
			if (vertices[v].neighbors.size() > 0) {
				for (vector<int>::iterator it = vertices[v].neighbors.begin(); it != vertices[v].neighbors.end(); ++it)  {
					cout << *it << " ";
				}
			}
			cout << endl;
		}
	}
	bool compareAndReturnMismatches (const Graph& g, vector<int>& mismatches) 
	{
		bool isTrue = true;
		int size = vertices.size();
		if (size != g.vertices.size()) {
			cout << "wrong vertices size???" << endl;
			return false;
		}
		for (int k = 0; k < size; ++k) {
			if (!(vertices[k] == g.vertices[k])) {
				mismatches.push_back(k);
				isTrue = false;

			}
		}
		cout << endl;
		return isTrue;
	}

	// compare graphs
	bool operator==(const Graph& g) const
	{
		bool isTrue = true;
		int size = vertices.size();
		if (size != g.vertices.size()) {
			cout << "wrong vertices size???" << endl;
			return false;
		}
		for (int k = 0; k < size; ++k) {
			if (!(vertices[k] == g.vertices[k])) {
				cout << k << " ";
				isTrue = false;
				
			}
		}
		cout << endl;
		return isTrue;
	}
	void writeGraphAsDumpFile(std::string myFileName, Molecule m) {

		std::ofstream file = std::ofstream(myFileName);
		if (!file)
		{
			std::cout << myFileName << " cannot be accessed and/or written to. Terminating process";
		}
		else {
			file = std::ofstream(myFileName);
			int numberOfAtomsToPrint = 0;
			vector <int> relavantNumbers();
			for (int i = 0;i<=V;i++) {
				if (vertices.at(i).neighbors.size() > 0)
					numberOfAtomsToPrint++;
			}
			file << numberOfAtomsToPrint << endl;
			file << "Atoms. Timestep: 0" << endl; 
			for (int i = 0;i<=V;i++) {
				if (vertices.at(i).neighbors.size() > 0)
					file << 1 << " " << m.GetAtom(i).GetX() << " " << m.GetAtom(i).GetY() << " " << m.GetAtom(i).GetZ() << endl;
			}

		}
	}
};
#endif