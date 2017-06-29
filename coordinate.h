#include <vector>

using namespace std;

struct Coordinate{
	int x;
	int y;
	int z;
};

void PrintCoordinate(Coordinate c);

vector<Coordinate> FindNeighbours(Coordinate coordinate, int size);
