#pragma once
#include "boxlist.h"
#include "molecule.h"
#include <vector>

#include <vector>
#include "atom.h"
#include <algorithm>
#include "molecule.h"
// #include "sann.h"
#include "boxlist.h"
#include "Graph.cpp"

struct AtomWithRadius {
	int id;
	double radius;
};

class Soig {
	private:
		Graph graph; 
	public: 
		vector<AtomWithRadius> ComputeSpheresSoig(Molecule, double periodicBoundary);
		int FindAtomNeighbors(Atom, Molecule, vector<AtomWithRadius>, double, Graph&); 
		Graph CreateGraphSoig(Molecule, vector<AtomWithRadius>, double, Graph&); 
};