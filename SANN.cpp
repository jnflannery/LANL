using namespace std;
#include "sann.h"


//structure to hold atom IDs and distances from atom being computed. This prevents us from manipulating the order of the vector holding the atoms
struct AtomIdAndDistance {
	int id;
	double distance;
};
bool operator<(const AtomIdAndDistance &s1, const AtomIdAndDistance &s2) {
	return s1.distance < s2.distance;
}
bool CompareByDistance(AtomIdAndDistance lhs, AtomIdAndDistance rhs) { return lhs.distance < rhs.distance; }
void AtomDistanceSort(vector<AtomIdAndDistance>& neighborCandidates) {
	std::sort(neighborCandidates.begin(), neighborCandidates.end());
}

AtomIdAndDistance SimplifyNeighborCandidate(Atom centralAtom, Atom neighborCandidate, double periodicDistance)
{
	//get distance between two atoms and use atom's ID to create a simplified member of the structure
	AtomIdAndDistance pair = AtomIdAndDistance();
	pair.id = neighborCandidate.GetId();
	pair.distance = neighborCandidate.EuclidianPeriodicDistance(centralAtom, periodicDistance);
	return pair;
}

int Sann::ComputeSannAtom(Atom centralAtom, vector<Atom> potentialNeighbors, Graph& g, double periodicDistance) {
	double distanceSum, radius;
	int count=potentialNeighbors.size(); 
	int i;
	//add simplified potential neighbors to a vector
	vector<AtomIdAndDistance> neighborCandidates = vector <AtomIdAndDistance>();
	for (int i = 0; i < (int)potentialNeighbors.size()-1;i++) {
		if(i!=centralAtom.GetId()-1)
		neighborCandidates.push_back(SimplifyNeighborCandidate(centralAtom, potentialNeighbors.at(i), periodicDistance));
	}
	////if there aren't enough neighbors to do algorithm then quit
	if ((int)neighborCandidates.size() < 3) {
		return -1;
	}
	//potentially check verlet list if we have not done so already
	//sort candidates by their distance
	AtomDistanceSort(neighborCandidates);
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
	cout << centralAtom.GetId() <<" "<< centralAtom.GetX()<< " " << centralAtom.GetY()<< " " << centralAtom.GetZ()<<endl;
	for (int j = 0; j < i;j++) {
		int currentID = potentialNeighbors.at(neighborCandidates.at(j).id).GetId();
		int candidateID = neighborCandidates.at(j).id;
		cout << potentialNeighbors.at(neighborCandidates.at(j).id-1).GetId() << " " << potentialNeighbors.at(neighborCandidates.at(j).id-1).GetX() << " " << potentialNeighbors.at(neighborCandidates.at(j).id-1).GetY() << " " << potentialNeighbors.at(neighborCandidates.at(j).id-1).GetZ() << endl;
		g.addEdge(centralAtom.GetId(), neighborCandidates.at(j).id);
	}
	//g.printGraph();
	return i;
}
Graph Sann::ComputeSannMolecule(Molecule molecule, Boxlist boxList){
	Graph g = Graph(molecule.GetNumberOfAtoms());
	for (int i = 0; i < molecule.GetNumberOfAtoms()-1;i++) {
		vector<Atom> potentialNeighbors=vector<Atom>();//get candidates from the boxes... how do we know which boxes we are going to use from the list?
		ComputeSannAtom(molecule.GetAtom(i), potentialNeighbors, g, molecule.GetCubeSize());
	}
	return g;
}