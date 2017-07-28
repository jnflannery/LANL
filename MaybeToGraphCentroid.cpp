#include "maybetographcentroid.h"


Graph MaybeToGraphCentroid(Graph gh, Molecule mol) {
	for (auto V : gh.getAllVertices()) {

		triplet center(0, 0, 0);
		double weight = V.neighbors.size();
		for (auto N : V.neighbors) {
			//add N-V to C
			Atom Vatom = mol.GetAtom(V.id);
			Atom Natom = mol.GetAtom(N);
			triplet VtoN = Vatom.VectorTo(Natom, mol.GetCubeSize());
			center = add(center, VtoN);
		}
		for (auto N : V.maybeNeighbors) {
			Atom Vatom = mol.GetAtom(V.id);
			Atom Natom = mol.GetAtom(N);
			triplet VtoN = Vatom.VectorTo(Natom, mol.GetCubeSize());
			//check if adding moves cetre closer to V
			triplet	newcenter = add(mult(weight / (weight + 1.0), center), mult(1.0 / (weight + 1.0), VtoN));
			//add an edge if it is
			if (size(newcenter) < size(center)) gh.addEdge(V.id, N);
		}
	}
	return gh;
}

triplet Atom::VectorTo(Atom atom, double periodicBoundary) {
	return triplet(PeriodicDiffX(atom, periodicBoundary), atom.PeriodicDiffY(atom, periodicBoundary), atom.PeriodicDiffZ(atom, periodicBoundary));
}

double dot_product(triplet a, triplet b) {
	return sqrt(get<0>(a)*get<0>(b) + get<1>(a)*get<1>(b) + get<2>(a)*get<2>(b));
}

double size(triplet a) {
	return dot_product(a, a);
}

triplet add(triplet a, triplet b) {
	return triplet(get<0>(a) + get<0>(b), get<1>(a) + get<1>(b), get<2>(a) + get<2>(b));
}

triplet mult(double c, triplet a) {
	return triplet(get<0>(a)*c, get<1>(a)*c, get<2>(a)*c);
}