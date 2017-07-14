#pragma once

#include "molecule.h"
#include "atom.h"
#include "Graph.cpp"

Graph Shadow(Molecule molecule, double rc, double S);
bool shadows(Atom atom, Atom pot_neigh, Atom oth_atom, double S, Molecule molecule);
