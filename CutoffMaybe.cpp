#include <math.h>

#include "boxbuilder.h"
#include "coordinate.h"
#include "cutoffmaybe.h"
#include "maybetograph.h"

using namespace std;

/*
	Creates and returns a graph from given molecule using cutoff criteria with 
	potential neighbors.
	This algorithm uses boxes of size R_c to avoid computing all the distances.
*/

 Graph CutoffMaybe(Molecule molecule, double rc, double Rc)
{
	vector<Atom> atoms = molecule.GetAtomVector();
	int size = molecule.GetNumberOfAtoms();

	// create graph object
	Graph gh(size);

	// build boxes
	BoxBuilder myBoxBuilder = BoxBuilder(Rc);
	Boxlist boxlist = myBoxBuilder.BuildBoxes(molecule, Rc);	
	vector<Box> boxes = boxlist.GetAllBoxes();

	//iterate through boxes
	for (vector<Box>::iterator itBox = boxes.begin(); itBox != boxes.end(); ++itBox){
		Box box = FindNeighbours(*itBox, boxlist.NumberOfBoxes());
		vector<Coordinate> neighborhood = box.GetNeighborList();
		vector<int> boxAtoms = box.GetAtomsFromBox();
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
						double dist = atom1.EuclidianPeriodicDistance(atom2, molecule.GetCubeSize());
						//add edge if the atoms are within r_c cutoff
						if (dist < rc){
							gh.addEdge(atom1.GetId(), atom2.GetId());
						}
						//if not, add as potential neighbor if within R_c cutoff
						else if (dist < Rc){
							gh.addMaybeEdge(atom1.GetId(), atom2.GetId());
						}
					}
				}
			}
		}
	}
	return gh;
}


 /*
	Three algorithms using CutoffMaybe to create a graph and some further 
	evaluation of potential neighbors.
 */
 Graph CutoffWithForces(Molecule mol, double rc, double Rc){
	Graph gh = CutoffMaybe(mol, rc, Rc);
	return MaybeToGraphForces(gh, mol);
 }

 Graph CutoffCentroid(Molecule mol, double rc, double Rc){
	Graph gh = CutoffMaybe(mol, rc, Rc);
	return MaybeToGraphCentroid(gh, mol);
 }

 Graph CutoffDoubleCentroid(Molecule mol, double rc, double Rc){
	 Graph gh = CutoffMaybe(mol, rc, Rc);
	 return MaybeToGraphDoubleCentroid(gh, mol);
 }