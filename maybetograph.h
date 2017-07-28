#include "Graph.cpp"

Graph MaybeToGraphCentroid(Graph gh, Molecule mol);
Graph MaybeToGraphForces(Graph gh, Molecule mol);
Graph MaybeToGraphDoubleCentroid(Graph gh, Molecule mol);
double dot_product(triplet a, triplet b);
double size(triplet a);
triplet add(triplet, triplet);
triplet subtract(triplet, triplet);
triplet mult(double, triplet);