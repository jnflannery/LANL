#pragma once
#include <vector>
#include "Atom.h"
class Molecule {
private:
	int timestep;
	float cubeSize;
	std::vector<Atom> atoms;
public:
	void setCubeSize(float);
	float getCubeSize();
	Molecule();
	void SetTimestep(int);
	Molecule(int numberOfAtoms);
	void AddAtom(Atom);
	int GetTimestep();
	void SetAtomWithIndex(Atom atom, int i);
	std::vector<Atom> GetMolecule();
	Atom GetAtom(int);
	int GetNumberOfAtoms();
};