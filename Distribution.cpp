#pragma once
#include "distribution.h"

vector<double> GetDistancesBetweenAllAtoms(Molecule mol){
	vector<double> distances;
	for(int i = 1; i <= mol.GetNumberOfAtoms(); i++)
	{
		Atom atom1 = mol.GetAtom(i);
		for(int j = 1; j<= mol.GetNumberOfAtoms(); j++)
		{
			if(j != i)
			{
				Atom atom2 = mol.GetAtom(j);
				double distance = atom1.EuclidianPeriodicDistance(atom2, mol.GetCubeSize());
				distances.push_back(distance);
			}
		}
	}
	return distances;
};