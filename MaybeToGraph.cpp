#include "maybetograph.h"


Graph MaybeToGraphCentroid(Graph gh, Molecule mol){
	for (auto V : gh.getAllVertices()){

		triplet center(0, 0, 0);
		double weight = V.neighbors.size();
		for (auto N : V.neighbors){
			//add N-V to C
			Atom Vatom = mol.GetAtom(V.id);
			Atom Natom = mol.GetAtom(N);
			triplet VtoN = Vatom.VectorTo(Natom, mol.GetCubeSize());
			center = add(center, VtoN);
		}
		for (auto N : V.maybeNeighbors){
			Atom Vatom = mol.GetAtom(V.id);
			Atom Natom = mol.GetAtom(N);
			triplet VtoN = Vatom.VectorTo(Natom, mol.GetCubeSize());
			//check if adding moves cetre closer to V
			triplet	newcenter = add( mult( weight/(weight+1.0), center ), mult( 1.0/(weight+1.0), VtoN ) );
			//add an edge if it is
			if (size(newcenter) < size(center)) gh.addEdge(V.id, N);
		}
	}
	return gh;
}

Graph MaybeToGraphForces(Graph gh, Molecule mol){
	for (auto V : gh.getAllVertices()){

		for (auto N : V.maybeNeighbors){
			Atom Vatom = mol.GetAtom(V.id);
			Atom Natom = mol.GetAtom(N);
			triplet VtoN = Vatom.VectorTo(Natom, mol.GetCubeSize());
			double VtoNsize = size(VtoN);
			triplet ForceVectorEdgeAtom(Natom.GetFx(), Natom.GetFy(), Natom.GetFz());
			triplet ForceVectorCenterAtom(Vatom.GetFx(), Vatom.GetFy(), Vatom.GetFz());
			triplet ForceVector = subtract(ForceVectorEdgeAtom, ForceVectorCenterAtom);
			triplet VtoNforced = add(VtoN, ForceVector);
			double VtoNforcedSize = size(VtoNforced);
			if (VtoNforcedSize < VtoNsize-0.05) gh.addEdge(V.id, N);
		}
	}
	return gh;
}

Graph MaybeToGraphDoubleCentroid(Graph gh, Molecule mol){
	for (auto V : gh.getAllVertices()){
		triplet centerV(0, 0, 0);
		double weight = V.neighbors.size();
		for (auto N : V.neighbors){
			//add N-V to C
			Atom Vatom = mol.GetAtom(V.id);
			Atom Natom = mol.GetAtom(N);
			triplet VtoN = Vatom.VectorTo(Natom, mol.GetCubeSize());
			centerV = add(centerV, VtoN);
		}
		for (auto N : V.maybeNeighbors){
			triplet centerN(0,0,0);
			Atom Vatom = mol.GetAtom(V.id);
			Vertex NVertex = gh.getVertex(N);
			Atom Natom = mol.GetAtom(N);
			triplet VtoNeighbor = Vatom.VectorTo(Natom, mol.GetCubeSize());
			for (auto Q: NVertex.neighbors){
				Atom Qatom = mol.GetAtom(Q);
				triplet NtoQ = Natom.VectorTo(Qatom, mol.GetCubeSize());
				centerN = add(centerN, NtoQ);
			}
			double DistanceBetweenAtoms = size(VtoNeighbor);
			triplet VtoCenterN = add(VtoNeighbor, centerN);
			triplet CenterVtoCenterN = subtract(VtoCenterN, centerV);
			if (size(CenterVtoCenterN) < size(VtoNeighbor)) gh.addEdge(V.id, N);
		}
	}
	return gh;
}

triplet Atom::VectorTo(Atom atom, double periodicBoundary){
	return triplet(PeriodicDiffX(atom, periodicBoundary), PeriodicDiffY(atom, periodicBoundary), PeriodicDiffZ(atom, periodicBoundary));
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