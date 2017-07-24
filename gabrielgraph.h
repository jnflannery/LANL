#pragma once
#include "molecule.h"
#include "atom.h"
#include "graph.cpp"
#include <vector>
#include "box.h"
#include <algorithm>
//check to see if stuff is saving  
class GabrielGraph {
public:
	int ComputeGabrielAtom(Atom centralAtom, Molecule molecule, Graph & g, double periodicDistance);
	Graph ComputeGabrielMolecule(Molecule molecule);
	Atom findMidpointAtom(Atom atom1, Atom atom2, double periodicBoundary);
	double findMidpointOneDimension(double x1, double x2, double periodicBoundary);
};