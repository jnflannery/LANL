#pragma once
#include "molecule.h"
#include "atom.h"
#include "graph.cpp"
#include <vector>
#include <algorithm>
//check to see if stuff is saving  
class DistanceComparison {
public:
	int ComputeDistAtom(Atom centralAtom, vector<Atom> potentialNeighbors, Graph& g, double periodicDistance);
	Graph ComputeDistMolecule(Molecule molecule);
};