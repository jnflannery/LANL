#pragma once
class Atom
{
private:
		double x;
		double y;
		double z;
		int id;
public:
	Atom(double newX, double newY, double newZ, int newId);
	Atom();
	void SetX(double);
	void SetY(double);
	void SetZ(double);
	void SetId(int);
	double EuclidianDistance(Atom);
	double PeriodicDiffX(Atom atom, double periodicBoundary);
	double PeriodicDiffY(Atom atom, double periodicBoundary);
	double PeriodicDiffZ(Atom atom, double periodicBoundary);
	double EuclidianPeriodicDistance(Atom atom, double periodicBoundary);
	double GetX();
	double GetY();
	double GetZ();
	int GetId();
	triplet VectorTo(Atom,  double periodicBoundary);
};

double dot_product(triplet a, triplet b);
double size(triplet a);
triplet Add(triplet, triplet);