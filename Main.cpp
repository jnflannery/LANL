// example about structures
#include <string>
#include <sstream>
#include "Atom.h"
#include "Molecule.h"
#include "cutoff.h"
#include "reader.h"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main()
{
	string myFileName = "R://LANL/SiDiamond/Extra/500K/minimized1000.data" ;
	Reader myReader = Reader();
	if (myReader.Initialize(myFileName)) {
		Molecule molecule = myReader.GetMoleculeFromOutputFile();
		Graph g = Cutoff(molecule, 2.52);
		
		//g.printGraph();
	}
	
	cin.get();

	return 0;
}
