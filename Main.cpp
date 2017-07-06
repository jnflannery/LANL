// example about structures
#include <string>
#include <sstream>
#include "Atom.h"
#include "Molecule.h"
#include "cutoff.h"
#include "reader.h"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

bool compareGraphsCutOff(string, string, double);
void analyzeDataCutOff(string, int, int, int, double);

int main()
{
	//choose data to run the algorithm on
	const string datapath = "R://LANL/Data/";
	const string material = "SiDiamond";
	const string defect = "Extra";
	const string temperature = "300K";
	const string folderPath = datapath + material + "/" + defect + "/" + temperature;
	// choose timestamps. available data: from 5010 to 15000, timestep 10.
	const int firstTime = 5010;
	const int lastTime = 15000;
	const int timeStep = 500;
	//set cutoff distance
	double rc = 3.1;

	analyzeDataCutOff(folderPath, firstTime, lastTime, timeStep, rc);

	cout << "Done.";
	cin.get();

	return 0;
}

void analyzeDataCutOff(string folderPath, int firstTime, int lastTime, int timeStep, double rc){
	vector<short> sameTimes;
	vector<short> diffTimes;
	
	for (int time = firstTime; time <= lastTime; time+=timeStep){
		string pre = folderPath + "/minimized" + to_string(time) + ".data";
		string min = folderPath + "/preminimize" + to_string(time)+ ".data";
		
		

		bool same = compareGraphsCutOff(pre, min, 3.10);
		if (same) {
			sameTimes.push_back(time);
		} else {
			diffTimes.push_back(time);
		}
	}

	//output data
	string outFileName = folderPath + "/OutputCutoff" + to_string(rc) + ".txt";
	std::ofstream file = std::ofstream(outFileName);
	if (!file)
		{
			std::cout << outFileName << " cannot be accessed and/or written to. Terminating process";
	} else {
		file = std::ofstream(outFileName);
		file << folderPath << "\n";
		file << "CotOff algorithm with CUtOFf distance " << to_string(rc) << "\n";
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
	}
}

bool compareGraphsCutOff(string pre, string min, double rc){
	string fileNames[2] = {pre, min};
	Graph graphs[2];
	for (int i = 0; i<2; ++i){
		Reader myReader = Reader();
		if (myReader.Initialize(fileNames[i])) {
			Molecule molecule = myReader.GetMoleculeFromOutputFile();
			graphs[i] = Cutoff(molecule, rc);
		}
	}
	return (graphs[0] == graphs[1]);
}
