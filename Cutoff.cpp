#include <math.h>

#include "boxbuilder.h"
#include "coordinate.h"
#include "cutoff.h"
#include "cutoffmaybe.h"


using namespace std;

/*
	Creates and returns a graph from given molecule using cutoff criteria.
	This algorithm uses boxes of size r_c to avoid computing all the distances.
*/
Graph Cutoff(Molecule molecule, double rc)
{
  return CutoffMaybe(molecule, rc, rc);
}