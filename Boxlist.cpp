#include "boxlist.h"


// the boxlist class keeps track of all of the boxes and their x, y, z indices in a vector of boxes

// add a box to a boxlist
void Boxlist::AddBox(Box newBox)
{
	// std::cout << "5";
	int IDx = newBox.GetIDx();
	int IDy = newBox.GetIDy();
	int IDz = newBox.GetIDz(); // get x, y, z, coordinates of the box
							   // std::cout << "6";
	int VectorIndex = IDx + N*IDy + N*N*IDz; // convert x, y, z, coordinates to 1D vector index
											 // std::cout << VectorIndex;
	boxes.at(VectorIndex) = newBox; // insert the box into the boxes vector at the proper vector index
									// std::cout << "8";
}

// retrieve a box with a given x, y, z index from the vector of boxes
Box Boxlist::GetBox(int IDx, int IDy, int IDz)
{
	int VectorIndex = IDx + N*IDy + N*N*IDz; // convert x, y, z coordinates into 1D vector index
	Box FoundBox = boxes.at(VectorIndex); // find the box with this vector index
	return FoundBox;
}
Box Boxlist::GetBox(Coordinate coordinate)
{
	int IDx = coordinate.x;
	int IDy = coordinate.y;
	int IDz = coordinate.z;
	int VectorIndex = IDx + N*IDy + N*N*IDz; // convert x, y, z coordinates into 1D vector index
	Box FoundBox = boxes.at(VectorIndex); // find the box with this vector index
	return FoundBox;
}
vector<Box> Boxlist::GetAllBoxes(){
	return boxes;
}

// declare a Boxlist
Boxlist::Boxlist(int newN)
{
	N = newN; //N is the number of boxes in each dimension
	std::vector<Box> boxesWithSize(newN*newN*newN); // create a vector of boxes with N^3 total entries
	boxes = boxesWithSize;
}

Boxlist::Boxlist()
{
	N = 0;
}

// find the number of boxes in each dimension
int Boxlist::NumberOfBoxes()
{
	return N;
}

//find the x, y, z, indices of a box given its vector index
int Boxlist::FindBoxLocationX(int VectorIndex)
{
	int Xcoordinate = VectorIndex % N;
	return Xcoordinate;
}

int Boxlist::FindBoxLocationY(int VectorIndex)
{
	int Ycoordinate = floor(VectorIndex / N);
	return Ycoordinate;
}

int Boxlist::FindBoxLocationZ(int VectorIndex)
{
	int Zcoordinate = floor(VectorIndex / (N*N));
	return Zcoordinate;
}

Box Boxlist::FindBoxWithAtom(Atom atomIN)
{
	int xindex = floor(atomIN.GetX() / BoxSize);
	int yindex = floor(atomIN.GetY() / BoxSize);
	int zindex = floor(atomIN.GetZ() / BoxSize);
	return GetBox(xindex, yindex, zindex);
}

void Boxlist::AssignBoxSize(double BoxSizeIN)
{
	BoxSize = BoxSizeIN;
}

double Boxlist::GetBoxSize()
{
	return BoxSize;
}
std::vector<Atom> Boxlist::getAtomVectorOfNeighborCandidates(Box box, int boxesPerSide, Molecule molecule) {
	std::vector<Atom> atomsInBoxes=std::vector<Atom>();
	std::vector<Box> boxNeighbors = std::vector<Box>();
	box = FindNeighbours(box, boxesPerSide);
	vector<Atom> largeBoxAtoms = box.getVectorOfAtomsFromBox(molecule);

	vector<Coordinate> neighborBoxes = box.GetNeighborList();
	for (int i = 0;i < neighborBoxes.size();i++) {
		boxNeighbors.push_back(GetBox(neighborBoxes.at(i).x, neighborBoxes.at(i).y, neighborBoxes.at(i).z));
	}
	for (int i = 0;i < boxNeighbors.size();i++) {
		for (int j = 0;j < boxNeighbors.at(i).GetNumberOfAtoms();j++) {
			atomsInBoxes.push_back(molecule.GetAtom(boxNeighbors.at(i).GetAtomsFromBox().at(j)));
		}
	}
	return atomsInBoxes;
}