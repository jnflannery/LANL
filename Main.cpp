// example about structures
#include <string>
#include <sstream>
#include "Atom.h"
#include "Molecule.h"
#include "reader.h"
using namespace std;

#include <iostream>
#include <fstream>
#include <vector>
int main()
{
	//you will  likely have a different file name
	string myFileName = "C://Examples/example2.txt" ;
	Reader myReader = Reader();
	if (myReader.Initialize(myFileName)) {
		Molecule molecule = myReader.GetMoleculeFromOutputFile();
	}
	string line;



	return 0;
}

