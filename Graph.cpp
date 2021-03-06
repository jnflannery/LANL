#ifndef GRAPH_CPP
#define GRAPH_CPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "errorstats.h"
#include "molecule.h"

using namespace std;

/*
	Store information about vertex: 
		vertex ID
		list of neighbors
		list of potential neighbors, 
*/
struct Vertex
{
	int id;
	vector<int> neighbors;
	vector<int> maybeNeighbors;

	// two vertices are considered same if they have same neighbors
	bool operator==(const Vertex& a) const
	{
		set<int> vertexA(neighbors.begin(), neighbors.end());
		set<int> vertexB(a.neighbors.begin(), a.neighbors.end());
		if (vertexA.size() != vertexB.size())
			return false;
		return ((id == a.id) && (vertexA == vertexB));

	}
	bool operator<(const Vertex& a) const
	{
		return (id < a.id);
	}
	//stores information about mismatches in ErrorStats
	bool compareAndReturnDifferences(const Vertex& a, ErrorStats & errors) {
		set<int> vertexA(neighbors.begin(), neighbors.end());
		set<int> vertexB(a.neighbors.begin(), a.neighbors.end());
		if((id == a.id) && (vertexA == vertexB))
			return true;
		std::set<int>::iterator it;
		for (it = vertexA.begin(); it != vertexA.end(); ++it)
		{
			if (!vertexB.count(*it))
				errors.mismatchedAtoms.at(id).push_back(*it);
		}
		for (it = vertexB.begin(); it != vertexB.end(); ++it)
		{
			if (!vertexA.count(*it))
				errors.mismatchedAtoms.at(id).push_back(*it);
		}
		return ((id == a.id) && (vertexA == vertexB));
	}
};

/*
	Store graph as adjacency list. 
*/
class Graph
{
private:
	//Number of vertices V
	int V;
	vector<Vertex> vertices;
public:
	// Create an empty graph
	Graph()
	{
		this->V = 0;
	}
	// Create graph with V vertices, no edges
	Graph(int V)
	{
		this->V = V;
		for (int i = 0; i < V + 1; ++i) {
			Vertex v = { i };
			this->vertices.push_back(v);
		}
	}
	// Create graph from given vertices
	Graph(vector<Vertex> verts) {
		this->V = verts.size();
		this->vertices = verts;
	}
	//Add a potential edge to the graph
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
	Vertex getVertex(int id) {
		return vertices[id];
	}

	//get all vertices
	vector<Vertex> getAllVertices() {
		return vertices;
	}

	//Print a vertex (using LAMMPS IDs)
	void printVertex(int id) {
		cout << "Adjacency list of vertex " << id << ":" << endl;
		if (vertices[id].neighbors.size() > 0) {
			set<int> n1(vertices[id].neighbors.begin(), vertices[id].neighbors.end());
			for (set<int>::iterator it = n1.begin(); it != n1.end(); ++it) {
				cout << *it << " ";
			}
		}
		cout << "\n";
		if (vertices[id].maybeNeighbors.size() > 0) {
			set<int> n1(vertices[id].neighbors.begin(), vertices[id].neighbors.end());
			cout << "Potential neighbors of " << id << ":" << endl;
			for (set<int>::iterator it = n1.begin(); it != n1.end(); ++it) {
				cout << *it << " ";
			}
		}
		cout << "\n";
	}
	void printVertex(Vertex v) {
		printVertex(v.id);
	}
	//Get number of vertices
	int GetNumberOfVertices() {
		return V;
	}

	//Add edge to Graph
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
		for (v = 1; v <= V; ++v)
		{
			cout << "Adjacency list of vertex " << v << ":" << endl;
			if (vertices[v].neighbors.size() > 0) {
				for (vector<int>::iterator it = vertices[v].neighbors.begin(); it != vertices[v].neighbors.end(); ++it) {
					cout << *it << " ";
				}
			}
			cout << endl;
		}
	}

	// compare graphs. graphs are considered same if all their vertices are same
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
				isTrue = false;
			}
		}
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
			for (int i = 0;i <= V;i++) {
				if (vertices.at(i).neighbors.size() > 0)
					numberOfAtomsToPrint++;
			}
			file << numberOfAtomsToPrint << endl;
			file << "Atoms. Timestep: 0" << endl;
			for (int i = 0;i <= V;i++) {
				if (vertices.at(i).neighbors.size() > 0)
					file << 1 << " " << m.GetAtom(i).GetX() << " " << m.GetAtom(i).GetY() << " " << m.GetAtom(i).GetZ() << endl;
			}

		}
	}
	// Simple hash algorithm assigns a number to each graph
	int getHashValue() {
		long long hash = 0;
		int whatIsIt = 0;
		for (int i = 0; i < vertices.size();i++) {
			set<int> neigh = set<int>(vertices.at(i).neighbors.begin(), vertices.at(i).neighbors.end());
			std::set<int>::iterator it;
			for (it = neigh.begin(); it != neigh.end(); ++it)
			{
				whatIsIt = *it;
				hash = hash + ((hash + i + 1)*(*it));
				hash = hash % 100003;
			}
		}
		return hash;
	}
	bool compareAndReturnMismatches(const Graph& g, ErrorStats & errors)
	{
		bool isTrue = true;
		int size = vertices.size();
		if (size != g.vertices.size()) {
			cout << "wrong vertices size???" << endl;
			return false;
		}
		for (int k = 0; k < size; ++k) {
			if (!(vertices[k].compareAndReturnDifferences(g.vertices[k], errors))) {
				isTrue = false;

			}
		}
		errors.setAll();
		cout << endl;
		return isTrue;
	}
	bool compareAndReturnMismatches(const Graph& g, vector<int>& mismatches)
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
		return isTrue;
	}
};
#endif