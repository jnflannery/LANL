#pragma once
#include "box.h"
#include "molecule.h"
#include <vector>

class Boxlist {
private:
	 std::vector<Box> boxes;
	 int N; 

public:
	void AddBox(Box);
	Box GetBox(int, int, int);
	int FindBoxLocationX (int);
	int FindBoxLocationY (int);
	int FindBoxLocationZ (int);
	Boxlist();
	Boxlist(int);
};