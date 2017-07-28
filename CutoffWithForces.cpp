#include "cutoffwithforces.h"
#include "boxbuilder.h"
#include "coordinate.h"
#include <tuple>
#include <math.h>

using namespace std;
typedef tuple<double, double, double> triplet;

Graph CutoffWithForces(Molecule molecule, double rc, double Rc)
{
	vector<Atom> atoms = molecule.GetAtomVector();
	int size = molecule.GetNumberOfAtoms();

	// create graph object
	Graph gh(size);

	// build boxes
	BoxBuilder myBoxBuilder = BoxBuilder(Rc);
	Boxlist boxlist = myBoxBuilder.BuildBoxes(molecule, Rc);
	// check pairs of vertices, connect if within cutoff distance	
	vector<Box> boxes = boxlist.GetAllBoxes();
	for (vector<Box>::iterator itBox = boxes.begin(); itBox != boxes.end(); ++itBox) {
		Box box = FindNeighbours(*itBox, boxlist.NumberOfBoxes());
		vector<Coordinate> neighborhood = box.GetNeighborList();
		vector<int> boxAtoms = box.GetAtomsFromBox();
		// itereate through atoms in middle box
		for (vector<int>::iterator itAtom = boxAtoms.begin(); itAtom != boxAtoms.end(); ++itAtom) {
			//iterate through neighbouring boxes/coordinates
			for (vector<Coordinate>::iterator itNeighCoor = neighborhood.begin(); itNeighCoor != neighborhood.end(); ++itNeighCoor) {
				Box Nbox = boxlist.GetBox(*itNeighCoor);
				vector<int> Natoms = Nbox.GetAtomsFromBox();
				//itereate though atoms in box by given coordinate
				for (vector<int>::iterator itNeighAtom = Natoms.begin(); itNeighAtom != Natoms.end(); ++itNeighAtom) {
					Atom atom1 = molecule.GetAtom(*itAtom);
					Atom atom2 = molecule.GetAtom(*itNeighAtom);
					if (atom1.GetId() < atom2.GetId()) { //use '<' to avoid adding edge twice. this assumes graph is undirected.

						double dist = atom1.EuclidianPeriodicDistance(atom2, molecule.GetCubeSize());
						if (dist < rc) {
							gh.addEdge(atom1.GetId(), atom2.GetId());
						}
						else if (dist < Rc) {
							gh.addMaybeEdge(atom1.GetId(), atom2.GetId());
						}
					}
				}
			}
		}
	}
	return MaybeToGraphForces(gh, molecule);
}