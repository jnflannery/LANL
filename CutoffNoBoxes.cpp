#include "cutoffnoboxes.h"
#include "boxbuilder.h"
#include "coordinate.h"
#include <math.h>

using namespace std;

Graph CutoffNoBoxes(Molecule mol, double rc)
{
	int size = mol.GetNumberOfAtoms();

	// create graph object
	Graph gh(size);

	for(int i = 1; i <= mol.GetNumberOfAtoms(); i++)
	{
		Atom atom1 = mol.GetAtom(i);
		for(int j = i; j <= mol.GetNumberOfAtoms(); j++)
		{
			Atom atom2 = mol.GetAtom(j);
			if(atom1.EuclidianPeriodicDistance(atom2, mol.GetCubeSize()) < rc)
			{
				gh.addEdge(atom1.GetId(), atom2.GetId()); 
			}
		}
	}
	return gh;
};