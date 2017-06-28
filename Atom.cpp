#include "atom.h"
#include <cmath>
////
double Atom::GetX() {
	return x;
}
double Atom::GetY() {
	return y;
}
double Atom::GetZ() {
	return z;
}
int Atom::GetAtomicMass() {
	return atomicMass;
}
void Atom::SetX(double newX) {
	x = newX;
}
void Atom::SetY(double newY) {
	y = newY;
}
void Atom::SetZ(double newZ) {
	z = newZ;      
}
void Atom::SetAtomicMass(int newAtomicMass) {
	atomicMass = newAtomicMass;
}
Atom::Atom() {
	x = 0;
	y = 0;
	z = 0;
	atomicMass = 0;
}
Atom::Atom(float newX, float newY, float newZ, int newMass) {
	x = newX;
	y = newY;
	z = newZ;
	atomicMass = newMass;
}
float Atom::EuclidianDistance(Atom atom) {
	float xdif = (x - atom.GetX())*(x - atom.GetX());
	float ydif = (y - atom.GetY())*(y - atom.GetY());
	float zdif = (z - atom.GetZ())*(z - atom.GetZ());
	return std::sqrt(xdif + ydif + zdif);
}