#include "reader.h"
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
std::vector<Molecule> Reader::GetMolecules() {
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