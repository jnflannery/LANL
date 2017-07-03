
#pragma once
#include "box.h"
#include "molecule.h"
#include <vector>

class Boxlist {
private:
	std::vector<Box> boxes;
	int N;
	double BoxSize;

public:
	void AddBox(Box);
	void AssignBoxSize(double);
	double GetBoxSize();
	Box GetBox(int, int, int);
	int FindBoxLocationX(int);
	int FindBoxLocationY(int);
	int FindBoxLocationZ(int);
	int NumberOfBoxes();
	Box FindBoxWithAtom(Atom);
	Boxlist();
	Boxlist(int);
};