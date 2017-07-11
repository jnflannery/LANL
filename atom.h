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
	double PeriodicDistanceX(Atom atom, double periodicBoundary);
	double PeriodicDistanceY(Atom atom, double periodicBoundary);
	double PeriodicDistanceZ(Atom atom, double periodicBoundary);
	double EuclidianPeriodicDistance(Atom atom, double periodicBoundary);
	double GetX();
	double GetY();
	double GetZ();
	int GetId();
};