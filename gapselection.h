#pragma once
#include "molecule.h"
#include "atom.h"
#include "graph.cpp"
#include <vector>
#include <algorithm>
class GapSelection {
private: 
	int GapSelectionAtom(Atom centralAtom, vector<Atom> potentialNeighbors, Graph& g, double periodicDistance);
public:
	Graph GapSelectionMolecule(Molecule molecule);
};
