using namespace std;
#include <vector>
#include "atom.h"

//structure to hold atom IDs and distances from atom being computed. This prevents us from manipulating the order of the vector holding the atoms
struct AtomIdAndDistance {
	int id;
	float distance;
};
AtomIdAndDistance simplifyNeighborCandidate(Atom centralAtom, Atom neighborCandidate) {
	//get distance between two atoms and use atom's ID to 
	return AtomIdAndDistance();
}
void atomDistanceSort(vector<AtomIdAndDistance> neighborCandidates) {
	//sort AtomIdAndDistance's low to high
}

int computeSANN(int id, Atom centralAtom, vector<Atom> potentialNeighbors) {
	double distanceSum; 
	int count; 
	int i;
	//add simplified potential neighbors to a vector
	vector<AtomIdAndDistance> neighborCandidates = vector <AtomIdAndDistance>();
	for (int i = 0; i < (int)potentialNeighbors.size();i++) {
		neighborCandidates.push_back(simplifyNeighborCandidate(centralAtom, potentialNeighbors.at(i)));
	}
	//potentially check verlet list if we have not done so already
	//sort candidates by their distance
	atomDistanceSort(neighborCandidates);

	//if there aren't enough neighbors to do algorithm then quit
	if ((int)neighborCandidates.size() < 3) {
		return -1;
	}

}