#include <string>
#include "molecule.h"
#include <vector>

#include <iostream>
#include <fstream>
class Reader {
private:
	std::string fileName;
	std::ifstream file;
public:
	bool Initialize(std::string);//gives the reader a file and it checks the validity of said file. Will output false if it can't be opened
	std::vector<Molecule> GetMoleculesFromDump();
	Atom ParseAtomLine(std::string line);
	float ScientificNotationToFloat(std::string numberAsString);
	float GetCubeSizeFromLine(std::string line);
	Molecule GetMoleculeFromOutputFile();
};