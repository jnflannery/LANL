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
	int timeStep;
	void setPercentWrongVertices();
	void setMostFrequentlyWrongAtom();
	void setAvgNumMismatched();
	void setCountsMismatched();
public:
	//mismatched atoms must be filled before calling setAll, which sets the error statistics
	//mismatched atoms is filled with the 
	std::vector< std::vector <int> > mismatchedAtoms;
	void setAll();
	void initializeWithSize(int);

	//get specific error statistics
	double getPercentWrongVertices();
	int getMostFrequentlyWrongAtom();

	//what percentage of atoms added the most frequently wrong atom?
	double getPercentWrongForMostFrequentlyWrong();

	//average number mismatched in a mismatched atom
	double getAvgNumMismatched();

	//last statistic as a percentage from total edges
	double getAvgPercentMismatched();

	//get the number of mismatched atoms per vertex, as a vector
	std::vector<int> getCountsMismatched();



	//can hold a timestep variable
	void setTimestep(int);
	int getTimestep();
};