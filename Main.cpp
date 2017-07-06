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

bool compareGraphsCutOff(string, string, double);

int main()
{
	const int firstTime = 5010;
	const int lastTime = 15000;
	const int timeStep = 200;

	for (int time = firstTime; time <= lastTime; time+=timeStep){
		string pre = "R://LANL/Data/SiDiamond/Extra/500K/minimized" + to_string(time) + ".data";
		string min = "R://LANL/Data/SiDiamond/Extra/500K/preminimize" + to_string(time)+ ".data";
		
		bool same = compareGraphsCutOff(pre, min, 3.14);
		cout << same << "\n";
	}

	cout << "Done.";
	cin.get();

	return 0;
}


bool compareGraphsCutOff(string pre, string min, double rc){
	string fileNames[2] = {pre, min};
	Graph graphs[2];
	for (int i = 0; i<2; ++i){
		Reader myReader = Reader();
		if (myReader.Initialize(fileNames[i])) {
			Molecule molecule = myReader.GetMoleculeFromOutputFile();
			graphs[i] = Cutoff(molecule, rc);
		}
	}
	return (graphs[0] == graphs[1]);
}