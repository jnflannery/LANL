// example about structures
#include <string>
#include <sstream>
#include "Atom.h"
#include "Molecule.h"
#include "reader.h"
#include "box.h"
#include "boxlist.h"
#include "boxbuilder.h"
#include "coordinate.h"
using namespace std;

#include <iostream>
#include <fstream>
#include <vector>
int main()
{
	//you will  likely have a different file name
	string myFileName = "C:\\LAMMPS 64-bit 20170127\\bin\\SiDiamond\\gapdefect.data";
	Reader myReader = Reader();
	if (myReader.Initialize(myFileName)) {
		Molecule molecule = myReader.GetMoleculeFromOutputFile();
		double newcutoff = 5;
		BoxBuilder boxbuilder = BoxBuilder (newcutoff);
		Boxlist boxList = boxbuilder.BuildBoxes(molecule, newcutoff); 
		Box myBox = FindNeighbours(boxList.GetBox(1,1,1), boxList.GetBoxSize()); 
		std::vector<Coordinate> neighbors = myBox.GetNeighborList();
		std::cout << "We made it here \n";
		std::cout << neighbors.at(26).x << " " << neighbors.at(26).y << " " << neighbors.at(26).z; 
	}
	string line;
	std::cin.get();

	return 0;
}