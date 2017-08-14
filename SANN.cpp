#pragma once
#include "sann.h"


//structure to hold atom IDs and distances from atom being computed. This prevents us from manipulating the order of the vector holding the atoms
struct AtomIdAndDistance {
	int id;
	double distanceFromMainAtom;
};
bool operator<(const AtomIdAndDistance &s1, const AtomIdAndDistance &s2) {
	return s1.distanceFromMainAtom < s2.distanceFromMainAtom;
}
bool CompareByDistance(AtomIdAndDistance lhs, AtomIdAndDistance rhs) { return lhs.distanceFromMainAtom < rhs.distanceFromMainAtom; }
void AtomDistanceSort(vector<AtomIdAndDistance>& neighborCandidates) {
	std::sort(neighborCandidates.begin(), neighborCandidates.end());
}

AtomIdAndDistance SimplifyNeighborCandidate(Atom centralAtom, Atom neighborCandidate, double periodicDistance)
{
	//get distance between two atoms and use atom's ID to create a simplified member of the structure
	AtomIdAndDistance pair = AtomIdAndDistance();
	pair.id = neighborCandidate.GetId();
	pair.distanceFromMainAtom = neighborCandidate.EuclidianPeriodicDistance(centralAtom, periodicDistance);
	return pair;
}
Graph Sann::ComputeSannMolecule(Molecule molecule) {
	Graph g = Graph(molecule.GetNumberOfAtoms());
	for (int i = 1;i <= molecule.GetNumberOfAtoms();i++)
		ComputeSannAtom(molecule.GetAtom(i), molecule.GetAtomVector(), g, molecule.GetCubeSize());
	return g;
}
int Sann::ComputeSannAtom(Atom centralAtom, vector<Atom> potentialNeighbors, Graph& g, double periodicDistance) {
	if (centralAtom.GetId() == 1000)
	{
		///
		centralAtom.GetX();
	}
	double distanceSum, radius;
	int count = potentialNeighbors.size();
	int i;
	//add simplified potential neighbors to a vector
	vector<AtomIdAndDistance> neighborCandidates = vector <AtomIdAndDistance>();
	for (int i = 0; i < (int)potentialNeighbors.size();i++) {
	
		if (potentialNeighbors.at(i).GetId() != centralAtom.GetId()) {
			AtomIdAndDistance newAtom = SimplifyNeighborCandidate(centralAtom, potentialNeighbors.at(i), periodicDistance);
			if(newAtom.distanceFromMainAtom<5.5)
			neighborCandidates.push_back(newAtom);
		}
	}
	////if there aren't enough neighbors to do algorithm then quit
	if (centralAtom.GetId() == 37 || centralAtom.GetId() == 43) {
		centralAtom.GetId();
	}
	if ((int)neighborCandidates.size() < 3) {
		return -1;
	}
	//potentially check verlet list if we have not done so already
	//sort candidates by their distance
	AtomDistanceSort(neighborCandidates);
	distanceSum = 0;
	if (centralAtom.GetId() == 839)
	{
		///
		centralAtom.GetX();
	}
	for (int i = 0;i < 3;i++) {
		distanceSum = neighborCandidates.at(i).distanceFromMainAtom + distanceSum;
	}
	//radius should be equal to distanceSum/(i-2)
	radius = distanceSum;
	double prevDistanceDifference = neighborCandidates.at(2).distanceFromMainAtom - neighborCandidates.at(1).distanceFromMainAtom;
	i = 3;
	//adjust the radius to make sure it is hasn't converged yet or anything
	while ((i < neighborCandidates.size()) && (radius > neighborCandidates[i].distanceFromMainAtom)) {//|| (neighborCandidates[i].distance - neighborCandidates[i - 1].distance<prevDistanceDifference * 5)|| neighborCandidates[i].distance - neighborCandidates[i - 1].distance<.1) {
		distanceSum = neighborCandidates.at(i).distanceFromMainAtom + distanceSum;
		prevDistanceDifference = neighborCandidates.at(i).distanceFromMainAtom - neighborCandidates.at(i - 1).distanceFromMainAtom;
		radius = distanceSum / (i - 2.0);
		i++;
		//(neighborCandidates[i].distance-neighborCandidates[i-1].distance<prevDistanceDifference*14)||
	}

	//if there are so many neighbors that it doesn't converge
	if (i == count)
		return -1;
	//Everything worked great! now we add these things to the graph structure
	for (int j = 0; j < i;j++) {
		int candidateID = neighborCandidates.at(j).id;
		g.addEdge(centralAtom.GetId(), neighborCandidates.at(j).id);
	}
	//g.printGraph();
	return i;
}

