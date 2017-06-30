#include "box.h"

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

void Box::InsertAtom(int atomID)
{
	atomlist.push_back (atomID);
}

int Box::GetIDx()
{
	return xnumber;
}

int Box::GetIDy()
{
	return ynumber;
}

int Box::GetIDz()
{
	return znumber; 
}

std::vector<int> Box::GetAtomsFromBox()
{
	return atomlist;
}

int Box::GetNumberOfAtoms()
{
	return atomlist.size();
}