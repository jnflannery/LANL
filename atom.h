#pragma once

#include <tuple>
using namespace std;
typedef tuple<double, double, double> triplet;

class Atom
{
private:
	//xyz coordinates
	double x;
	double y;
	double z;
	//force coordinates
	double fx;
	double fy;
	double fz;
	int id;
public:
	//functions to create atoms and set their values
	Atom(double newX, double newY, double newZ, int newId);
	Atom();
	void SetX(double);
	void SetY(double);
	void SetZ(double);
	void SetFx(double);
	void SetFy(double);
	void SetFz(double);
	void SetId(int);
	//get values from the atom
	double GetX();
	double GetY();
	double GetZ();
	double Atom::GetFx();
	double Atom::GetFy();
	double Atom::GetFz();
	int GetId();
	//functions to get periodicDistance for a single dimension
	double getPeriodicDistanceOneDimension(double, double, double);
	double PeriodicDiffX(Atom atom, double periodicBoundary);
	double PeriodicDiffY(Atom atom, double periodicBoundary);
	double PeriodicDiffZ(Atom atom, double periodicBoundary);
	//get euclidean dist between this atom and another, potentially with a periodic boundary too
	double EuclidianDistance(Atom);
	double EuclidianPeriodicDistance(Atom atom, double periodicBoundary);
	triplet VectorTo(Atom,  double periodicBoundary);
};