#pragma once
class Atom
{
private:
		double x=0;
		double y=0;
		double z=0;
		int atomicMass = 0;
public:
	Atom(float newX, float newY, float newZ, int newMass);
	Atom();
	void SetX(double);
	void SetY(double);
	void SetZ(double);
	void SetAtomicMass(int);
	double GetX();
	double GetY();
	double GetZ();
	int GetAtomicMass();
};