// example about structures
#include <string>
#include <sstream>
#include "Atom.h"
#include "Molecule.h"
#include "reader.h"
#include "box.h"
#include "boxlist.h"
#include "boxbuilder.h"
using namespace std;

#include <iostream>
#include <fstream>
#include <vector>
int main()
{
	//you will  likely have a different file name
	string myFileName = "C:\\LAMMPS 64-bit 20170127\\bin\\SiDiamond\\extradefect.data";
	Reader myReader = Reader();
	if (myReader.Initialize(myFileName)) {
		Molecule molecule = myReader.GetMoleculeFromOutputFile();
		std::cout << "read molecule";
		double newcutoff = 5;
		BoxBuilder boxbuilder = BoxBuilder (newcutoff);
		Boxlist boxList = boxbuilder.BuildBoxes(molecule, newcutoff); 
	}
	string line;
	std::cin.get();

	return 0;
}