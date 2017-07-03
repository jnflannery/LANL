// example about structures
#include <string>
#include <sstream>
#include "Atom.h"
#include "Molecule.h"
#include "sann.h"
#include "reader.h"
using namespace std;

#include <iostream>
#include <fstream>
#include <vector>
int main()
{
	//you will  likely have a different file name
	string myFileName = "C://Examples/PlatinumCrystalLattice.txt" ;
	Reader myReader = Reader();
	if (myReader.Initialize(myFileName)) {
		Molecule molecule = myReader.GetMoleculeFromOutputFile();
		Sann sann = Sann();
		Graph g = Graph(500);
		cout<< "i is: " <<sann.ComputeSannAtom(molecule.GetAtom(0), molecule.GetAtomVector(), g, molecule.GetCubeSize());
		g.printGraph();
	}
	string line;
	cin >> line;


	return 0;
}

