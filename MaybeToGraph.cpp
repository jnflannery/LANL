#include "maybetograph.h"

/*
	Evaluate potential edges using position of centroids.
*/
Graph MaybeToGraphCentroid(Graph gh, Molecule mol){
	for (auto V : gh.getAllVertices()){
		//compute position of centroid of the atom V
		triplet center(0, 0, 0);
		double weight = V.neighbors.size();
		for (auto N : V.neighbors){
			//add N-V to C
			Atom Vatom = mol.GetAtom(V.id);
			Atom Natom = mol.GetAtom(N);
			triplet VtoN = Vatom.VectorTo(Natom, mol.GetCubeSize());
			center = add(center, VtoN);
		}
		//go through potential neighbors
		for (auto N : V.maybeNeighbors){
			Atom Vatom = mol.GetAtom(V.id);
			Atom Natom = mol.GetAtom(N);
			triplet VtoN = Vatom.VectorTo(Natom, mol.GetCubeSize());
			//check if adding the potential neighbor moves cetre closer to V
			triplet	newcenter = add(mult( weight/(weight+1.0), center ), 
									mult( 1.0/(weight+1.0), VtoN ));
			//add an edge if so
			if (size(newcenter) < size(center)) gh.addEdge(V.id, N);
		}
	}
	return gh;
}

/*
	Evaluate potential edges using forces acting on the atoms.
*/
Graph MaybeToGraphForces(Graph gh, Molecule mol){
	for (auto V : gh.getAllVertices()){

		for (auto N : V.maybeNeighbors){
			Atom Vatom = mol.GetAtom(V.id);
			Atom Natom = mol.GetAtom(N);
			triplet VtoN = Vatom.VectorTo(Natom, mol.GetCubeSize());
			double VtoNsize = size(VtoN);
			//get the forces
			triplet ForceVectorEdgeAtom(Natom.GetFx(), Natom.GetFy(), Natom.GetFz());
			triplet ForceVectorCenterAtom(Vatom.GetFx(), Vatom.GetFy(), Vatom.GetFz());
			//compute force on edge atom with reppect to the central atom
			triplet ForceVector = subtract(ForceVectorEdgeAtom, ForceVectorCenterAtom);
			//compute distance of the forced positions
			triplet VtoNforced = add(VtoN, ForceVector);
			double VtoNforcedSize = size(VtoNforced);
			// if forced positions are significantly closer, add potential neighbor
			if (VtoNforcedSize < VtoNsize-0.01) gh.addEdge(V.id, N);
		}
	}
	return gh;
}

/*
	Evaluate potential edges using relative positions of centroids.
*/
Graph MaybeToGraphDoubleCentroid(Graph gh, Molecule mol){
	for (auto V : gh.getAllVertices()){
		//compute centroid for neighbors of central atom V (wrt position of V)
		triplet centerV(0, 0, 0);
		double weight = V.neighbors.size();
		for (auto N : V.neighbors){
			//add N-V to C
			Atom Vatom = mol.GetAtom(V.id);
			Atom Natom = mol.GetAtom(N);
			triplet VtoN = Vatom.VectorTo(Natom, mol.GetCubeSize());
			centerV = add(centerV, VtoN);
      centerV = mult(1/weight, centerV);
		}

		for (auto N : V.maybeNeighbors){
			// compute centroid for neighbors of potential neighbor N with 
			// respect to the position of N
			triplet centerN(0,0,0);
			Atom Vatom = mol.GetAtom(V.id);
			Vertex NVertex = gh.getVertex(N);
			Atom Natom = mol.GetAtom(N);
			triplet VtoNeighbor = Vatom.VectorTo(Natom, mol.GetCubeSize());
      double weight = gh.getVertex(N).neighbors.size();
			for (auto Q: NVertex.neighbors){
				Atom Qatom = mol.GetAtom(Q);
				triplet NtoQ = Natom.VectorTo(Qatom, mol.GetCubeSize());
				centerN = add(centerN, NtoQ);
        centerN = mult(1/weight, centerN);
			}
			//compare distances between atoms to distances between centroids
			double DistanceBetweenAtoms = size(VtoNeighbor);
			triplet VtoCenterN = add(VtoNeighbor, centerN);
			triplet CenterVtoCenterN = subtract(VtoCenterN, centerV);
			if (size(CenterVtoCenterN) < size(VtoNeighbor)) gh.addEdge(V.id, N);
		}
	}
	return gh;
}


/*
	Functions for manipulating triplets of real numbers representing a vector in
	three dimensional periodic space.
*/
//Return vector from one atom to another taking peropdic boundary into account
triplet Atom::VectorTo(Atom atom, double periodicBoundary){
	return triplet(PeriodicDiffX(atom, periodicBoundary), 
				   PeriodicDiffY(atom, periodicBoundary), 
				   PeriodicDiffZ(atom, periodicBoundary));
}

double dot_product(triplet a, triplet b){
	return (get<0>(a)*get<0>(b) + get<1>(a)*get<1>(b) + get<2>(a)*get<2>(b));
}

double size(triplet a){
	return sqrt(dot_product(a, a));
}

triplet add(triplet a, triplet b){
	return triplet(get<0>(a)+get<0>(b), get<1>(a)+get<1>(b), get<2>(a)+get<2>(b));
}

triplet subtract(triplet a, triplet b){
	return triplet(get<0>(a)-get<0>(b), get<1>(a)-get<1>(b), get<2>(a)-get<2>(b));
}

triplet mult(double c, triplet a){
	return triplet(get<0>(a)*c, get<1>(a)*c, get<2>(a)*c);
}