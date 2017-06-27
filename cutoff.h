#include "molecule.h"
#include "atom.h"
#include <boost/graph/undirected_graph.hpp>

typedef boost::undirected_graph<> Graph;
Graph Cutoff(Molecule molecule, float rc);
