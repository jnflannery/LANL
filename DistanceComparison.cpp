#pragma once
#include "distancecomparison.h"
//structure to hold atom IDs and distances from atom being computed. This prevents us from manipulating the order of the vector holding the atoms

struct AtomIdAndDistance3 {
	int id;
	double distanceFromMainAtom;
	AtomIdAndDistance3() {

	}
	AtomIdAndDistance3(int newId, double newDistanceFromMainAtom) {
		id = newId;
		distanceFromMainAtom = newDistanceFromMainAtom;
	}
};
bool operator<(const AtomIdAndDistance3 &s1, const AtomIdAndDistance3 &s2) {
	return s1.distanceFromMainAtom < s2.distanceFromMainAtom;}
void AtomDistanceSort(vector<AtomIdAndDistance3>& neighborCandidates) {
	std::sort(neighborCandidates.begin(), neighborCandidates.end());
}
struct AtomIdDistanceAndDifference3 {
	int id;
	double distanceFromMainAtom;
	double distanceFromClosestDifference;
	AtomIdDistanceAndDifference3(int newId, double newDistanceFromMainAtom, double newDistanceFromClosestDifference) {
		id = newId;
		distanceFromMainAtom = newDistanceFromMainAtom;
		distanceFromClosestDifference = newDistanceFromClosestDifference;
	}
};
bool operator<(const AtomIdDistanceAndDifference3 &s1, const AtomIdDistanceAndDifference3 &s2) {
	return s1.distanceFromClosestDifference > s2.distanceFromClosestDifference;
}
void AtomDifferenceSort(vector<AtomIdDistanceAndDifference3>& neighborCandidates) {
	std::sort(neighborCandidates.begin(), neighborCandidates.end());
}



AtomIdAndDistance3 SimplifyNeighborCandidate(Atom centralAtom, Atom neighborCandidate, double periodicDistance)
{
	//get distance between two atoms and use atom's ID to create a simplified member of the structure
	AtomIdAndDistance3 pair = AtomIdAndDistance3();
	pair.id = neighborCandidate.GetId();
	pair.distanceFromMainAtom = neighborCandidate.EuclidianPeriodicDistance(centralAtom, periodicDistance);
	return pair;
}


int DistanceComparison::ComputeDistAtom(Atom centralAtom, vector<Atom> potentialNeighbors, Graph & g, double periodicDistance)
{
	if (centralAtom.GetId() == 1000)
	{
		///
		centralAtom.GetX();
	}
	double distanceSum, radius;
	int count = potentialNeighbors.size();
	//add simplified potential neighbors to a vector
	vector<AtomIdAndDistance3> neighborCandidates = vector <AtomIdAndDistance3>();
	AtomIdAndDistance3 newCandidate;
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
	extern string TIMESTEP;
	extern string TEMPERATURE;
	//potentially check verlet list if we have not done so already
	//sort candidates by their distance
	AtomDistanceSort(neighborCandidates);
	vector<AtomIdDistanceAndDifference3> differenceCandidates = vector <AtomIdDistanceAndDifference3>();
	double distance;
	int num=13;
	if (neighborCandidates.size() < 13)
		num = neighborCandidates.size();
	for (int i = 1; i <= num;i++) {
		distance = neighborCandidates.at(i).distanceFromMainAtom - neighborCandidates.at(i - 1).distanceFromMainAtom;
		AtomIdDistanceAndDifference3 newDiff = AtomIdDistanceAndDifference3(neighborCandidates.at(i).id, neighborCandidates.at(i).distanceFromMainAtom, distance);
		differenceCandidates.push_back(newDiff);
	}
	AtomDifferenceSort(differenceCandidates);
	vector<AtomIdAndDistance3> fewNeighborCandidates = vector <AtomIdAndDistance3>();
	for (int i = 0; i < differenceCandidates.size(); i++) {
		fewNeighborCandidates.push_back(AtomIdAndDistance3(differenceCandidates.at(i).id, differenceCandidates.at(i).distanceFromMainAtom));
	}
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

Graph DistanceComparison::ComputeDistMolecule(Molecule molecule)
{
	Graph g = Graph(molecule.GetNumberOfAtoms());
	for (int i = 1;i <= molecule.GetNumberOfAtoms();i++)
		ComputeDistAtom(molecule.GetAtom(i), molecule.GetAtomVector(), g, molecule.GetCubeSize());
	return g;
}
