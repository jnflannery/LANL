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
#include "reader.h"
#include "distancecomparison.h"

/* When new algorithm is added, change 'outputData' and 'compareGraphs' accordingly. */
enum AlgorithmName
{
	CUTOFF,
	CUTOFF_MAYBE,
	SANN,
	SHADOW,
};

using namespace std;

bool compareGraphs(AlgorithmName, string, string, vector<double>);
double analyzeData(AlgorithmName, string, int, int, int, vector<double>, bool output = false);
double outputData(AlgorithmName, string, vector<double>, vector<short>, vector<short>);

int main()
{
	//choose data to run the algorithm on
	const string datapath = "R://LANL/Data/";
	string material = "PtFCC";
	string defect = "Extra";
	string temperature = "100K";
	string folderPath = datapath + material + "/" + defect + "/" + temperature;
	// choose timestamps. available data: from 5010 to 15000, timestep 10.
	const int firstTime = 5010;
	const int lastTime = 15000;
	const int timeStep = 400;
	const bool makeOutputFile = false;

	// Analyze data from single file (specified above)
	double rc = 3.320;
	double Rc = 3.380;
	vector<double> parameters;
	parameters.push_back(rc);
	parameters.push_back(Rc);
	cout << analyzeData(CUTOFF_MAYBE, folderPath, firstTime, lastTime, timeStep, parameters, makeOutputFile);


	// Run on multiple temperatures
	// CHECK CUTOFF WITH CENTROID MAYBE-RESOLUTION
	string temperatures [] = {"50K", "100K", "150K", "200K", "250K", "300K", "500K", "1000K", "1500K", "2000K"};
	rc = 3.320;
	Rc = 3.380;
	parameters;
	for (auto temperature : temperatures){
		string folderPath = datapath + material + "/" + defect + "/" + temperature;
		cout << "\n" << folderPath+" " << " Centroid cutoff "<<rc<<" - "<<Rc<<"\n";
		cout << analyzeData(CUTOFF_MAYBE, folderPath, firstTime, lastTime, timeStep, parameters, makeOutputFile);
	}


	cout << "\nDone.";
	cin.get();

	return 0;
}

bool compareGraphs(AlgorithmName algorithm, string folderPath, int time, vector<double> parameters){
	string pre = folderPath + "/minimized" + to_string(time) + ".data";
	string min = folderPath + "/preminimize" + to_string(time)+ ".data";
	string fileNames[2] = {pre, min};
	Graph graphs[2];
	for (int i = 0; i<2; ++i){
		Reader myReader = Reader();
		if (myReader.Initialize(fileNames[i])) {
			Molecule molecule = myReader.GetMoleculeFromOutputFile();

			switch (algorithm)
			{ 
			case CUTOFF:
				double rc;
				rc = parameters[0];
				graphs[i] = Cutoff(molecule, rc);
				break;
			case CUTOFF_MAYBE:
				double Rc;
				rc = parameters[0];
				Rc = parameters[1];
				graphs[i] = CutoffMaybe(molecule, rc, Rc);
				break;
			case SHADOW:
				double S;
				rc = parameters[0];
				S = parameters[1];
				graphs[i] = Shadow(molecule, rc, S);
				break;
			default:
				cout << "UNKNOWN ALGORITHM.\n";
				break;
			}
		}
	}
	return (graphs[0] == graphs[1]);
}
double analyzeData(AlgorithmName algorithm, string folderPath, int firstTime, int lastTime, int timeStep, vector<double> parameters, bool output){
	vector<short> sameTimes;
	vector<short> diffTimes;

	for (int time = firstTime; time <= lastTime; time+=timeStep){
		bool same = compareGraphs(algorithm, folderPath, time, parameters);
		if (same) 
			sameTimes.push_back(time);
		else 
			diffTimes.push_back(time);
		cout << same;
	}
	cout << "\n";
	//output data
	if (output) 
		return outputData(algorithm, folderPath, parameters, sameTimes, diffTimes);
	else
		return float(sameTimes.size())/float(sameTimes.size() + diffTimes.size());
}
double outputData(AlgorithmName algorithm, string folderPath, vector<double> parameters, vector<short> sameTimes, vector<short> diffTimes){

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
		default:
			cout << "UNKNOWN ALGORITHM TO OUTPUT.\n";
			break;
		}
		for (double p : parameters){
			outFileName += "-" + to_string(p);
		}
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
			for (vector<short>::iterator it = diffTimes.begin(); it != diffTimes.end(); ++it){
				file << *it << "\n";
			}
			return double(sameCount)/double(totalCount);
		}
	}
}
