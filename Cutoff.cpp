#include "cutoff.h"

#include <vector>
#include <math.h>

using namespace std;

Graph Cutoff(Molecule molecule, float rc)
{
	vector<Atom> atoms = molecule.GetMolecule();
	int size = molecule.GetNumberOfAtoms();

	// create grap object
	Graph gh;

	// add vertices
	typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
	vector<Vertex> verts;
	for (int i=0; i < size; i++) {
		verts.push_back(add_vertex(gh));
	}
	
	// check pairs of vertices, connect if within cutoff distance
	for (int i=0; i < size; i++) {
		for (int j = i+1; j < size; j++) {
			Atom atom1 = molecule.GetAtom(i);
			Atom atom2 = molecule.GetAtom(j);
			double dX = atom1.GetX() - atom2.GetX();
			double dY = atom1.GetY() - atom2.GetY();
			double dZ = atom1.GetZ() - atom2.GetZ();
			if (pow(dX, 2) + pow(dY, 2) + pow(dZ, 2) < pow(rc, 2)) {
				gh.add_edge(verts[i], verts[j]);
			}
		}
	}
	
	return gh;
}