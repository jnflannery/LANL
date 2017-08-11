#pragma once
#include "molecule.h"
#include "atom.h"
#include "Graph.cpp"
#include "cutoffmaybe.h"

Graph Cutoff(Molecule molecule, double rc);