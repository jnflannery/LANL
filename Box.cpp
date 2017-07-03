#include "box.h"
#include <vector>

// declare a box at a given x, y, and z index
Box::Box(int newXnumber, int newYnumber, int newZnumber)
{
	xnumber = newXnumber;
	ynumber = newYnumber;
	znumber = newZnumber;
}

Box::Box()
{
	xnumber = 0;
	ynumber = 0;
	znumber = 0;
}

// add an atom to a box given its ID
void Box::InsertAtom(int atomID)
{
	atomlist.push_back(atomID);
}

// find the x index of a box
int Box::GetIDx()
{
	return xnumber;
}

// find the y index of a box
int Box::GetIDy()
{
	return ynumber;
}

// find the z index of a box
int Box::GetIDz()
{
	return znumber;
}

// print out the IDs of the atoms in a given box
std::vector<int> Box::GetAtomsFromBox()
{
	return atomlist;
}

// print out the number of atoms in a box
int Box::GetNumberOfAtoms()
{
	return atomlist.size();
}

std::vector<Coordinate> Box::GetNeighborList()
{
	return neighbors;
}

void Box::AddNeighbor(Coordinate NeighborCoord)
{
	neighbors.push_back(NeighborCoord);
}