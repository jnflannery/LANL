#include "atom.h"
#include <cmath>
#include <algorithm>
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
int Atom::GetId() {
	return id;
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
void Atom::SetId(int newId) {
	id = newId;
}
Atom::Atom() {
	x = 0;
	y = 0;
	z = 0;
	id = 0;
}
Atom::Atom(double newX, double newY, double newZ, int newId) {
	x = newX;
	y = newY;
	z = newZ;
	id = newId;
}
double Atom::EuclidianDistance(Atom atom) {
	double xdif = (x - atom.GetX())*(x - atom.GetX());
	double ydif = (y - atom.GetY())*(y - atom.GetY());
	double zdif = (z - atom.GetZ())*(z - atom.GetZ());
	return std::sqrt(xdif + ydif + zdif);
}
//does euclidian distance except it takes into account things that are close to the periodic boundaries
double Atom::EuclidianPeriodicDistance(Atom atom, double periodicBoundary) {
	double xdif = std::min((x - atom.GetX()), (periodicBoundary - (x - atom.GetX())));
	double ydif = std::min((y - atom.GetY()), (periodicBoundary - (y - atom.GetY())));
	double zdif = std::min((z - atom.GetZ()), (periodicBoundary - (z - atom.GetZ())));
	return std::sqrt(xdif + ydif + zdif);
}