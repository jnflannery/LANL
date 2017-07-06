// example about structures
#include <string>
#include <sstream>
#include "Atom.h"
#include "Molecule.h"
#include "sann.h"
#include "reader.h"
#include "boxbuilder.h"
#include "box.h"
#include "coordinate.h"
#include "boxlist.h"
using namespace std;

#include <iostream>
#include <fstream>
#include <vector>
int main()
{
	//you will likely have a different file name
	string myFileName = "C://Examples/PlatinumCrystalLattice.txt";
	Reader myReader = Reader();
	if (myReader.Initialize(myFileName)) {
		Molecule molecule = myReader.GetMoleculeFromOutputFile();
		Sann sann = Sann();
		Graph g = Graph(500);
		BoxBuilder boxBuilder = BoxBuilder(5);
		Boxlist boxes = boxBuilder.BuildBoxes(molecule, 5);
		Box box = boxes.GetBox(1, 1, 1);
		vector<Atom> originalBoxAtoms = box.getVectorOfAtomsFromBox(molecule);
		vector<Atom> atomsInBoxes = boxes.getAtomVectorOfNeighborCandidates(box, 5, molecule);
		
		cout << "i is: " << sann.ComputeSannAtom(originalBoxAtoms.at(0), atomsInBoxes, g, molecule.GetCubeSize());
		g.printGraph();
		g.writeGraphAsDumpFile("C://Examples/PlatinumCrystalLatticeAtom50Connectionwhonkows.xyz", molecule);
	}
	string line;
	cin >> line;


	return 0;
}