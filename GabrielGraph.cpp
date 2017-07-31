#include "gabrielgraph.h"
#include "maybetograph.h"

struct AtomIdAndDistance2 {
	int id;
	double distanceFromMainAtom;
	AtomIdAndDistance2() {

	}
	AtomIdAndDistance2(int newId, double newDistanceFromMainAtom) {
		id = newId;
		distanceFromMainAtom = newDistanceFromMainAtom;
	}
};
bool operator<(const AtomIdAndDistance2 &s1, const AtomIdAndDistance2 &s2) {
	return s1.distanceFromMainAtom < s2.distanceFromMainAtom;
}
void AtomDistanceSort(vector<AtomIdAndDistance2>& neighborCandidates) {
	std::sort(neighborCandidates.begin(), neighborCandidates.end());
}

AtomIdAndDistance2 SimplifyNeighborCandidate(Atom centralAtom, Atom neighborCandidate, double periodicDistance)
{
	//get distance between two atoms and use atom's ID to create a simplified member of the structure
	AtomIdAndDistance2 pair = AtomIdAndDistance2();
	pair.id = neighborCandidate.GetId();
	pair.distanceFromMainAtom = neighborCandidate.EuclidianPeriodicDistance(centralAtom, periodicDistance);
	return pair;
}

int GabrielGraph::ComputeGabrielAtom(Atom centralAtom, Molecule molecule, Graph & g,double theta, double periodicDistance)
{
	if (centralAtom.GetId() == 1000)
	{
		///
		centralAtom.GetX();
	}
	double distanceSum, radius;
	vector <Atom> potentialNeighbors = molecule.GetAtomVector();
	int count = potentialNeighbors.size();
	//add simplified potential neighbors to a vector
	vector<AtomIdAndDistance2> neighborCandidates = vector <AtomIdAndDistance2>();
	AtomIdAndDistance2 newCandidate;
	for (int i = 0; i < (int)potentialNeighbors.size(); i++) {
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
	//potentially check verlet list if we have not done so already
	//sort candidates by their distance
	AtomDistanceSort(neighborCandidates);

	for (int i = 0; i < neighborCandidates.size(); i++) {
		bool atomInSphere = false;
		Atom toTest = molecule.GetAtom(neighborCandidates.at(i).id);
		Atom midpoint = findMidpointAtom(centralAtom, toTest, periodicDistance);
		double distance = toTest.EuclidianPeriodicDistance(centralAtom, periodicDistance);
		double sphereRadius = midpoint.EuclidianPeriodicDistance(centralAtom, periodicDistance);
		for (int j = 0; j < i; j++) {
			Atom potentialSphereRuiner = molecule.GetAtom(neighborCandidates.at(j).id);
			if (isOtherNeighborInEllipse(centralAtom, toTest, potentialSphereRuiner,theta, periodicDistance)) {
				atomInSphere = true;
			}
		}
		if (atomInSphere == false) {
			g.addEdge(centralAtom.GetId(), neighborCandidates.at(i).id);
		}
	}
	return 0;
}

//theta determines the thickness of ellipse
bool GabrielGraph::isOtherNeighborInEllipse(Atom TargetAtom, Atom ClosestNeighbor, Atom OtherAtom, double theta, double periodicDistance)
{
	triplet a = TargetAtom.VectorTo(OtherAtom, periodicDistance); //distanceAtomPeriodic(TargetAtom, OtherAtom);
	triplet b = TargetAtom.VectorTo(ClosestNeighbor, periodicDistance); //distanceAtomPeriodic(TargetAtom, ClosestNeighbor);
	double projX = dot_product(a, b) / size(a);
	if (projX < 0 && projX > size(b)) {
		return false;
	}
	double radiusSq = (theta*theta)*(abs(projX)*(1 - abs(projX)));
	double dSq = pow(size(a), 2) - pow(projX, 2);
		if (dSq < radiusSq) {
			return true;
		}
		else {
			return false;
		}
}

Graph GabrielGraph::ComputeGabrielMolecule(Molecule molecule, double theta)
{
	Graph g = Graph(molecule.GetNumberOfAtoms());
	for (int i = 1; i <= molecule.GetNumberOfAtoms(); i++)
		ComputeGabrielAtom(molecule.GetAtom(i), molecule, g, theta, molecule.GetCubeSize());
	//g.printGraph();
	return g;
}

Atom GabrielGraph::findMidpointAtom(Atom atom1, Atom atom2, double periodicBoundary)
{
	double x = findMidpointOneDimension(atom1.GetX(), atom2.GetX(), periodicBoundary);
	double y = findMidpointOneDimension(atom1.GetY(), atom2.GetY(), periodicBoundary);
	double z = findMidpointOneDimension(atom1.GetZ(), atom2.GetZ(), periodicBoundary);
	return Atom(x, y, z, -1);
}

double GabrielGraph::findMidpointOneDimension(double x1, double x2, double periodicBoundary)
{
	Atom atom = Atom();
	if (x1 > x2) {
		double temp = x1;
		x1 = x2;
		x2 = temp;
	}
	double dx = atom.getPeriodicDistanceOneD(x1, x2, periodicBoundary);
	if (x2 - x1 > periodicBoundary*.5) {
		dx = x2 + (dx*.5);
		if (dx > periodicBoundary) {
			dx = dx - periodicBoundary;
		}
	}
	else {
		dx = x1 + (dx*.5);
	}
	return dx;
}