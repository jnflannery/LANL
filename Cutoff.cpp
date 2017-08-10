#include <math.h>

#include "boxbuilder.h"
#include "coordinate.h"
#include "cutoff.h"


using namespace std;

/*
	Creates and returns a graph from given molecule using cutoff criteria.
	This algorithm uses boxes of size r_c to avoid computing all the distances.
*/
Graph Cutoff(Molecule molecule, double rc)
{
	vector<Atom> atoms = molecule.GetAtomVector();
	int size = molecule.GetNumberOfAtoms();

	// create graph object
	Graph gh(size);

	// build boxes 
	BoxBuilder myBoxBuilder = BoxBuilder(rc);
	Boxlist boxlist = myBoxBuilder.BuildBoxes(molecule, rc);
	vector<Box> boxes = boxlist.GetAllBoxes();

	//iterate through boxes
	for (vector<Box>::iterator itBox = boxes.begin(); itBox != boxes.end(); ++itBox){
		//find neighboring boxes
		Box box = FindNeighbours(*itBox, boxlist.NumberOfBoxes());
		vector<Coordinate> neighborhood = box.GetNeighborList();
		vector<int> boxAtoms = box.GetAtomsFromBox();
		// itereate through atoms in middle box
		for (vector<int>::iterator itAtom = boxAtoms.begin(); itAtom != boxAtoms.end(); ++itAtom){
			//iterate through neighboring boxes/coordinates
			for (vector<Coordinate>::iterator itNeighCoor = neighborhood.begin(); itNeighCoor != neighborhood.end(); ++itNeighCoor){
				Box Nbox = boxlist.GetBox(*itNeighCoor);
				vector<int> Natoms = Nbox.GetAtomsFromBox();
				//itereate though atoms in box by given coordinate
				for (vector<int>::iterator itNeighAtom = Natoms.begin(); itNeighAtom != Natoms.end(); ++itNeighAtom){
					Atom atom1 = molecule.GetAtom(*itAtom);
					Atom atom2 = molecule.GetAtom(*itNeighAtom);
					if (atom1.GetId() < atom2.GetId()) { //use '<' to avoid adding edge twice. this assumes graph is undirected.
						//add edge if the atoms are within the cutoff
						if (atom1.EuclidianPeriodicDistance(atom2, molecule.GetCubeSize()) < rc) {
							gh.addEdge(atom1.GetId(), atom2.GetId());
						}
					}
				}
			}
		}
	}
	return gh;
}