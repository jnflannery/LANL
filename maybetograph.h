#pragma once
#include "Graph.cpp"

Graph MaybeToGraphCentroid(Graph gh, Molecule mol);
Graph MaybeToGraphForces(Graph gh, Molecule mol);
double dot_product(triplet a, triplet b);
double size(triplet a);
triplet add(triplet, triplet);
triplet mult(double, triplet);