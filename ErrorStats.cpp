#include "errorstats.h"
using namespace std;
void ErrorStats::setPercentWrongVertices()
{
	int k = 0;
	for (int i = 0;i < mismatchedAtoms.size(); i++) {
		if (mismatchedAtoms.at(i).size() > 0)
			k++;
	}
	percentWrongVertices = (double)k / (double)mismatchedAtoms.size();
}



void ErrorStats::setMostFrequentlyWrongAtom()
{
	std::vector<int> counts = std::vector <int>(mismatchedAtoms.size()+1);
	for (int i = 0;i < counts.size(); i++) {
		counts.at(i) = 0;
	}
	for (int i = 0;i < mismatchedAtoms.size();i++) {
		for (int j = 0; j < mismatchedAtoms.at(i).size();j++) {
			counts.at(mismatchedAtoms.at(i).at(j))++;
		}
	}
	mostFrequentlyWrongAtom =-1;
	int mistakeCount = 0;
	for (int i = 1; i < counts.size(); i++) {
		if (counts.at(i) > mistakeCount) {
			mistakeCount = counts.at(i);
			mostFrequentlyWrongAtom = i;
		}
	}
	percentWrongForMostFrequentlyWrong = (double)mistakeCount / (double)(mismatchedAtoms.size());
}

void ErrorStats::setAvgNumMismatched()
{
	int k = 0;
	int count = 0;
	for (int i = 0;i < mismatchedAtoms.size();i++) {
		k += mismatchedAtoms.at(i).size();
		if (mismatchedAtoms.at(i).size() > 0) {
			count++;
		}
	}
	avgNumMismatched = (double)k / (double)(count);
}
void ErrorStats::setTimestep(int t){
	timeStep=t;
}
int ErrorStats::getTimestep(){
	return timeStep;
}
void ErrorStats::setAll()
{
	setAvgNumMismatched();
	setMostFrequentlyWrongAtom();
	setPercentWrongVertices();
}

void ErrorStats::setCountsMismatched()
{
}

void ErrorStats::initializeWithSize(int s)
{
	std::vector< std::vector<int> > newArray = std::vector< std::vector<int> >(s+1);
	mismatchedAtoms = newArray;
}

double ErrorStats::getPercentWrongVertices()
{
	return percentWrongVertices;
}

int ErrorStats::getMostFrequentlyWrongAtom()
{
	return mostFrequentlyWrongAtom;
}

double ErrorStats::getAvgNumMismatched()
{
	return avgNumMismatched;
}

double ErrorStats::getAvgPercentMismatched()
{
	return avgPercentMismatched;
}

vector<int> ErrorStats::getCountsMismatched()
{
	return countsMismatched;
}

double ErrorStats::getPercentWrongForMostFrequentlyWrong()
{
	return percentWrongForMostFrequentlyWrong;
}