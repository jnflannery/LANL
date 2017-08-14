#pragma once
#pragma once
#include "molecule.h"
#include <vector>
#include "graph.cpp"
#include "atom.h"
#include <algorithm>
//check to see if stuff is saving  
class Sann {
private:
	int ComputeSannAtom(Atom centralAtom, vector<Atom> potentialNeighbors, Graph& g, double periodicDistance);

public:
	Graph ComputeSannMolecule(Molecule molecule);
};