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
class Sann {
public:
	int ComputeSannAtom(Atom centralAtom, vector<Atom> potentialNeighbors, Graph& g);
	Graph ComputeSannMolecule(Molecule molecule, Boxlist boxList);
};