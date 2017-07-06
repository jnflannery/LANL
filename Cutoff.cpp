#include "cutoff.h"
#include "boxbuilder.h"
#include "coordinate.h"

#include <math.h>

using namespace std;

Graph Cutoff(Molecule molecule, float rc)
{
	vector<Atom> atoms = molecule.GetAtomVector();
	int size = molecule.GetNumberOfAtoms();

	// create graph object
	Graph gh(size);
	
	// build boxes
	BoxBuilder myBoxBuilder = BoxBuilder(rc);
	Boxlist boxlist = myBoxBuilder.BuildBoxes(molecule, rc);

	// check pairs of vertices, connect if within cutoff distance
	int edgecount = 0;
	
	vector<Box> boxes = boxlist.GetAllBoxes();
	for (vector<Box>::iterator itBox = boxes.begin(); itBox != boxes.end(); ++itBox){
		Box b = FindNeighbours(*itBox, boxlist.NumberOfBoxes());
		vector<Coordinate> neighborhood = b.GetNeighborList();
		vector<int> boxAtoms = b.GetAtomsFromBox();
		// itereate through atoms in middle box
		for (vector<int>::iterator itAtom = boxAtoms.begin(); itAtom != boxAtoms.end(); ++itAtom){
			//iterate through neighbouring boxes/coordinates
			for (vector<Coordinate>::iterator itNeighCoor = neighborhood.begin(); itNeighCoor != neighborhood.end(); ++itNeighCoor){
				Box Nbox = boxlist.GetBox(*itNeighCoor);
				vector<int> Natoms = Nbox.GetAtomsFromBox();
				//itereate though atoms in box by given coordinate
				for (vector<int>::iterator itNeighAtom = Natoms.begin(); itNeighAtom != Natoms.end(); ++itNeighAtom){
					Atom atom1 = molecule.GetAtom(*itAtom);
					Atom atom2 = molecule.GetAtom(*itNeighAtom);
					if (atom1.GetId() != atom2.GetId()) {
						double dX = atom1.GetX() - atom2.GetX();
						double dY = atom1.GetY() - atom2.GetY();
						double dZ = atom1.GetZ() - atom2.GetZ();
						double rsq = pow(rc, 2);
						if (pow(dX, 2) + pow(dY, 2) + pow(dZ, 2) < rsq) {
							gh.addEdge(*itAtom,*itNeighAtom);
							++edgecount;
						}
					}
				}
			}
		}
	}
	return gh;
}
