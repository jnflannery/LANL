#pragma once

#include "molecule.h"
#include "atom.h"
#include "Graph.cpp"

typedef tuple<double, double, double> triplet;

Graph Shadow(Molecule molecule, double rc, double S);
bool shadows(Atom atom, Atom pot_neigh, Atom oth_atom, double S, Molecule molecule);
double dot_product(triplet a, triplet b);
double size(triplet a);