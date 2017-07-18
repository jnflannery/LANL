#pragma once
#pragma once
class Atom
{
private:
	double x;
	double y;
	double z;
	double Fx;
	double Fy;
	double Fz;
	int id;
public:
	Atom(double newX, double newY, double newZ, int newId);
	Atom();
	void SetX(double);
	void SetY(double);
	void SetZ(double);
	void SetId(int);
	double EuclidianDistance(Atom);
	double EuclidianPeriodicDistance(Atom atom, double periodicBoundary);
	double GetX();
	double GetY();
	double GetZ();
	double GetFx();
	double GetFy();
	double GetFz();
	void SetFx(double);
	void SetFy(double);
	void SetFz(double);
	int GetId();
};