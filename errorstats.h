#pragma once
#include <vector>
class ErrorStats
{
private:
	double avgPercentMismatched;
	double percentWrongVertices;
	int mostFrequentlyWrongAtom;
	double avgNumMismatched;
	double percentWrongForMostFrequentlyWrong;
	std::vector<int> countsMismatched;
public:
	std::vector< std::vector <int> > mismatchedAtoms;
	void setPercentWrongVertices();
	void setMostFrequentlyWrongAtom();
	void setAvgNumMismatched();
	void setAll();
	void setCountsMismatched();
	void initializeWithSize(int);
	double getPercentWrongVertices();
	int getMostFrequentlyWrongAtom();
	double getAvgNumMismatched();
	double getAvgPercentMismatched();
	std::vector<int> getCountsMismatched();
	double getPercentWrongForMostFrequentlyWrong();
};