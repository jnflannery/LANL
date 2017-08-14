#pragma once

#include "atom.h"
#include "Graph.cpp"
#include "molecule.h"

Graph CutoffMaybe(Molecule molecule, double rc, double Rc);
Graph CutoffWithForces(Molecule molecule, double rc, double Rc);
Graph CutoffCentroid(Molecule molecule, double rc, double Rc);
Graph CutoffDoubleCentroid(Molecule molecule, double rc, double Rc);