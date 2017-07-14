#pragma once

#include "molecule.h"
#include "atom.h"
#include "Graph.cpp"
#include "MaybeGraph.cpp"

MaybeGraph CutoffMaybe(Molecule molecule, double rc, double Rc);
