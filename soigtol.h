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

class SoigTol {
private:
	Graph graph;
public:
	vector<AtomWithRadius> ComputeSpheresSoigTol(Molecule, double periodicBoundary);
	int FindNeighborsSoigTol(Atom, Molecule, vector<AtomWithRadius>, double, double, Graph&);
	int CreateGraphSoigTol(Molecule, vector<AtomWithRadius>, double, double, Graph&);
};