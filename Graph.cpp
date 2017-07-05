#include <iostream>
#include <vector>
using namespace std;


//Vertex
struct Vertex
{
	int id;
	vector<int> neighbours;

	bool operator==(const Vertex& a) const
	{
		return ((id == a.id) && (is_permutation(neighbours.begin(), neighbours.end(), a.neighbours.begin())));

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
		for (int i = 0; i < V; ++i) {
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
	void addEdge(int id1, int id2)
	{
		vertices[id1].neighbours.push_back(id2);
		vertices[id2].neighbours.push_back(id1);
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
		cout << "Neighbours of vertex " << (id) << ":\n";
		for (vector<int>::iterator it = vertices[id-1].neighbours.begin(); it != vertices[id-1].neighbours.end(); ++it) {
			cout << (((*it)-1)) << " ";
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
};