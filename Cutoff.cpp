#include "cutoff.h"
#include "boxbuilder.h"
#include "coordinate.h"

#include <math.h>

using namespace std;

Graph Cutoff(Molecule molecule, double rc)
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
					if (atom1.GetId() < atom2.GetId()) { //use '<' to avoid adding edge twice. this assumes graph is undirected.
						if (atom1.EuclidianPeriodicDistance(atom2, molecule.GetCubeSize()) < rc) {
						//if (atom1.EuclidianDistance(atom2) < rc) {
							gh.addEdge(*itAtom,*itNeighAtom);
							//gh.addEdge(atom1.GetId(), atom2.GetId());
							++edgecount;
						}
					}
				}
			}
		}
	}
	return gh;
}
