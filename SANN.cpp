using namespace std;
#include <vector>
#include "atom.h"
#include <algorithm>
#include "molecule.h"
#include "Graph.cpp"


//structure to hold atom IDs and distances from atom being computed. This prevents us from manipulating the order of the vector holding the atoms
struct AtomIdAndDistance {
	int id;
	double distance;
};
bool CompareById(AtomIdAndDistance lhs, AtomIdAndDistance rhs) { return lhs.id < rhs.id; }
void atomDistanceSort(vector<AtomIdAndDistance> neighborCandidates) {
	std::sort(neighborCandidates.begin(), neighborCandidates.end(), CompareById);
}
AtomIdAndDistance simplifyNeighborCandidate(Atom centralAtom, Atom neighborCandidate) {
	//get distance between two atoms and use atom's ID to create a simplified member of the structure
	AtomIdAndDistance pair = AtomIdAndDistance();
	pair.id = neighborCandidate.GetId();
	pair.distance = neighborCandidate.EuclidianDistance(centralAtom);
	return AtomIdAndDistance();
}

int computeSannAtom(Atom centralAtom, vector<Atom> potentialNeighbors, Graph g) {
	double distanceSum, radius;
	int count=potentialNeighbors.size(); 
	int i;
	//add simplified potential neighbors to a vector
	vector<AtomIdAndDistance> neighborCandidates = vector <AtomIdAndDistance>();
	for (int i = 0; i < (int)potentialNeighbors.size();i++) {
		neighborCandidates.push_back(simplifyNeighborCandidate(centralAtom, potentialNeighbors.at(i)));
	}
	//if there aren't enough neighbors to do algorithm then quit
	if ((int)neighborCandidates.size() < 3) {
		return -1;
	}
	//potentially check verlet list if we have not done so already
	//sort candidates by their distance
	atomDistanceSort(neighborCandidates);
	distanceSum = 0;
	for (int i = 0;i < 3;i++) {
		distanceSum = neighborCandidates.at(i).distance + distanceSum;
	}
	//radius should be equal to distanceSum/(i-2)
	radius = distanceSum;
	i = 3;
	//adjust the radius to make sure it is hasn't converged yet or anything
	while ((i < neighborCandidates.size()) && radius > neighborCandidates[i].distance) {
		distanceSum = neighborCandidates.at(i).distance + distanceSum;
		radius = distanceSum / (i - 2.0);
		i++;
	}
	//if there are so many neighbors that it doesn't converge
	if (i == count)
		return -1;
	//Everything worked great! now we add these things to the graph structure
	for (int j = 0; j < i;j++) {
		g.addEdge(centralAtom.GetId, neighborCandidates.at(j).id);
	}
	return i;
}
Graph computeSannMolecule(Molecule moleule /*insert boxes*/){
}