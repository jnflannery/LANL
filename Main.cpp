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
double analyzeDataCutOff(string, int, int, int, double);

int main()
{
	//choose data to run the algorithm on
	const string datapath = "R://LANL/Data/";
	const string material = "SiDiamond";
	const string defect = "Extra";
	const string temperature = "300K";
	string folderPath = datapath + material + "/" + defect + "/" + temperature;
	// choose timestamps. available data: from 5010 to 15000, timestep 10.
	const int firstTime = 5010;
	const int lastTime = 15000;
	const int timeStep = 1500;
	
	//set cutoff distance [run on full data for 2.6, 3.2 for SiDiamond; 3.348=0.854(halfway between first and second shell)*3.92(lattice constant) for fcc data]
	//const double rc = 2.7;
	//cout << analyzeDataCutOff(folderPath, firstTime, lastTime, timeStep, rc);

	string defects [] = {"Gap"};
	string temperatures [] = {"300K"};


	
	//find rc for pt nano part
	for (auto i : temperatures){
		for (double rc = 2.6; rc<5; rc+=0.15){
			string folderPath = datapath + "PtNanoPart/CompleteParticle" + "/" + i;
			cout << folderPath << " " << rc << "\n";
			cout << analyzeDataCutOff(folderPath, firstTime, lastTime, timeStep, rc) << "\n";
		}
	}





	cout << "\nDone.";
	cin.get();

	return 0;
}

double analyzeDataCutOff(string folderPath, int firstTime, int lastTime, int timeStep, double rc){
	vector<short> sameTimes;
	vector<short> diffTimes;
	
	for (int time = firstTime; time <= lastTime; time+=timeStep){
		string pre = folderPath + "/minimized" + to_string(time) + ".data";
		string min = folderPath + "/preminimize" + to_string(time)+ ".data";
		
		

		bool same = compareGraphsCutOff(pre, min, rc);
		if (same) {
			sameTimes.push_back(time);
		} else {
			diffTimes.push_back(time);
		}
		cout << same;
	}
	cout << "\n";
	//output data
	string outFileName = folderPath + "/OutputCutoff" + to_string(rc) + ".txt";
	std::ofstream file = std::ofstream(outFileName);
	if (!file)
		{
			std::cout << outFileName << " cannot be accessed and/or written to. Terminating process";
	} else {
		file = std::ofstream(outFileName);
		file << folderPath << "\n";
		file << "CotOff algorithm with CutOff distance " << to_string(rc) << "\n";
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
		return float(sameCount)/float(totalCount);
	}
	return -1;
}

bool compareGraphsCutOff(string pre, string min, double rc){
	string fileNames[2] = {pre, min};
	Graph graphs[2];
	for (int i = 0; i<2; ++i){
		Reader myReader = Reader();
		if (myReader.Initialize(fileNames[i])) {
			Molecule molecule = myReader.GetMoleculeFromOutputFile();
			graphs[i] = Cutoff(molecule, rc);


			/*
			graphs[i].printVertex(47);
			graphs[i].printVertex(48);
			graphs[i].printVertex(49);
			graphs[i].printVertex(50);
			graphs[i].printVertex(51);
			graphs[i].printVertex(52);

			cout << "\n";*/
		}
	}
	return (graphs[0] == graphs[1]);
}
