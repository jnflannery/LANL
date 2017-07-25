#pragma once

#include <tuple>
using namespace std;
typedef tuple<double, double, double> triplet;

class Atom
{
private:
	double x;
	double y;
	double z;
	double fx;
	double fy;
	double fz;
	int id;
public:
	Atom(double newX, double newY, double newZ, int newId);
	Atom();
	void SetX(double);
	void SetY(double);
	void SetZ(double);
	void SetFx(double);
	void SetFy(double);
	void SetFz(double);
	void SetId(int);
	double getPeriodicDistanceOneD(double, double, double);
	double EuclidianDistance(Atom);
	double PeriodicDiffX(Atom atom, double periodicBoundary);
	double PeriodicDiffY(Atom atom, double periodicBoundary);
	double PeriodicDiffZ(Atom atom, double periodicBoundary);
	double EuclidianPeriodicDistance(Atom atom, double periodicBoundary);
	double GetX();
	double GetY();
	double GetZ();
	double Atom::GetFx();
	double Atom::GetFy();
	double Atom::GetFz();
	int GetId();
	triplet VectorTo(Atom,  double periodicBoundary);
};
