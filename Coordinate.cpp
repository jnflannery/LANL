#include "coordinate.h"

#include <iostream>
#include <vector>

using namespace std;

void PrintCoordinate(Coordinate c){
	cout << c.x << " " << c.y << " "<< c.z << endl;
}

vector<Coordinate> FindNeighbours(Coordinate coordinate, int size) {
	// create empty list of neighbours
	vector<Coordinate> neighbours;
	for (int i=-1; i<2; ++i) {
		for (int j=-1; j<2; ++j) {
			for (int k=-1; k<2; ++k) {
				// add neighbour to the list
				Coordinate c = {(coordinate.x+i+8) % 8, (coordinate.y+j+8) % 8, (coordinate.z+k+8) % 8};
				neighbours.push_back(c);
			}
		}
	}

	return neighbours;
} 
