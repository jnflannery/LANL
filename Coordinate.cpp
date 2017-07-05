
#include "coordinate.h"

#include <iostream>
#include <vector>

using namespace std;

void PrintCoordinate(Coordinate c) {
	cout << c.x << " " << c.y << " " << c.z << "\n";
}


// find the neighbors of a given box. The second argument is the number of boxes on each side of the domain. 
Box FindNeighbours(Box BoxIN, int size) {
	// create empty list of neighbours
	for (int i = -1; i<2; ++i) {
		for (int j = -1; j<2; ++j) {
			for (int k = -1; k<2; ++k) {
				// add neighbour to the list
				Coordinate c = { (BoxIN.GetIDx() + i + size) % size, (BoxIN.GetIDy() + j + size) % size, (BoxIN.GetIDz() + k + size) % size };
				BoxIN.AddNeighbor(c);
			}
		}
	}
	return BoxIN;
}