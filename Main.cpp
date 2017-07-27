// example about structures
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>

#include "Atom.h"
#include "Molecule.h"
#include "shadow.h"
#include "cutoffmaybe.h"
#include "cutoff.h"
#include "maybetograph.h"
#include "reader.h"
#include "distancecomparison.h"

/* When new algorithm is added, change 'outputData' and 'compareGraphs' accordingly. */
enum AlgorithmName
{
	CUTOFF,
	CUTOFF_MAYBE,
	CUTOFF_FORCES,
	SANN,
	SHADOW,
};

using namespace std;

double analyzeData(AlgorithmName, string, int, int, int, vector<double>, vector<string>, bool output = false);
vector<Graph> getGraphs(AlgorithmName, string, string, vector<double>, vector<string>, ErrorStats &);
bool compareGraphs(Graph, Graph, ErrorStats &);
double outputData(AlgorithmName, string, vector<double>, vector<short>, vector<short>, vector<string>, vector<ErrorStats>);

int main()
{
	//choose data to run the algorithm on
	const string datapath = "R://LANL/DataUpdatedAgain/";
	string material = "PtFCC";
	string defect = "Extra";
	string temperature = "50K";
	string folderPath = datapath + material + "/" + defect + "/" + temperature;
	// Choose the level of minimization you want to compare to the fully minimized state. "0" = no minimization. Other options are "tol_2", "tol_4", and "tol_6" for 10^-2, etc.
	vector<string> MinimizationLevels; MinimizationLevels.push_back("0"); 
	// choose timestamps. available data: from 5010 to 15000, timestep 10.
	const int firstTime = 5010;
	const int lastTime = 15000;
	const int timeStep = 400;
	const bool makeOutputFile = (timeStep == 10);

	// Analyze data from single file (specified above)
	double rc = 3.348;
	double Rc = 3.380;
	vector<double> parameters;
	parameters.push_back(rc);
	parameters.push_back(Rc);
	cout << analyzeData(CUTOFF_FORCES, folderPath, firstTime, lastTime, timeStep, parameters, MinimizationLevels, makeOutputFile);


	// Run on multiple temperatures
	// CHECK CUTOFF WITH CENTROID MAYBE-RESOLUTION
	/*
	string temperatures [] = {"50K", "100K", "150K", "200K", "250K", "300K", "500K", "1000K", "1500K", "2000K"};
	rc = 3.320;
	Rc = 3.380;
	parameters;
	for (auto temperature : temperatures){
		string folderPath = datapath + material + "/" + defect + "/" + temperature;
		cout << "\n" << folderPath+" " << " Centroid cutoff "<<rc<<" - "<<Rc<<"\n";
		cout << analyzeData(CUTOFF_FORCES, folderPath, firstTime, lastTime, timeStep, parameters, makeOutputFile);
	}
	*/

	cout << "\nDone.";
	cin.get();

	return 0;
}

bool getGraphs(AlgorithmName algorithm, string folderPath, int time, vector<double> parameters, vector<string> MinimizationLevels, ErrorStats & stats){
	int number_of_tests = sizeof(MinimizationLevels)-1;
	vector<Graph> graphs;
	for (auto tol : MinimizationLevels){
		string file = folderPath + "/minimize_" + tol + "_" + to_string(time) + ".data";
		Reader myReader = Reader();
		if (myReader.Initialize(file)) {
			Molecule molecule = myReader.GetMoleculeFromOutputFile();
			int moleculeSize = molecule.GetNumberOfAtoms();
			stats.initializeWithSize(moleculeSize);

			switch (algorithm)
			{ 
			case CUTOFF:
				double rc;
				rc = parameters[0];
				graphs.push_back(Cutoff(molecule, rc));
				break;
			case CUTOFF_MAYBE:
				double Rc;
				rc = parameters[0];
				Rc = parameters[1];
				graphs.push_back(CutoffCentroid(molecule, rc, Rc));

				break;
			case SHADOW:
				double S;
				rc = parameters[0];
				S = parameters[1];
				graphs.push_back(Shadow(molecule, rc, S));
				break;
			case CUTOFF_FORCES:
				rc = parameters[0];
				Rc = parameters[1];
				myReader.Initialize(force);
				myReader.AddForcesToMolecule(molecule);
				graphs.push_back(CutoffWithForces(molecule, rc, Rc));
				break;
			default:
				cout << "UNKNOWN ALGORITHM.\n";
				break;
			}
		}
	}

	if (graphs[0] == graphs[1]) return true;
	else {
		graphs[0].compareAndReturnMismatches(graphs[1], stats);
		return false;
	}
}
double analyzeData(AlgorithmName algorithm, string folderPath, int firstTime, int lastTime, int timeStep, vector<double> parameters, vector<string> MinimizationLevels, bool output){
	vector<short> sameTimes;
	vector<short> diffTimes;
	vector<ErrorStats> statsForMolecules = vector <ErrorStats>();

	for (int time = firstTime; time <= lastTime; time+=timeStep){
		ErrorStats stats = ErrorStats();
		vector<Graph> graphs = getGraphs(algorithm, folderPath, time, parameters, MinimizationLevels, stats);

		if (same) 
			sameTimes.push_back(time);
		else {
			diffTimes.push_back(time);
			statsForMolecules.push_back(stats);
		}
		cout << same;
	}
	cout << "\n";
	//output data
	if (output) 
		return outputData(algorithm, folderPath, parameters, sameTimes, diffTimes, MinimizationLevels, statsForMolecules);
	else
		return float(sameTimes.size())/float(sameTimes.size() + diffTimes.size());
}
double outputData(AlgorithmName algorithm, string folderPath, vector<double> parameters, vector<short> sameTimes, vector<short> diffTimes, vector<string> MinimizationLevels, vector<ErrorStats> statsForMolecules){

	{
		string outFileName = folderPath + "/";
		switch (algorithm)
		{ 
		case CUTOFF:
			outFileName += "Cutoff";
			break;
		case CUTOFF_MAYBE:
			outFileName += "Cutoff_Maybe";
			break;
		case SHADOW:
			outFileName += "Shadow";
			break;
		case CUTOFF_FORCES:
			outFileName += "Cutoff_Forces";
			break;
		default:
			cout << "UNKNOWN ALGORITHM TO OUTPUT.\n";
			break;
		}
		for (double p : parameters){
			outFileName += "-" + to_string(p);
		}
		outFileName += "_" + MinimizationLevel;
		std::ofstream file = std::ofstream(outFileName);
		if (!file)
			{
				std::cout << outFileName << " cannot be accessed and/or written to. Terminating process";
				return -1;
		} else {
			file = std::ofstream(outFileName);
			file << outFileName << "\n";
			int sameCount = sameTimes.size();
			int totalCount = sameCount + diffTimes.size();
			file <<sameCount<<" graph pairs same out of "<<totalCount<<" ("<<100*float(sameCount)/float(totalCount)<<"%)\n";

			file << "\nTIMESTAMPS FOR SAME GRAPHS:\n";
			for (vector<short>::iterator it = sameTimes.begin(); it != sameTimes.end(); ++it){
				file << *it << "\n";
			}
			file << "\nTIMESTAMPS FOR DIFFERENT GRAPHS:\n";
			for (int j = 0; j < diffTimes.size();j++) {
				file << diffTimes.at(j) << endl;

				file << "Percent vertices wrong: " << statsForMolecules.at(j).getPercentWrongVertices() << endl;
				file << "Avg number edges mismatched: " << statsForMolecules.at(j).getAvgNumMismatched() << endl;
				file << "Most frequently mismatched atom: " << statsForMolecules.at(j).getMostFrequentlyWrongAtom() << endl;
				file << "Percent who mistakenly added mismatched atom: " << statsForMolecules.at(j).getPercentWrongForMostFrequentlyWrong() << endl;
				file << "Wrong atoms are: ";
				for (int k = 0; k < statsForMolecules.at(j).mismatchedAtoms.size(); k++) {
					if (statsForMolecules.at(j).mismatchedAtoms.at(k).size() > 0) {
						file << k << ", ";
					}
				}
				file << endl;
				file << endl;
			}
			return double(sameCount)/double(totalCount);
		}
	}
}