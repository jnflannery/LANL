#include "molecule.h"
//declares and empty molecule
Molecule::Molecule()
{
	std::vector<Atom> atoms;
	timestep = 0;
	cubeSize = 0;
}
/*
declare a molecule where you know how many atoms there will be
useful for having atoms in order such that if you want atom with 
number 522, you can later say "molecule.GetAtom(521) to get that
atom specifically
*/
Molecule::Molecule(int numberOfAtoms) {
	std::vector<Atom> atomsWithSize (numberOfAtoms);
	atoms = atomsWithSize;
	cubeSize = 0;
	timestep = 0;
}
//set or get the size of the cube
void Molecule::setCubeSize(double newSize) {
	cubeSize = newSize;
}
double Molecule::getCubeSize() {
	return cubeSize;
}
//set or get the timestep
void Molecule::SetTimestep(int newX) {
	timestep = newX;
}
int Molecule::GetTimestep() {
	return timestep;
}
//add an atom without specifying where it should go, will just be put at the end of a list of atoms
void Molecule::AddAtom(Atom newAtom) {
	atoms.push_back(newAtom);
}
//get atom at a certain index
Atom Molecule::GetAtom(int i) {
	return atoms.at(i);
}
//put atom into certain index
void Molecule::SetAtomWithIndex(Atom atom, int i) {
	atoms.at(i) = atom;
}
std::vector<Atom> Molecule::GetAtomVector() {
	return atoms;
}
int Molecule::GetNumberOfAtoms() {
	return atoms.size();
}
