#pragma once
class Atom
{
private:
		double x;
		double y;
		double z;
		int atomicMass;
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