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
	string temperatures[] = { "50K", "100K", "150K", "200K", "250K", "300K", "500K", "1000K", "1500K", "2000K" };
	rc = 3.320;
	Rc = 3.380;
	parameters;
	for (auto temperature : temperatures) {
		string folderPath = datapath + material + "/" + defect + "/" + temperature;
		cout << "\n" << folderPath + " " << " Centroid cutoff " << rc << " - " << Rc << "\n";
		cout << analyzeData(CUTOFF_MAYBE, folderPath, firstTime, lastTime, timeStep, parameters, makeOutputFile);
	}


	cout << "\nDone.";
	cin.get();

	return 0;
}

bool compareGraphs(AlgorithmName algorithm, string folderPath, int time, vector<double> parameters) {
	string pre = folderPath + "/minimized" + to_string(time) + ".data";
	string min = folderPath + "/preminimize" + to_string(time) + ".data";
	string fileNames[2] = { pre, min };
	Graph graphs[2];
	for (int i = 0; i<2; ++i) {
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
double analyzeData(AlgorithmName algorithm, string folderPath, int firstTime, int lastTime, int timeStep, vector<double> parameters, bool output) {
	vector<short> sameTimes;
	vector<short> diffTimes;

	for (int time = firstTime; time <= lastTime; time += timeStep) {
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
		return float(sameTimes.size()) / float(sameTimes.size() + diffTimes.size());
}
double outputData(AlgorithmName algorithm, string folderPath, vector<double> parameters, vector<short> sameTimes, vector<short> diffTimes) {

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
		for (double p : parameters) {
			outFileName += "-" + to_string(p);
		}
		std::ofstream file = std::ofstream(outFileName);
		if (!file)
		{
			std::cout << outFileName << " cannot be accessed and/or written to. Terminating process";
			return -1;
		}
		else {
			file = std::ofstream(outFileName);
			file << outFileName << "\n";
			int sameCount = sameTimes.size();
			int totalCount = sameCount + diffTimes.size();
			file << sameCount << " graph pairs same out of " << totalCount << " (" << 100 * float(sameCount) / float(totalCount) << "%)\n";

			file << "\nTIMESTAMPS FOR SAME GRAPHS:\n";
			for (vector<short>::iterator it = sameTimes.begin(); it != sameTimes.end(); ++it) {
				file << *it << "\n";
			}
			file << "\nTIMESTAMPS FOR DIFFERENT GRAPHS:\n";
			for (vector<short>::iterator it = diffTimes.begin(); it != diffTimes.end(); ++it) {
				file << *it << "\n";
			}
			return double(sameCount) / double(totalCount);
		}
	}
}
/*
#include <string>
#include <sstream>
#include "Atom.h"
#include "Molecule.h"
#include "reader.h"
#include "boxbuilder.h"
#include "box.h"
#include "errorstats.h"
#include "coordinate.h"
#include "boxlist.h"
#include "distancecomparison.h"
using namespace std;

#include <iostream>
#include <fstream>
#include <vector>
string TEMPERATURE;
string TIMESTEP;
bool compareGraphsSANN(string pre, string min, ErrorStats & stats, int &moleculeSize) {
	DistanceComparison dist = DistanceComparison();
	string fileNames[2] = { pre, min };
	Graph graphs[2];

	if (TIMESTEP.compare("5010")==0 && TEMPERATURE.compare("500K")==0) {
		cout << endl;
	}
	for (int i = 0; i < 2; ++i) {
		Reader myReader = Reader();
		if (myReader.Initialize(fileNames[i])) {
			Molecule molecule = myReader.GetMoleculeFromOutputFile();
			moleculeSize = molecule.GetNumberOfAtoms();
			stats.initializeWithSize(moleculeSize);
			graphs[i] = dist.ComputeDistMolecule(molecule);
		}
	}
	if (graphs[0]==graphs[1]) {
		cout << "same graph hash: " << graphs[0].getHashValue() << " and " << graphs[1].getHashValue() << endl;
		return true;
		
	}
	else {
		graphs[0].compareAndReturnMismatches(graphs[1], stats);
		cout << "different graph hash: " << graphs[0].getHashValue() << " and " << graphs[1].getHashValue() << endl;
		return false;
	}
}
float analyzeDataSANN(string folderPath, int firstTime, int lastTime, int timeStep, string outputFolder, string description) {
	vector<short> sameTimes;
	vector<short> diffTimes;
	int moleculeSize = 0;
	vector<ErrorStats> statsForMolecules = vector <ErrorStats>();
	
	for (int time = firstTime; time <= lastTime; time += timeStep) {
		TIMESTEP = std::to_string(time);
//		string pre = folderPath + "/minimized" + to_string(time) + ".data";
//		string min = folderPath + "/preminimize" + to_string(time)  + ".data";
		string pre = folderPath + "/minimized" + to_string(time) + "_full" + ".data";
		string min = folderPath + "/minimized" + to_string(time) + "_iter5_eval10" + ".data";
		ErrorStats stats = ErrorStats();
		

		bool same = compareGraphsSANN(pre, min,stats, moleculeSize);
		if (same) {
			sameTimes.push_back(time);
		}
		//if there's a mismatch, also feed back which atoms were wrong
		else {
			diffTimes.push_back(time);
			statsForMolecules.push_back(stats);
		}
	}

	//output data
	string outFileName = outputFolder + "/" + description + ".txt";
	std::ofstream file = std::ofstream(outFileName);
	if (!file)
	{
		std::cout << outFileName << " cannot be accessed and/or written to. Terminating process";
	}
	else {
		file = std::ofstream(outFileName);
		file << folderPath << "\n";
		int sameCount = sameTimes.size();
		int totalCount = sameCount + diffTimes.size();
		file << sameCount << " graph pairs same out of " << totalCount << " (" << 100 * float(sameCount) / float(totalCount) << "%)\n";

		file << "\nTIMESTAMPS FOR SAME GRAPHS:\n";
		for (vector<short>::iterator it = sameTimes.begin(); it != sameTimes.end(); ++it) {
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
		return (float(sameCount) / float(totalCount));
	}
	return -1;
}


int main()
{
	

	
	//first designate a folder you want to output your results to
	const string outputFolder = "R://LANL/AlgorithmTesting/JumpGapDanny/testtest/";
	//now choose the range of timesteps you want to run on and the frequency you want to test
	const int firstTime = 5010;
	const int lastTime = 15000;
	const int timeStep = 1000;
	int numberRanPerTemp = 10000 / timeStep;


	
	//create a file to write a summary of all the information to 
	string outputSummaryFile = outputFolder + "AggregateSummary.txt";
	std::ofstream summaryWriter = std::ofstream(outputSummaryFile);

	//this iterates through the standard types of molecules (the ones created at multiple sizes) and test their extra and gap formations for your algorithm
	const string datapath = "R://LANL/Data/";
	//vector<string>material = { "PtFCC","SiDiamond", "PtNanoPart", "SiMelt"};
	//vector <string> defect = { "Extra", "Gap" };
	//vector <string> temperature = {  "50K", "300K","500K", "1000K", "1500K"};

	//vector<string>material = { "SiDiamond"};
	//vector <string> defect = { "Perfect" };
	//vector <string> temperature = { "50K" };
	vector<string>material = { "SiDiamond"};
	vector <string> defect = { "Extra216" };
	vector <string> temperature = { "50K", "300K", "500K"};
	
	string folderPath;
	string description;
	for (int i = 0; i < material.size();i++) {
		//PtNanoPart and SiMelt don't have gap or extra so their code is done in the else statement
		if (i < 2) {
			for (int j = 0;j < defect.size();j++) {
				for (int k = 0;k < temperature.size();k++) {
					//this creates the location of the files you wish to read
					folderPath = datapath;
					folderPath.append(material.at(i) + "/");
					folderPath.append(defect.at(j) + "/");
					folderPath.append(temperature.at(k));
					TEMPERATURE = temperature.at(k);
					
					//create a description of the files independent of the location
					description = material.at(i);
					description.append(defect.at(j));
					description.append(temperature.at(k));
					//run the analysis function, which writes to your summary file and writes to individual files in the folder more specific information
					summaryWriter << "Material: " << material.at(i) << " Defect: " << defect.at(j) << " Temperature: " << temperature.at(k) << " Number of Runs: " << numberRanPerTemp<< endl;
					summaryWriter << "Percentage of runs that matched: " << analyzeDataSANN(folderPath, firstTime, lastTime, timeStep, outputFolder, description) << endl;
					summaryWriter << endl;
				}
			}
		}
		else {
			for (int k = 0;k < temperature.size();k++) {
				//same thing except it doesn't include defects
				folderPath = datapath;
				folderPath.append(material.at(i) + "/");
				folderPath.append(temperature.at(k));
				description = material.at(i);
				description.append(temperature.at(k));
				summaryWriter << "Material: " << material.at(i) <<  " Temperature: " << temperature.at(k) << " Number of Runs: " << numberRanPerTemp << endl;
				summaryWriter << "Percentage of runs that matched: " << analyzeDataSANN(folderPath, firstTime, lastTime, timeStep, outputFolder, description) << endl;
				summaryWriter << endl;
			}
		}
	}
	int b;
	cin >> b;
	return 0;

}*/
