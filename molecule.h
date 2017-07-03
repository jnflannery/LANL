#pragma once
#include <vector>
#include "Atom.h"
class Molecule {
private:
	int timestep;
	double cubeSize;
	std::vector<Atom> atoms;
public:
	void setCubeSize(double);
	double GetCubeSize();
	Molecule();
	void SetTimestep(int);
	Molecule(int numberOfAtoms);
	void AddAtom(Atom);
	int GetTimestep();
	void SetAtomWithIndex(Atom atom, int i);
	std::vector<Atom> GetAtomVector();
	Atom GetAtom(int);
	int GetNumberOfAtoms();
};