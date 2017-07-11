// example about structures
#include <string>
#include <sstream>
#include "Atom.h"
#include "Molecule.h"
#include "shadow.h"
//#include "cutoff.h"
#include "reader.h"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

bool compareGraphsCutOff(string, string, double);
double analyzeDataCutOff(string, int, int, int, double, bool output = false);

bool compareGraphsShadow(string, string, double, double);
double analyzeDataShadow(string, int, int, int, double, double, bool output = false);

int main()
{
	//choose data to run the algorithm on
	const string datapath = "R://LANL/Data/";
	const string material = "PtFCC";
	const string defect = "Gap";
	const string temperature = "300K";
	string folderPath = datapath + material + "/" + defect + "/" + temperature;
	// choose timestamps. available data: from 5010 to 15000, timestep 10.
	const int firstTime = 5010;
	const int lastTime = 15000;
	const int timeStep = 1850;
	
	//set cutoff distance [run on full data for 2.6, 3.2 for SiDiamond; 3.348=0.854(halfway between first and second shell)*3.92(lattice constant) for fcc data]
	//const double rc = 2.7;
	//cout << analyzeDataCutOff(folderPath, firstTime, lastTime, timeStep, rc);

	string defects [] = {"Extra"};
	string temperatures [] = {"100K", "150K", "200K", "250K"};
	
	for (double S = .1; S<=.9; S+=.2){
		for (double rc = 3.348; rc<=3.348; rc+=.5){
		cout << "\n" << rc << " " << S << "\n";
			cout << analyzeDataShadow(folderPath, firstTime, lastTime, timeStep, rc, S);
		}
	}
	/*
	pre-results to check
	si diamond gap 50K 
		2.15 anyS
		2.60 anyS?
		try 3.2, varS
	PtFCC gap 50K
		3-3.5
		4.5		#takes ages
	*/

	double rc = 4.2;
	double S = .4;
	//cout << analyzeDataShadow(folderPath, firstTime, lastTime, timeStep, rc, S);
	
	/*
	// test on specific graph
	string pre = "R://LANL/Data/SiDiamond/Gap/500K/preminimize8010.data";
	string min = "R://LANL/Data/SiDiamond/Gap/500K/minimized8010.data";
	string fileNames[2] = {pre, min};
	Graph graphs[2];
	for (int i = 0; i<2; ++i){
		Reader myReader = Reader();
		if (myReader.Initialize(fileNames[i])) {
			Molecule molecule = myReader.GetMoleculeFromOutputFile();
			graphs[i] = Shadow(molecule, 3, 0.2);
			graphs[i].printVertex(113);
		}
	}
	cout << (graphs[0] == graphs[1]);
	*/




	//find rc for pt nano part
	/*for (auto i : temperatures){
		for (double rc = 3.348; rc==3.348; rc+=0.15){
			string folderPath = datapath + "PtFCC/Extra" + "/" + i;
			cout << folderPath << " " << rc << "\n";
			cout << analyzeDataCutOff(folderPath, firstTime, lastTime, timeStep, rc) << "\n";
		}
	}*/





	cout << "\nDone.";
	cin.get();

	return 0;
}
/*
double analyzeDataCutOff(string folderPath, int firstTime, int lastTime, int timeStep, double rc, bool output){
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
	if (output) {
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
	{
	return float(sameTimes.size())/float(sameTimes.size() + diffTimes.size());
}*/
double analyzeDataShadow(string folderPath, int firstTime, int lastTime, int timeStep, double rc, double S, bool output){
	vector<short> sameTimes;
	vector<short> diffTimes;
	
	for (int time = firstTime; time <= lastTime; time+=timeStep){
		string pre = folderPath + "/minimized" + to_string(time) + ".data";
		string min = folderPath + "/preminimize" + to_string(time)+ ".data";
		
		

		bool same = compareGraphsShadow(pre, min, rc, S);
		if (same) {
			sameTimes.push_back(time);
		} else {
			diffTimes.push_back(time);
		}
		cout << same;
	}
	cout << "\n";
	//output data
	if (output) {
		string outFileName = folderPath + "/OutputShadow" + to_string(rc) + "-" + to_string(S) + ".txt";
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
	}
	return float(sameTimes.size())/float(sameTimes.size() + diffTimes.size());
}
/*bool compareGraphsCutOff(string pre, string min, double rc){
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

			cout << "\n";
		}
	}
	return (graphs[0] == graphs[1]);
}
*/
bool compareGraphsShadow(string pre, string min, double rc, double S){
	string fileNames[2] = {pre, min};
	Graph graphs[2];
	for (int i = 0; i<2; ++i){
		Reader myReader = Reader();
		if (myReader.Initialize(fileNames[i])) {
			Molecule molecule = myReader.GetMoleculeFromOutputFile();
			graphs[i] = Shadow(molecule, rc, S);

			//check a vertex
			/*cout << "\n";
			graphs[i].printVertex(52);
			cout << "\n";*/
		}
	}
	return (graphs[0] == graphs[1]);
}