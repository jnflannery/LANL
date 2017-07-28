using namespace std;
#include "soigtol.h"

// function to find the radius for the sphere of influence for each atom
vector<AtomWithRadius> SoigTol::ComputeSpheresSoigTol(Molecule mol, double periodicBoundary)
{
	vector<AtomWithRadius> sphereList;
	for (int i = 1; i <= mol.GetNumberOfAtoms(); i++)
	{
		AtomWithRadius sphere; // declare the sphere of influence as an AtomWithRadius for atom i
		sphere.id = mol.GetAtom(i).GetId(); // assign atom i's ID to the sphere
											// cout << "3 \n";
		vector<double> distances;
		for (int j = 1; j <= mol.GetNumberOfAtoms(); j++)
		{
			if (j != i)
			{
				double distance = mol.GetAtom(i).EuclidianPeriodicDistance(mol.GetAtom(j), periodicBoundary); // compute distance between atoms i and j taking into account periodic BCs
				distances.push_back(distance); // add to vector of distances between atom i and atoms j
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
		sphere.radius = *min_element(distances.begin(), distances.end()); // find the minimum distance from the list of distances between atom i and atoms j 
		sphereList.push_back(sphere); // add the sphere of influence for atom i to the spherelist
									  // cout << sphere.radius << "\n";
									  // cout << "sphere size = " << sphereList.size() << "\n";
									  // cout << mol.GetNumberOfAtoms() << "\n";
	}
	return sphereList;
}

// given the list of spheres, find the neighbors for each atom
int SoigTol::FindNeighborsSoigTol(Atom atomIN, Molecule mol, vector<AtomWithRadius> sphereList, double periodicBoundary, double tolerance, Graph& g)
{
	int atomID = atomIN.GetId();
	// cout << "atomIN id = " << atomID << "\n";
	double CenterAtomRadius = sphereList.at(atomID - 1).radius; // retrieve the radius of the input atom from the sphere list
																// cout << CenterAtomRadius << "\n";
																// cout << "1 \n";
																// cout << "Number of atoms = " << mol.GetNumberOfAtoms();
	for (int i = atomID; i <= mol.GetNumberOfAtoms(); i++)
	{
		if (i != atomID)
		{
			// int thingtooutput = mol.GetAtom(i).GetId();
			// cout << "i = " << i << "\n";
			// cout << thingtooutput << "\n";
			double OtherAtomRadius = sphereList.at(i - 1).radius; // retrieve the radius for the neighbor candidate from the spherelist
			double DistanceBetweenAtoms = atomIN.EuclidianPeriodicDistance(mol.GetAtom(i), periodicBoundary);
			// cout << "Inner i = " << i << "\n";
			if (DistanceBetweenAtoms < CenterAtomRadius + OtherAtomRadius + tolerance)
			{
				g.addEdge(atomIN.GetId(), mol.GetAtom(i).GetId());
				// cout << "added edge \n";
			}

		}
	}
	return 0;
}

// iterate through atoms to create the full graph
int SoigTol::CreateGraphSoigTol(Molecule mol, vector<AtomWithRadius> sphereList, double periodicBoundary, double tolerance, Graph& g)
{
	for (int i = 1; i <= mol.GetNumberOfAtoms(); i++)
	{
		int output = FindNeighborsSoigTol(mol.GetAtom(i), mol, sphereList, periodicBoundary, tolerance, g);
		// cout << "Outer i = " << i << "\n";
	}
	return 0;
}