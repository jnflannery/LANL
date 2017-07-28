
#pragma once
#include "box.h"
#include "coordinate.h"
#include <iostream>
#include "molecule.h"

class Boxlist {
private:
	std::vector<Box> boxes;
	int N;
	double BoxSize;

public:
	void AddBox(Box);
	void AssignBoxSize(double);
	double GetBoxSize();
	std::vector<Atom> getAtomVectorOfNeighborCandidates(Box box, int boxesPerSide, Molecule molecule);
	Box GetBox(int, int, int);
	Box GetBox(Coordinate);
	std::vector<Box> GetAllBoxes();
	int FindBoxLocationX(int);
	int FindBoxLocationY(int);
	int FindBoxLocationZ(int);
	int NumberOfBoxes();
	Box FindBoxWithAtom(Atom);
	Boxlist();
	Boxlist(int);
};