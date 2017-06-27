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
	std::vector<Molecule> GetMolecules();
};