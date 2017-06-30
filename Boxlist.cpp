#include "boxlist.h"
#include <vector>
#include <iostream>

void Boxlist::AddBox(Box newBox)
{
	// std::cout << "5";
	int IDx = newBox.GetIDx();
	int IDy = newBox.GetIDy();
	int IDz = newBox.GetIDz();
	// std::cout << "6";
	int VectorIndex = IDx+N*IDy+N*N*IDz;
	// std::cout << VectorIndex;
	boxes.at(VectorIndex) = newBox; 
	// std::cout << "8";
}

Box Boxlist::GetBox(int IDx, int IDy, int IDz)
{
	int VectorIndex = IDx+N*IDy+N*N*IDz;
	Box FoundBox = boxes.at(VectorIndex);
	return FoundBox;
}

Boxlist::Boxlist(int newN)
{
   N = newN;
   std::vector<Box> boxesWithSize(newN*newN*newN);
   boxes = boxesWithSize;
}

Boxlist::Boxlist()
{
	N = 0;
}

int Boxlist::FindBoxLocationX(int VectorIndex)
{
	int Xcoordinate = VectorIndex % N;
	return Xcoordinate; 
}

int Boxlist::FindBoxLocationY(int VectorIndex)
{
	int Ycoordinate = floor(VectorIndex/N);
	return Ycoordinate;
}

int Boxlist::FindBoxLocationZ(int VectorIndex)
{
	int Zcoordinate = floor(VectorIndex/(N*N));
	return Zcoordinate;
}