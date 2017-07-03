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
	string myFileName = "R://LANL/SiDiamond/Extra/500K/minimized1000.data" ;
	Reader myReader = Reader();
	if (myReader.Initialize(myFileName)) {
		Molecule molecule = myReader.GetMoleculeFromOutputFile();
		Sann sann = Sann();
		Graph g = Graph(1001);
		cout<< "i is: " <<sann.ComputeSannAtom(molecule.GetAtom(1000), molecule.GetAtomVector(), g, molecule.GetCubeSize());
//		g.printGraph();
	}
	string line;
	cin >> line;


	return 0;
}

