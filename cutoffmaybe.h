#pragma once

#include "molecule.h"
#include "atom.h"
#include "Graph.cpp"
#include "MaybeGraph.cpp"

Graph CutoffMaybe(Molecule molecule, double rc, double Rc);

Graph MaybeToGraphCentroid(MaybeGraph gh, Molecule mol);
