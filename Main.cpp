#include "Atom.h"
#include "Molecule.h"
#include "reader.h"
#include "cutoff.h"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main()
{
	cout << std::boolalpha;

	// set paths to files to be compared
	string min = "R://LANL/SiDiamond/Extra/500K/minimized1000.data" ;
	string pre_min = "R://LANL/SiDiamond/Extra/500K/preminimize1000.data" ;

	string myFiles [2] = {min, pre_min};
	Graph myGraphs [2];
	
	

	// read files
	Reader myReader = Reader();

	

	for (int i = 0; i < 2; i++){
		//Reader myReader = Reader();
		if (myReader.Initialize(myFiles[i])) {

			Molecule molecule = myReader.GetMoleculeFromOutputFile();


			//apply algorithm to get graph
			cout << "Applying Cutoff to graph " << i << endl;
			myGraphs[i] = Cutoff(molecule, 2.5);

		}
	}

	// compare the two graphs
	cout << "The graphs are same ? " << (myGraphs[0] == myGraphs[1]) << endl;


	std::cin.get();

	return 0;
}

