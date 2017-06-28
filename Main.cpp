// example about structures

//#include "Atom.h"
//#include "Molecule.h"
#include "reader.h"
#include "Cutoff.h"
using namespace std;

#include <utility>
#include <algorithm>
#include <string>
#include <sstream>
#include <iterator>
#include <iostream>
#include <fstream>
#include <vector>
int main()
{
	// set paths to files to be compared
	string file1 = "R://LANL/SiDiamond/Extra/300K/minimized300.xyz" ;
	string file2 = "R://LANL/SiDiamond/Extra/300K/preminimize300.xyz" ;

	string myFiles [2] = {file1, file2};
	Graph myGraphs [2];

	// read files
	Reader myReader = Reader();
	for (int i = 0; i < 2; i++){
		//Reader myReader = Reader();
		if (myReader.Initialize(myFiles[i])) {
			vector<Molecule> molecules = myReader.GetMolecules();
			myGraphs[i] = Cutoff(molecules.back(), 0);

			// compare the two graphs
			typedef boost::undirected_graph<> Graph;
			typedef Graph::vertex_iterator VertexIterator;
			std::pair<VertexIterator, VertexIterator> vs0 = boost::vertices(myGraphs[0]);
			std::pair<VertexIterator, VertexIterator> vvs1 = boost::vertices(myGraphs[1]);

			
		}
	}

	std::cin.get();
	return 0;
}
