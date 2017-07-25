#pragma once

#include "molecule.h"
#include "atom.h"
#include "Graph.cpp"

Graph CutoffMaybe(Molecule molecule, double rc, double Rc);

Graph MaybeToGraphCentroid(Graph gh, Molecule mol);
