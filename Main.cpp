// example about structures
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
		return true;
	}
	else {
		graphs[0].compareAndReturnMismatches(graphs[1], stats);
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
	
	/*
	*******************************************************************
	----------Variables which you will likely want to change-----------
	------------YOU WILL WANT TO CHANGE IT NOT TO RUN SANN-------------
	*******************************************************************
	*/
	
	//first designate a folder you want to output your results to
	const string outputFolder = "R://LANL/AlgorithmTesting/JumpGapDanny/testtest/";
	//now choose the range of timesteps you want to run on and the frequency you want to test
	const int firstTime = 5010;
	const int lastTime = 15000;
	const int timeStep = 1000;
	int numberRanPerTemp = 10000 / timeStep;

	/*
	*******************************************************************
	---------------------Code, not to be changed-----------------------
	*******************************************************************
	*/
	
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
	/*
	Graph g = Graph(500);
	DistanceComparison dist = DistanceComparison();
	Reader myReader = Reader();
	if (myReader.Initialize("R:/LANL/Data/PtFCC/Extra/500K/minimized5010.data")) {
		Molecule molecule = myReader.GetMoleculeFromOutputFile();
		std::cout << "i is: " << dist.ComputeDistAtom(molecule.GetAtom(344), molecule.GetAtomVector(), g, molecule.GetCubeSize());
		g.writeGraphAsDumpFile("R:/LANL/AlgorithmDebugging/PtFCCExtra500Kminimze5010_344.xyz", molecule);
	}
	g = Graph(500);
	if (myReader.Initialize("R:/LANL/Data/PtFCC/Extra/500K/preminimize5010.data")) {
		Molecule molecule = myReader.GetMoleculeFromOutputFile();
		std::cout << "i is: " << dist.ComputeDistAtom(molecule.GetAtom(344), molecule.GetAtomVector(), g, molecule.GetCubeSize());
		g.writeGraphAsDumpFile("R:/LANL/AlgorithmDebugging/PtFCCExtra500Kpreminimze5010_344.xyz", molecule);
	}
	g.printGraph();
	std::cout << "\nDone.";
	std::cin.get();/*
	Reader reader = Reader();
	reader.Initialize("R:/LANL/Data/SiDiamond/Perfect/300K/minimized5010_full.data");
	Molecule molecule = reader.GetMoleculeFromOutputFile();
	reader.Initialize("R:/LANL/Data/SiDiamond/Extra216/300K/forces5010.force");
	reader.AddForcesToMolecule(molecule);
	cout << molecule.GetAtom(1).GetFx() << endl;
	cout << molecule.GetAtom(210).GetFx() << endl;
	cout << molecule.GetAtom(217).GetFx();
	string in;
	cin >> in;/*
	Reader myReader = Reader();
	if (myReader.Initialize("R:/LANL/Data/SiDiamond/Perfect/50K/minimized5020.data")) {
		Molecule molecule = myReader.GetMoleculeFromOutputFile();
		cout << molecule.GetStepsAdvancedPastTimestep();
		int b;
		cin >> b;
	}*/
	int b;
	cin >> b;
	return 0;

}
