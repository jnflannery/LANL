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
	vector<int> neighbours;

	bool operator==(const Vertex& a) const
	{
		if (neighbours.size() != a.neighbours.size()) return false;
		set<int> n1(neighbours.begin(), neighbours.end());
		set<int> n2(a.neighbours.begin(), a.neighbours.end());
		return (n1 == n2);
		//return ((id == a.id) && (is_permutation(neighbours.begin(), neighbours.end(), a.neighbours.begin())));

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

	//Get number of vertices
	int GetNumberOfVertices() {
		return V;
	}

	//Add Edge to Graph
	void addEdge(Vertex a, Vertex b)
	{
		a.neighbours.push_back(b.id);
		b.neighbours.push_back(a.id);
	}
	void addEdge(int a, int b)
	{
		vertices[a].neighbours.push_back(b);
		vertices[b].neighbours.push_back(a);
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
		if (vertices[id].neighbours.size() > 0) {
			for (vector<int>::iterator it = vertices[id].neighbours.begin(); it != vertices[id].neighbours.end(); ++it)  {
				cout << *it << " ";
			}
		}
		cout << "\n";
	}
	void printVertex(Vertex v){
		printVertex(v.id);
	}

	//Print the graph
	void printGraph()
	{
		int v;
		for (v = 1; v <= V; ++v)
		{
			printVertex(v);
		}
	}

	// compare graphs
	bool operator==(const Graph& g) const
	{
		int size = vertices.size();
		if (size != g.vertices.size()) return false;
		for (int k = 0; k < size; ++k) {
			if (!(vertices[k] == g.vertices[k])) return false;
		}
		return true;
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
				if (vertices.at(i).neighbours.size() > 0)
					numberOfAtomsToPrint++;
			}
			file << numberOfAtomsToPrint << endl;
			file << "Atoms. Timestep: 0" << endl; 
			for (int i = 0;i<=V;i++) {
				if (vertices.at(i).neighbours.size() > 0)
					file << 1 << " " << m.GetAtom(i).GetX() << " " << m.GetAtom(i).GetY() << " " << m.GetAtom(i).GetZ() << endl;
			}

		}
	}
};