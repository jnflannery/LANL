#pragma once
#include "shadow.h"
#include "boxbuilder.h"
#include "coordinate.h"
#include <tuple> 
#include <math.h>
#include "maybetographcentroid.h"

using namespace std;
typedef tuple<double, double, double> triplet;

Graph Shadow(Molecule molecule, double rc, double S)
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
				//itereate through potential neighboring atoms in box by given coordinate
				for (vector<int>::iterator itNeighAtom = Natoms.begin(); itNeighAtom != Natoms.end(); ++itNeighAtom) {
					Atom atom = molecule.GetAtom(*itAtom);
					Atom pot_neigh = molecule.GetAtom(*itNeighAtom);
					if (atom.GetId() == pot_neigh.GetId()) continue;
					if (atom.EuclidianPeriodicDistance(pot_neigh, molecule.GetCubeSize()) >= rc) continue;
					// check if any other atom shadows pot_neigh
					bool shadowed = false;
					for (vector<int>::iterator it_oth_atom = Natoms.begin(); it_oth_atom != Natoms.end(); ++it_oth_atom) {
						Atom oth_atom = molecule.GetAtom(*it_oth_atom);
						if (oth_atom.GetId() == atom.GetId() || oth_atom.GetId() == pot_neigh.GetId()) continue;
						if (shadows(atom, pot_neigh, oth_atom, S, molecule)) {
							shadowed = true;
							break;
						}
					}
					if (!shadowed) {
						gh.addEdge(atom.GetId(), pot_neigh.GetId());
						++edgecount;

					}
				}
			}
		}
	}
	return gh;
}



bool shadows(Atom atom, Atom pot_neigh, Atom oth_atom, double S, Molecule molecule) {
	triplet a = atom.VectorTo(oth_atom, molecule.GetCubeSize());
	triplet b = atom.VectorTo(pot_neigh, molecule.GetCubeSize());

	if (size(a) > size(b)) return false;

	double theta = acos(dot_product(a, b) / (size(a)*size(b)));
	double alpha = asin((S / 2.0) / size(a));
	double beta = asin((S / 2.0) / size(b));

	if (alpha + beta > theta) return true;
	return false;
}