#pragma once
#include "molecule.h"
#include "box.h"
#include "boxlist.h"
#include <iostream>
#include "math.h"

class BoxBuilder
{
private:
	double cutoff;

public:
	Boxlist BuildBoxes(Molecule, double);
	BoxBuilder(double cutoff);
	double GetBoxSize();
};