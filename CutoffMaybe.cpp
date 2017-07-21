#include "cutoffmaybe.h"
#include "boxbuilder.h"
#include "coordinate.h"
#include <tuple>
#include <math.h>

using namespace std;
typedef tuple<double, double, double> triplet;

 Graph CutoffMaybe(Molecule molecule, double rc, double Rc)
{
	vector<Atom> atoms = molecule.GetAtomVector();
	int size = molecule.GetNumberOfAtoms();

	// create graph object
	MaybeGraph gh(size);

	// build boxes
	BoxBuilder myBoxBuilder = BoxBuilder(rc);
	Boxlist boxlist = myBoxBuilder.BuildBoxes(molecule, rc);
	// check pairs of vertices, connect if within cutoff distance	
	vector<Box> boxes = boxlist.GetAllBoxes();
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
						if (dist < rc){
							gh.addEdge(atom1.GetId(), atom2.GetId());
						}
						else if (dist < Rc){
							gh.addMaybeEdge(atom1.GetId(), atom2.GetId());
						}
					}
				}
			}
		}
	}
	return MaybeToGraphCentroid(gh, molecule);
}

 Graph MaybeToGraphCentroid(MaybeGraph gh, Molecule mol){
	for (auto V : gh.getAllVertices()){
		
		triplet center(0, 0, 0);
		double weight = V.neighbors.size();
		for (auto N : V.neighbors){
			//add N-V to C
			Atom Vatom = mol.GetAtom(V.id-1);
			Atom Natom = mol.GetAtom(N-1);
			triplet VtoN = Vatom.VectorTo(Natom, mol.GetCubeSize());
			center = add(center, VtoN);
		}
		for (auto N : V.maybeNeighbors){
			Atom Vatom = mol.GetAtom(V.id);
			Atom Natom = mol.GetAtom(N);
			triplet VtoN = Vatom.VectorTo(Natom, mol.GetCubeSize());
			//check if adding moves cetre closer to V
			triplet	newcenter = mult(weight/(weight+1.0), add(center, VtoN));
			//addedge if so
			if (size(newcenter) > size(center)) gh.addEdge(V.id, N);
		}
	}
	auto maybes = gh.getAllVertices();
	vector<Vertex> verts;
	for (auto m : maybes){
		verts.push_back(m.MaybeToVertex());
	}
	Graph graph = Graph(verts);
	return graph;
 }