#include "atom.h"
#include <cmath>
#include <algorithm>

using namespace std;


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
void Atom::SetFx(double newFx)
{
	fx = newFx;
}
void Atom::SetFy(double newFY)
{
	fy = newFY;
}
void Atom::SetFz(double newFz)
{
	fz = newFz;
}
double Atom::GetFx()
{
	return fx;
}
double Atom::GetFy()
{
	return fy;
}
double Atom::GetFz()
{
	return fz;
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
double Atom::PeriodicDiffX(Atom atom, double periodicBoundary){
	return (abs(x - atom.GetX()) < abs(periodicBoundary - abs(x - atom.GetX()))) ? (atom.GetX()-x) : ((x<atom.GetX() ? atom.GetX()-periodicBoundary-x : atom.GetX()+periodicBoundary-x));
}
double Atom::PeriodicDiffY(Atom atom, double periodicBoundary){
	return (abs(y - atom.GetY()) < abs(periodicBoundary - abs(y - atom.GetY()))) ? (atom.GetY()-y) : ((y<atom.GetY() ? atom.GetY()-periodicBoundary-y : atom.GetY()+periodicBoundary-y));
}
double Atom::PeriodicDiffZ(Atom atom, double periodicBoundary){
	return (abs(z - atom.GetZ()) < abs(periodicBoundary - abs(z - atom.GetZ()))) ? (atom.GetZ()-z) : ((z<atom.GetZ() ? atom.GetZ()-periodicBoundary-z : atom.GetZ()+periodicBoundary-z));
}
double Atom::getPeriodicDistanceOneDimension(double x, double y, double periodicDist)
{
	double dx = x - y;
	if (dx > periodicDist * 0.5)
		dx = dx - periodicDist;
	if (dx <= -periodicDist * 0.5)
		dx = dx + periodicDist;
	return std::abs(dx);
}
//does euclidian distance except it takes into account things that are close to the periodic boundaries
double Atom::EuclidianPeriodicDistance(Atom atom, double periodicBoundary) {
	double xdif = abs(PeriodicDiffX(atom, periodicBoundary));
	double ydif = abs(PeriodicDiffY(atom, periodicBoundary));
	double zdif = abs(PeriodicDiffZ(atom, periodicBoundary));
	return std::sqrt(xdif*xdif + ydif*ydif + zdif*zdif);
}