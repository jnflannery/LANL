#pragma once
#pragma once

#include "molecule.h"
#include "atom.h"
#include "Graph.cpp"

Graph CutoffMaybe(Molecule molecule, double rc, double Rc);
Graph CutoffWithForces(Molecule molecule, double rc, double Rc);
Graph CutoffCentroid(Molecule molecule, double rc, double Rc);