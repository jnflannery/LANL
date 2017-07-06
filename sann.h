#pragma once
#pragma once
#include "boxlist.h"
#include "molecule.h"
#include <vector>
#include "graph.cpp"

#include <vector>
#include "atom.h"
#include <algorithm>
#include "molecule.h"
#include "sann.h"
#include "boxlist.h"
//check to see if stuff is saving  
class Sann {
public:
	int ComputeSannAtom(Atom centralAtom, vector<Atom> potentialNeighbors, Graph& g, double periodicDistance);
	Graph ComputeSannMolecule(Molecule molecule, Boxlist boxList);
};