#pragma once
class Atom
{
private:
		float x;
		float y;
		float z;
		int id;
public:
	Atom(float newX, float newY, float newZ, int newId);
	Atom();
	void SetX(double);
	void SetY(double);
	void SetZ(double);
	void SetId(int);
	float EuclidianDistance(Atom);
	double GetX();
	double GetY();
	double GetZ();
	int GetId();
};