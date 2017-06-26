#pragma once
#include <vector>
#include "Atom.h"
class Molecule {
private:
	int timestep;
	std::vector<Atom> atoms;
public:
	Molecule();
	void SetTimestep(int);
	void AddAtom(Atom);
	int GetTimestep();
	std::vector<Atom> GetMolecule();
	Atom GetAtom(int);
	int GetNumberOfAtoms();
};