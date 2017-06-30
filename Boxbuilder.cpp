#include "boxbuilder.h"
#include "boxlist.h"
#include <iostream>
#include "math.h"

BoxBuilder::BoxBuilder(double newcutoff)
{
	cutoff = newcutoff;
}

Boxlist BoxBuilder::BuildBoxes (Molecule mol, double cutoff)
{
	double domainSize = mol.getCubeSize();
	int N = floor(domainSize/cutoff);
	double BoxDim = domainSize/N;
	double x;
	double y;
	int TotalAtoms = 0;
	double z; 

	Boxlist boxlist(N); 
	for(int k=0; k<N; k++)
	{
		for(int j=0; j<N; j++)
		{
			for(int i=0; i<N; i++)
			{
				std::cout << "here";
				 Box newbox = Box(i, j, k);
				 boxlist.AddBox(newbox);
			}
		}
	}

	for (int n=0; n< mol.GetNumberOfAtoms(); n++)
	{
		std::cout << "We got here: ";
		int xindex = floor(mol.GetAtom(n).GetX()/BoxDim);
		int yindex = floor(mol.GetAtom(n).GetY()/BoxDim);
		int zindex = floor(mol.GetAtom(n).GetZ()/BoxDim);
		// std::cout << "x = " << mol.GetAtom(n).GetX() << "\n";
		// std::cout << "Domain Size = " << domainSize << "\n";
		// std::cout << "Atom " << n << ": " << "x = " << xindex << " y = " << yindex << " z = " << zindex << "\n";
		Box fillbox = boxlist.GetBox(xindex, yindex, zindex);
		fillbox.InsertAtom(n);
		boxlist.AddBox(fillbox); 
	}

	// this loop below is just for debugging to check how many atoms are in each box and that all atoms are put in boxes. 
	for(int k=0; k<N; k++)
	{
		for(int j=0; j<N; j++)
		{
			for(int i=0; i<N; i++)
			{
				TotalAtoms = TotalAtoms + boxlist.GetBox(i, j, k).GetNumberOfAtoms();
				 std::cout << i << " " << j << " " << k << " Number of Atoms: " << boxlist.GetBox(i, j, k).GetNumberOfAtoms() << "\n";
			}
		}
	}
	std::cout << TotalAtoms;
	return boxlist;
};

