#pragma once
#include "gapselection.h"
//structure to hold atom IDs and distances from atom being computed. This prevents us from manipulating the order of the vector holding the atoms

struct DistanceHolder {
	int id;
	double distanceFromMainAtom;
	DistanceHolder() {

	}
	DistanceHolder(int newId, double newDistanceFromMainAtom) {
		id = newId;
		distanceFromMainAtom = newDistanceFromMainAtom;
	}
};
//sort overload
bool operator<(const DistanceHolder &s1, const DistanceHolder &s2) {
	return s1.distanceFromMainAtom < s2.distanceFromMainAtom;}
void AtomDistanceSort(vector<DistanceHolder>& neighborCandidates) {
	std::sort(neighborCandidates.begin(), neighborCandidates.end());
}
struct GapHolder {
	int id;
	double distanceFromMainAtom;
	double distanceFromClosestDifference;
	GapHolder(int newId, double newDistanceFromMainAtom, double newDistanceFromClosestDifference) {
		id = newId;
		distanceFromMainAtom = newDistanceFromMainAtom;
		distanceFromClosestDifference = newDistanceFromClosestDifference;
	}
};
//sort overload
bool operator<(const GapHolder &s1, const GapHolder &s2) {
	return s1.distanceFromClosestDifference > s2.distanceFromClosestDifference;
}
void AtomDifferenceSort(vector<GapHolder>& neighborCandidates) {
	std::sort(neighborCandidates.begin(), neighborCandidates.end());
}



DistanceHolder SimplifyNeighborCandidate(Atom centralAtom, Atom neighborCandidate, double periodicDistance)
{
	//get distance between two atoms and use atom's ID to create a simplified member of the structure
	DistanceHolder pair = DistanceHolder();
	pair.id = neighborCandidate.GetId();
	pair.distanceFromMainAtom = neighborCandidate.EuclidianPeriodicDistance(centralAtom, periodicDistance);
	return pair;
}


int GapSelection::GapSelectionAtom(Atom centralAtom, vector<Atom> potentialNeighbors, Graph & g, double periodicDistance)
{
	double distanceSum, radius;
	int count = potentialNeighbors.size();
	//add simplified potential neighbors to a vector
	vector<DistanceHolder> neighborCandidates = vector <DistanceHolder>();
	DistanceHolder newCandidate;
	for (int i = 0; i < (int)potentialNeighbors.size();i++) {
		if (potentialNeighbors.at(i).GetId() != centralAtom.GetId()) {
			newCandidate = SimplifyNeighborCandidate(centralAtom, potentialNeighbors.at(i), periodicDistance);
			if (newCandidate.distanceFromMainAtom<6)
				neighborCandidates.push_back(newCandidate);
		}
	}
	////if there aren't enough neighbors to do algorithm then quit
	if ((int)neighborCandidates.size() < 3) {
		return -1;
	}
	//sort candidates by their distance
	AtomDistanceSort(neighborCandidates);
	vector<GapHolder> differenceCandidates = vector <GapHolder>();
	double distance;
	int num=13;
	if (neighborCandidates.size() < 13)
		num = neighborCandidates.size();
	//find differences between atom distances
	for (int i = 1; i <= num;i++) {
		distance = neighborCandidates.at(i).distanceFromMainAtom - neighborCandidates.at(i - 1).distanceFromMainAtom;
		GapHolder newDiff = GapHolder(neighborCandidates.at(i).id, neighborCandidates.at(i).distanceFromMainAtom, distance);
		differenceCandidates.push_back(newDiff);
	}
	//sort the differences
	AtomDifferenceSort(differenceCandidates);
	vector<DistanceHolder> fewNeighborCandidates = vector <DistanceHolder>();
	for (int i = 0; i < differenceCandidates.size(); i++) {
		fewNeighborCandidates.push_back(DistanceHolder(differenceCandidates.at(i).id, differenceCandidates.at(i).distanceFromMainAtom));
	}
	//add atoms until you get to the biggest gap
	bool endReached = false;
	int i = 0;
	while (endReached == false) {
		if (neighborCandidates.at(i).id == fewNeighborCandidates.at(0).id) {
			endReached = true;
		}
		else {
			g.addEdge(centralAtom.GetId(), neighborCandidates.at(i).id);
			i++;
		}
	}
	return 0;
}

Graph GapSelection::GapSelectionMolecule(Molecule molecule)
{
	Graph g = Graph(molecule.GetNumberOfAtoms());
	for (int i = 1;i <= molecule.GetNumberOfAtoms();i++)
		GapSelectionAtom(molecule.GetAtom(i), molecule.GetAtomVector(), g, molecule.GetCubeSize());
	return g;
}