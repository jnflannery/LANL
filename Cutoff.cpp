#include "cutoff.h"

/*
	Creates and returns a graph from given molecule using cutoff criteria.
	This algorithm uses boxes of size r_c to avoid computing all the distances.
*/
Graph Cutoff(Molecule molecule, double rc)
{
  return CutoffMaybe(molecule, rc, rc);
}