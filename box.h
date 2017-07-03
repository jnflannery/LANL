#pragma once
#include<vector>

struct Coordinate {
	int x;
	int y;
	int z;
};

class Box {
private:
	std::vector<int> atomlist;
	int xnumber;
	int ynumber;
	int znumber;
	std::vector<Coordinate> neighbors;

public:
	Box(int newXnumber, int newYnumber, int newZnumber);
	Box();
	void InsertAtom(int atomID);
	std::vector<int> GetAtomsFromBox();
	int GetNumberOfAtoms();
	int GetIDx();
	int GetIDy();
	int GetIDz();
	std::vector<Coordinate> GetNeighborList();
	void AddNeighbor(Coordinate);
};