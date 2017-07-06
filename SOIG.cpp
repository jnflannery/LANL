using namespace std;
#include "soig.h"

// function to find the radius for the sphere of influence for each atom
vector<AtomWithRadius> Soig::ComputeSpheresSoig(Molecule mol, double periodicBoundary)
{
	vector<AtomWithRadius> sphereList; 
	for(int i=1; i <= mol.GetNumberOfAtoms(); i++)
	{
		AtomWithRadius sphere;
		sphere.id = mol.GetAtom(i).GetId();
		// cout << "3 \n";
		vector<double> distances; 
		for(int j=1; j <= mol.GetNumberOfAtoms(); j++)
		{
			if(j != i)
			{
				double distance = mol.GetAtom(i).EuclidianPeriodicDistance(mol.GetAtom(j), periodicBoundary);
				distances.push_back(distance);
				// cout << "i = " << i << " j = " << j;
				//if(j == 0 || distances.at(j)<distances.at(j-1))
				/* if (distances.at(j)<distances.at(j-1))
				{
					cout << "6 \n";
					sphere.radius = distances.at(j);
					cout << "7 \n";
				} */
			}
		}
		sphere.radius = *min_element(distances.begin(), distances.end());
		sphereList.push_back(sphere);
		// cout << "sphere size = " << sphereList.size() << "\n";
		// cout << mol.GetNumberOfAtoms() << "\n";
	}
	return sphereList;
}

// given the list of spheres, find the neighbors for each atom
int Soig::FindAtomNeighbors(Atom atomIN, Molecule mol, vector<AtomWithRadius> sphereList, double periodicBoundary, Graph& g)
{
	int atomID = atomIN.GetId();
	// cout << "atomIN id = " << atomID << "\n";
	double CenterAtomRadius = sphereList.at(atomID-1).radius;
	// cout << CenterAtomRadius << "\n";
	// cout << "1 \n";
	// cout << "Number of atoms = " << mol.GetNumberOfAtoms();
	for(int i=atomID; i <= mol.GetNumberOfAtoms(); i++)
	{
		if (i != atomID)
		{
			int thingtooutput = mol.GetAtom(i).GetId();
			// cout << "i = " << i << "\n";
			// cout << thingtooutput << "\n";
			double OtherAtomRadius = sphereList.at(atomID).radius;
			double DistanceBetweenAtoms = atomIN.EuclidianPeriodicDistance(mol.GetAtom(i), periodicBoundary);
			// cout << "Inner i = " << i << "\n";
			if(DistanceBetweenAtoms < CenterAtomRadius+OtherAtomRadius)
			{
				g.addEdge(atomIN.GetId(), mol.GetAtom(i).GetId());
			    cout << "added edge \n";
			}

		}
	}
	return 0;
}

// iterate through atoms to create the full graph
int Soig::CreateGraphSoig(Molecule mol, vector<AtomWithRadius> sphereList, double periodicBoundary, Graph& g)
{
	for(int i=1; i <= mol.GetNumberOfAtoms(); i++)
	{
		int output = FindAtomNeighbors(mol.GetAtom(i), mol, sphereList, periodicBoundary, g);
		cout << "Outer i = " << i << "\n";
	}
	return 0;
}