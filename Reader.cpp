#include "reader.h"
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

std::vector<std::string> split(const char *str, char c = ' ')
{
	std::vector<std::string> result;

	do
	{
		const char *begin = str;

		while (*str != c && *str)
			str++;

		result.push_back(std::string(begin, str));
	} while (0 != *str++);

	return result;
}
bool Reader::Initialize(std::string myFileName) {
	fileName = myFileName;
	
	std::ifstream myFile(myFileName);
	if (!myFile)
	{
		std::cout << myFileName << " cannot be read and/or does not open. Terminating process";
		return false;
	}
	else {
		file = std::ifstream(myFileName);
		return true;
	}
}
std::vector<Molecule> Reader::GetMoleculesFromDump() {
	int numberOfAtoms;
	std::string line;
	file >> numberOfAtoms;
	getline(file, line);
	std::vector<Molecule> molecules=std::vector<Molecule>();
	while (getline(file, line)) {
		int timestepLocation = line.find("Timestep");
		if (timestepLocation > 0 && timestepLocation < 3000) {
			std::cout << line.substr(timestepLocation, line.length() - timestepLocation);
			std::cout << line + "this was the line \r\n";
			std::cout << "number of atoms is:" << numberOfAtoms << "\r\n";
			Molecule newMolecule = Molecule();
			float a, x, y, z;
			for (int atomsSoFar = 0; atomsSoFar < numberOfAtoms; atomsSoFar++)
			{
				file >> a;
				file >> x;
				file >> y;
				file >> z;
				std::cout << x << " " << y << " " << z << "\r\n";
				Atom newAtom = Atom(x, y, z, (int)a);
				newMolecule.AddAtom(newAtom);
				std::cout << newMolecule.GetAtom(atomsSoFar).GetX() << " " << newMolecule.GetAtom(atomsSoFar).GetY() << " " << newMolecule.GetAtom(atomsSoFar).GetZ() << "\r\n";
				std::cout << "molecule size is: " << newMolecule.GetNumberOfAtoms() << "\r\n";

			}
			molecules.push_back(newMolecule);
		}
	}
	return molecules;
}
/*
Parse a line that looks like this:
"10 1 2.2556317953628206e+001 1.9830244238804401e+001 1.9849061312614726e+001 0 0 0"
*/
Atom Reader::ParseAtomLine(std::string line) {
	int id;
	float x, y, z;
	std::vector<std::string> outputAsStrings = split(line.c_str(), ' ');
	id = stoi(outputAsStrings.at(0));
	x = ScientificNotationToFloat(outputAsStrings.at(2));
	y = ScientificNotationToFloat(outputAsStrings.at(3));
	z = ScientificNotationToFloat(outputAsStrings.at(4));
	Atom atom = Atom();
	atom.SetId(id);
	atom.SetX(x);
	atom.SetY(y);
	atom.SetZ(z);
	return atom;
}
/*
turn a string that looks like this: "2.2556317953628206e+001"
into a float like 22.55631...
*/
float Reader::ScientificNotationToFloat(std::string numberAsString) {
	std::vector<std::string> numbersSeparatedByAnE = split(numberAsString.c_str(), 'e');
	float number = strtof(numbersSeparatedByAnE.at(0).c_str(), NULL); 
	int powerOfE = stoi(numbersSeparatedByAnE.at(1));
	for (int i = 0;i < powerOfE;i++) {
		number = number * 10;
	}
	return number;
}
float Reader::GetCubeSizeFromLine(std::string line) {
	float cubeSize;
	std::vector<std::string> splitLine = split(line.c_str());
	cubeSize = ScientificNotationToFloat(splitLine.at(1));
	return cubeSize;
}
Molecule Reader::GetMoleculeFromOutputFile() {
	int numberOfAtoms, timestep; 
	std::string line;
	getline(file, line);
	int timestepLocation = line.find("timestep");
	timestep = stoi(line.substr(timestepLocation+11, line.length() - (timestepLocation+11)));
	getline(file, line);
	file >> numberOfAtoms;
	/*
	below we create the molecule now that we know how many atoms there will be
	we will read each atom, and then put it into the molecule's atom vector in
	the spot at Atom.id-1
	*/
	Molecule molecule = Molecule(numberOfAtoms); 
	molecule.SetTimestep(timestep);
	//iterate over lines with no good info except for the line that has the size
	for (int i = 0;i < 13;i++) {
		getline(file, line);
		if (i == 4)
			molecule.setCubeSize(GetCubeSizeFromLine(line));
	}
	for (int i = 0;i < numberOfAtoms;i++) {
		getline(file, line);
		Atom atom = ParseAtomLine(line);
		molecule.SetAtomWithIndex(atom, atom.GetId()-1);
	}
	return molecule;
}