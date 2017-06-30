#pragma once
#include<vector>

class Box {
private:
		std::vector<int> atomlist;
		int xnumber;
		int ynumber;
		int znumber;

public:
	Box(int newXnumber, int newYnumber, int newZnumber);
	Box();
	void InsertAtom(int atomID);
	std::vector<int> GetAtomsFromBox();
	int GetNumberOfAtoms();
	int GetIDx();
	int GetIDy();
	int GetIDz();
};