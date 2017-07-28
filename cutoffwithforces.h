#pragma once

#include "molecule.h"
#include "atom.h"
#include "Graph.cpp"

Graph CutoffWithForces(Molecule molecule, double rc, double Rc);

Graph MaybeToGraphForces(Graph gh, Molecule mol);