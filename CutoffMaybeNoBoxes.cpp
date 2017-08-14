#include "cutoffmaybe.h"
#include "boxbuilder.h"
#include "coordinate.h"
#include "maybetograph.h"
#include <tuple>
#include <math.h>

using namespace std;
typedef tuple<double, double, double> triplet;

 Graph CutoffMaybeNoBoxes(Molecule mol, double rc, double Rc)
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
			else if(atom1.EuclidianPeriodicDistance(atom2, mol.GetCubeSize()) < Rc)
			{
				gh.addMaybeEdge(atom1.GetId(), atom2.GetId());
			}
		}
	}
	return gh;
 };

 Graph CutoffWithForcesNoBoxes(Molecule mol, double rc, double Rc){
	Graph gh = CutoffMaybeNoBoxes(mol, rc, Rc);
	return MaybeToGraphForces(gh, mol);
 }

 Graph CutoffCentroidNoBoxes(Molecule mol, double rc, double Rc){
	Graph gh = CutoffMaybeNoBoxes(mol, rc, Rc);
	return MaybeToGraphCentroid(gh, mol);
 }

 Graph CutoffDoubleCentroidNoBoxes(Molecule mol, double rc, double Rc){
	 Graph gh = CutoffMaybeNoBoxes(mol, rc, Rc);
	 return MaybeToGraphDoubleCentroid(gh, mol);
 }