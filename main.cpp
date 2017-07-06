// example about structures
#include <string>
#include <sstream>
#include "Atom.h"
#include "Molecule.h"
#include "sann.h"
#include "soig.h"
#include "reader.h"
using namespace std;

#include <iostream>
#include <fstream>
#include <vector>
int main()
{
	//you will  likely have a different file name
	string myFileName = "C://Examples/bulklattice.data" ;
	Reader myReader = Reader();
	if (myReader.Initialize(myFileName)) {
		Molecule molecule = myReader.GetMoleculeFromOutputFile();
		Graph g = Graph(1000);
		Soig soig = Soig();
		cout << "We made it here \n";
		vector<AtomWithRadius> sphereList = soig.ComputeSpheresSoig(molecule, molecule.GetCubeSize());
		cout << "We made the sphere list \n";
		int output = soig.FindAtomNeighbors(molecule.GetAtom(250), molecule, sphereList, molecule.GetCubeSize(), g);
		// int output = soig.CreateGraphSoig(molecule, sphereList, molecule.GetCubeSize(), g);
		cout << "We created the graph";
		// cout<< "i is: " <<sann.ComputeSannAtom(molecule.GetAtom(0), molecule.GetAtomVector(), g, molecule.GetCubeSize());
		g.printGraph();
		cout << sphereList.at(250).radius; 
	    g.writeGraphAsDumpFile("C://Graph Testing//soigtest.txt", molecule);
	}
	string line;
	cin >> line;
	std::cin.get();


	return 0;
}