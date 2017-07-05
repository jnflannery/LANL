#include "boxbuilder.h"
#include "boxlist.h"
#include <iostream>
#include "math.h"

BoxBuilder::BoxBuilder(double newcutoff)
{
	cutoff = newcutoff;
}

Boxlist BoxBuilder::BuildBoxes(Molecule mol, double cutoff)
{
	double domainSize = mol.GetCubeSize(); // domainSize is the size of one of the edges of the entire periodic cube
	int BoxesPerSide = floor(domainSize / cutoff); // calculate how many boxes will fit in one edge of the entire domain
	double BoxDim = domainSize / BoxesPerSide; // define the appropriate size of one side of a box so that BoxesPerSide boxes fit in the domain
											   // double x;  
											   // double y;
											   // double z; 
	int TotalAtoms = 0; //initialize count of the number of atoms that have been placed into boxes


	Boxlist boxlist(BoxesPerSide);
	boxlist.AssignBoxSize(BoxDim);

	for (int k = 0; k<BoxesPerSide; k++)
	{
		for (int j = 0; j<BoxesPerSide; j++)
		{
			for (int i = 0; i<BoxesPerSide; i++)
			{
				// std::cout << "here"; for debugging
				Box newbox = Box(i, j, k); // create a new box with x, y, z indices i, j, and k
				boxlist.AddBox(newbox); // add this box to the boxlist
			}
		}
	}

	for (int n = 0; n< mol.GetNumberOfAtoms(); n++)
	{
		// std::cout << "We got here: ";
		int xindex = floor(mol.GetAtom(n).GetX() / BoxDim);
		int yindex = floor(mol.GetAtom(n).GetY() / BoxDim);
		int zindex = floor(mol.GetAtom(n).GetZ() / BoxDim); // find the x, y, and z indices of the box the atom should go into
															// std::cout << "x = " << mol.GetAtom(n).GetX() << "\n";
															// std::cout << "Domain Size = " << domainSize << "\n";
															// std::cout << "Atom " << n << ": " << "x = " << xindex << " y = " << yindex << " z = " << zindex << "\n";
		Box fillbox = boxlist.GetBox(xindex, yindex, zindex); // retrieve the box at the proper location from the boxlist
		fillbox.InsertAtom(n); // add the atom to this box 
		boxlist.AddBox(fillbox); // put the box back in the boxlist
	}

	// this loop below is just for debugging to check how many atoms are in each box and that all atoms are put in boxes. 
	for (int k = 0; k<BoxesPerSide; k++)
	{
		for (int j = 0; j<BoxesPerSide; j++)
		{
			for (int i = 0; i<BoxesPerSide; i++)
			{
				TotalAtoms = TotalAtoms + boxlist.GetBox(i, j, k).GetNumberOfAtoms();
				//std::cout << i << " " << j << " " << k << " Number of Atoms: " << boxlist.GetBox(i, j, k).GetNumberOfAtoms() << "\n";
			}
		}
	}
	//std::cout << TotalAtoms;
	return boxlist;
};