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
	//you will likely have a different file name
	string myFileName = "C://Examples/example.txt" ;
	Reader myReader = Reader();
	if (myReader.Initialize(myFileName)) {
		vector<Molecule> molecules = myReader.GetMolecules();
		cout << molecules.at(0).GetAtom(4).GetY();
	}
	string line;
	cin >> line;



	return 0;
}

