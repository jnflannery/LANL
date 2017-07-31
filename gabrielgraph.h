#pragma once
#include "molecule.h"
#include "atom.h"
#include "graph.cpp"
#include <vector>
#include "molecule.h"
#include <algorithm>
//check to see if stuff is saving  
class GabrielGraph {
public:
	int ComputeGabrielAtom(Atom centralAtom, Molecule molecule, Graph & g, double theta, double periodicDistance);
	Graph ComputeGabrielMolecule(Molecule molecule, double theta);
	Atom findMidpointAtom(Atom atom1, Atom atom2, double periodicBoundary);
	double findMidpointOneDimension(double x1, double x2, double periodicBoundary);
	bool isOtherNeighborInEllipse(Atom TargetAtom, Atom ClosestNeighbor, Atom OtherAtom, double theta, double periodicDistance);
};