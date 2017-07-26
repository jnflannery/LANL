#pragma once
#pragma once
#include "molecule.h"
#include <vector>
#include "graph.cpp"
#include "atom.h"
#include <algorithm>
//check to see if stuff is saving  
class Sann {
public:
	int ComputeSannAtom(Atom centralAtom, vector<Atom> potentialNeighbors, Graph& g, double periodicDistance);
//	Graph ComputeSannMolecule(Molecule molecule, Boxlist boxList);
	Graph ComputeSannMolecule(Molecule molecule);
};