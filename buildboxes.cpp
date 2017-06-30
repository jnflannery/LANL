#include "box.h"
#include "molecule.h"
#include "reader.h"
#include <iostream>
#include <vector>

double xlength;
double ylength;
double zlength;
double cutoff;

int Nx = floor(xlength/cutoff);
int Ny = floor(xlength/cutoff);
int NZ = floor(zlength/cutoff);

double x;
double y;
double z; 

for(int i=0; i<Nx; i++)
{
	x = i*xsize;
	for(int j=0; j<Ny; j++)
	{
		y = j*ysize;
		for(int k=0; k<Nz; k++)
		{
			z = k*zsize; 
			Box(i, j, k, x, y, z, xsize, ysize, zsize);
		}
	}
}

for(int n=1; n<NAtoms; n++)
{
	int xindex = floor(Molecule.GetAtom(n).GetX()/xsize);
	int yindex = floor(Molecule.GetAtom(n).GetY()/ysize);
	int zindex = floor(Molecule.GetAtom(n).GetZ()/zsize);
	Box(i,j,k).InsertAtom(n); 
}