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
	string min = "R://LANL/SiDiamond/Extra/500K/minimized1000.xyz" ;
	string pre_min = "R://LANL/SiDiamond/Extra/500K/preminimize1000.xyz" ;

	string myFiles [2] = {min, pre_min};
	Graph myGraphs [2];

	// read files
	Reader myReader = Reader();
	for (int i = 0; i < 2; i++){
		//Reader myReader = Reader();
		if (myReader.Initialize(myFiles[i])) {
			vector<Molecule> molecules = myReader.GetMolecules();

			//apply algorithm to get graph
			myGraphs[i] = Cutoff(molecules.back(), 3.14);


		}
	}

	// compare the two graphs
	cout << (myGraphs[0] == myGraphs[1]) << endl;


	std::cin.get();
	return 0;
}
