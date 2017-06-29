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
				Vertex v = {i};
                this->vertices.push_back(v);
			}
		}
		Graph()
        {
            this->V = 0;
		}

		//Get number of vertices
		int GetNumberOfVertices(){
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
        
        //Print the graph
        void printGraph()
        {
            int v;
            for (v = 0; v < V; ++v)
            {
                cout<<"Adjacency list of vertex "<<v<<":"<<endl;
				for (vector<int>::iterator it = vertices[v].neighbours.begin(); it != vertices[v].neighbours.end(); ++it) {
				   cout << *it << " ";
				} 
                cout<<endl;
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
