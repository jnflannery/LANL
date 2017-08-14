#pragma once
#include "distribution.h"


// Compute all pairwise distances between atoms in the input molecule. To be used for choosing cutoff value.
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

/*
vector<vector<int>> CompareDistancesBetweenAllAtoms(Molecule molA, Molecule molB){
	vector<vector<int>> MissedPairs;
	double tol = 0.01;
	bool same = true;
	for(int i = 1; i <= molA.GetNumberOfAtoms(); i++)
	{
		vector<int> MissedAtoms;
		Atom atomA1 = molA.GetAtom(i);
		Atom atomB1 = molB.GetAtom(i);
		for(int j = 1; j<= molA.GetNumberOfAtoms(); j++)
		{
			if(j != i)
			{
				Atom atomA2 = molA.GetAtom(j);
				Atom atomB2 = molB.GetAtom(j);
				double distanceA = atomA1.EuclidianPeriodicDistance(atomA2, molA.GetCubeSize());
				double distanceB = atomB1.EuclidianPeriodicDistance(atomB2, molB.GetCubeSize());
				if(abs(distanceA-distanceB)/distanceA > tol)
				{
					MissedAtoms.push_back(j);
				}
			}
		}
		MissedPairs.push_back(MissedAtoms);
	}
	return MissedPairs;
};
*/