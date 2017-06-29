#include "cutoff.h"

#include <math.h>

using namespace std;

Graph Cutoff(Molecule molecule, float rc)
{
	vector<Atom> atoms = molecule.GetMolecule();
	int size = molecule.GetNumberOfAtoms();

	// create graph object
	Graph gh(size);

	// check pairs of vertices, connect if within cutoff distance
	int edgecount = 0;
	for (int i=0; i < size; i++) {
		for (int j = i+1; j < size; j++) {
			Atom atom1 = molecule.GetAtom(i);
			Atom atom2 = molecule.GetAtom(j);
			double dX = atom1.GetX() - atom2.GetX();
			double dY = atom1.GetY() - atom2.GetY();
			double dZ = atom1.GetZ() - atom2.GetZ();
			if (pow(dX, 2) + pow(dY, 2) + pow(dZ, 2) < pow(rc, 2)) {
				gh.addEdge(i,j);
				++edgecount;
			}
		}
	}
	cout << "Number of edges: " << edgecount << endl;
	return gh;
}
