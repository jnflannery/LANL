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

double Atom::PeriodicDistanceX(Atom atom, double periodicBoundary){
	return std::min(abs(x - atom.GetX()), abs(periodicBoundary - abs(x - atom.GetX())));
}
double Atom::PeriodicDistanceY(Atom atom, double periodicBoundary){
	return std::min(abs(y - atom.GetY()), abs(periodicBoundary - abs(y - atom.GetY())));
}
double Atom::PeriodicDistanceZ(Atom atom, double periodicBoundary){
	return std::min(abs(z - atom.GetZ()), abs(periodicBoundary - abs(z - atom.GetZ())));
}

//does euclidian distance except it takes into account things that are close to the periodic boundaries
double Atom::EuclidianPeriodicDistance(Atom atom, double periodicBoundary) {
	double xdif = PeriodicDistanceX(atom, periodicBoundary);
	double ydif = PeriodicDistanceY(atom, periodicBoundary);
	double zdif = PeriodicDistanceZ(atom, periodicBoundary);
	return std::sqrt(xdif*xdif + ydif*ydif + zdif*zdif);
}