#include "molecule.h"
Molecule::Molecule()
{
	std::vector<Atom> atoms;
	timestep = 0;
}
void Molecule::SetTimestep(int newX) {
	timestep = newX;
}
int Molecule::GetTimestep() {
	return timestep;
}
void Molecule::AddAtom(Atom newAtom) {
	atoms.push_back(newAtom);
}
Atom Molecule::GetAtom(int i) {
	return atoms.at(i);
}
std::vector<Atom> Molecule::GetMolecule() {
	return atoms;
}
int Molecule::GetNumberOfAtoms() {
	return atoms.size();
}