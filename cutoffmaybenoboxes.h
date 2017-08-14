#pragma once

#include "molecule.h"
#include "atom.h"
#include "Graph.cpp"

Graph CutoffMaybeNoBoxes(Molecule molecule, double rc, double Rc);
Graph CutoffWithForcesNoBoxes(Molecule molecule, double rc, double Rc);
Graph CutoffCentroidNoBoxes(Molecule molecule, double rc, double Rc);
Graph CutoffDoubleCentroidNoBoxes(Molecule molecule, double rc, double Rc);