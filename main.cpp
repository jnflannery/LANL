// example about structures
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>

#include "sann.h"
#include "Atom.h"
#include "Molecule.h"
#include "shadow.h"
#include "cutoffmaybe.h"
#include "cutoff.h"
#include "maybetograph.h"
#include "reader.h"
#include "distancecomparison.h"
#include <windows.h>
#include <stdio.h>
#include "soig.h"
using namespace std;

//auxiliary function for folder creation
void createFolder(const char * path)
{   
    if(!CreateDirectory(path ,NULL))
    {
        return;
    }
}


/* When new algorithm is added, change 'outputData' and 'compareGraphs' accordingly. */
enum AlgorithmName
{
	CUTOFF,
	CUTOFF_MAYBE,
	CUTOFF_FORCES,
	SANN,
	SHADOW,
	CUTOFF_DOUBLECENTROID,
	SOIG
};
string getAlgorithmName(AlgorithmName algorithm){
		switch (algorithm)
		{ 
		case CUTOFF:
			return "Cutoff";
		case CUTOFF_MAYBE:
			return "Cutoff_Maybe";
		case SHADOW:
			return "Shadow";
		case CUTOFF_FORCES:
			return "Cutoff_Forces";
		case CUTOFF_DOUBLECENTROID:
			return "Cutoff_DoubleCentroid";
		default:
			cout << "UNKNOWN ALGORITHM TO OUTPUT.\n";
			return "UNKNOWN";
		}
}


double analyzeData(AlgorithmName, string, int, int, int, vector<double>, vector<string>, string outputFolder, bool output = false);
vector<Graph> getGraphs(AlgorithmName, string, string, vector<double>, vector<string>);
bool compareGraphs(Graph, Graph, ErrorStats &);
Graph getGraph(AlgorithmName, string, string, vector<double>);
vector<Graph> getGraphsWithToleranceLevel(AlgorithmName, string, string, vector<double>, vector<string>);
int outputDataSuccessiveTimesteps(AlgorithmName, string, int, int, vector<double>, vector<int>, vector<double>, vector<double>, string, vector<ErrorStats>);
int CompareSuccessiveTimesteps(AlgorithmName, string, int, int, vector<double>, string);

double outputData(AlgorithmName, string, vector<double>, vector<short>, vector<short>, string,  vector<ErrorStats>, string);

int main()
{
	AlgorithmName algorithm = CUTOFF;
	double rc = 2.000;
	double Rc = 3.350;
	vector<double> parameters;
	parameters.push_back(rc);
	parameters.push_back(Rc);

	//choose data to run the algorithm on
	const string datapath = "R://LANL/DataUpdatedAgain/";
	const string outputFolder = "R://LANL/AlgorithmTesting/ConsolidatedTest/";
	const string materials[] = {"PtFCC", "SiDiamond"}; //{ "PtFCC"}; //"SiDiamond"};//, "PtNanoPart", "SiMelt"};
	const string defects[] = { "Extra", "Gap" };
	const string temperatures[] = {"500K"}; //{  "50K", "300K","500K", "750K", "1000K"};
	vector<string> material(materials, materials + sizeof(materials)/sizeof(materials[0]));
	vector<string> defect(defects, defects + sizeof(defects)/sizeof(defects[0]));
	vector<string> temperature(temperatures, temperatures + sizeof(temperatures)/sizeof(temperatures[0]));

	//string folderPath = datapath + material + "/" + defect + "/" + temperature;
	// Choose the level of minimization you want to compare to the fully minimized state. "0" = no minimization. Other options are "tol_2", "tol_4", and "tol_6" for 10^-2, etc.
	const string mini[] = {"tol_12", "tol_2"}; //"tol_10", "tol_8", "tol_6", "tol_4", "tol_2", "0"}; 
	vector<string> MinimizationLevels (mini, mini + sizeof(mini)/sizeof(mini[0])); ; 
	// choose timestamps. available data: from 5010 to 15000, timestep 10.
	const int firstTime = 5010;
	const int lastTime = 15000;
	const int timeStep = 1000;
	const int firstTimeForDifferentTimeStep = 5150;
	const int lastTimeForDifferentTimeStep = 5250;
	const bool makeOutputFile = (timeStep == 10);
	
	//Code for testing cutoff values
	/*
	for (double rc = 1; rc < 5; rc+=0.25){
		parameters[0] = rc;
		string path = datapath + "MgOxide/Standard/300K";
		cout << "1";
		cout << analyzeData(algorithm, path, firstTime, lastTime, timeStep, parameters, MinimizationLevels, outputFolder, makeOutputFile) << endl;
		cout << "2";
	}
	return 0;
	*/

	// Analyze data from single file (specified above)

	int numberRanPerTemp = 10000 / timeStep;
	string folderPath;
	string description;
	string outputSummaryFile = outputFolder + "AggregateSummary.txt";
	for (int i = 0; i < material.size();i++) {
		//PtNanoPart and SiMelt don't have gap or extra so their code is done in the else statement
		if (i < 2) {
			for (int j = 0;j < defect.size();j++) {
				for (int k = 0;k < temperature.size();k++) {
					cout<<temperature.at(k)<< endl;
					//this creates the location of the files you wish to read
					folderPath = datapath;
					folderPath.append(material.at(i) + "/");
					folderPath.append(defect.at(j) + "/");
					folderPath.append(temperature.at(k));

					//create a description of the files independent of the location
					description = material.at(i);
					description.append(defect.at(j));
					description.append(temperature.at(k));
					//run the analysis function, which writes to your summary file and writes to individual files in the folder more specific information
					//summaryWriter << "Material: " << material.at(i) << " Defect: " << defect.at(j) << " Temperature: " << temperature.at(k) << " Number of Runs: " << numberRanPerTemp<< endl;
					cout << analyzeData(algorithm, folderPath, firstTime, lastTime, timeStep, parameters, MinimizationLevels, outputFolder, makeOutputFile) << endl;
					cout << CompareSuccessiveTimesteps(algorithm, folderPath, firstTimeForDifferentTimeStep, lastTimeForDifferentTimeStep, parameters, outputFolder);
					//summaryWriter << endl;
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
				//summaryWriter << "Material: " << material.at(i) <<  " Temperature: " << temperature.at(k) << " Number of Runs: " << numberRanPerTemp << endl;
				cout << analyzeData(CUTOFF_DOUBLECENTROID, folderPath, firstTime, lastTime, timeStep, parameters, MinimizationLevels, outputFolder, makeOutputFile) << endl;
				cout << CompareSuccessiveTimesteps(algorithm, folderPath, firstTimeForDifferentTimeStep, lastTimeForDifferentTimeStep, parameters, outputFolder);
				//summaryWriter << endl;
			}
		}
	}


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

Graph getGraph(AlgorithmName algorithm, string fileName, string ForceFileName, vector<double> parameters){
	Graph gh; 
	Reader myReader = Reader();
	if (myReader.Initialize(fileName)) {
		Molecule molecule = myReader.GetMoleculeFromOutputFile();
		int moleculeSize = molecule.GetNumberOfAtoms();
		switch (algorithm)
		{ 
		case CUTOFF:
			double rc;
			rc = parameters[0];
			gh = Cutoff(molecule, rc); 
			break;
		case CUTOFF_MAYBE:
			double Rc;
			rc = parameters[0];
			Rc = parameters[1];
			gh = CutoffCentroid(molecule, rc, Rc); 
			break;
		case SHADOW:
			double S;
			rc = parameters[0];
			S = parameters[1];
			gh = Shadow(molecule, rc, S);
			break;
		case CUTOFF_FORCES:
			rc = parameters[0];
			Rc = parameters[1];
			myReader.Initialize(ForceFileName);
			myReader.AddForcesToMolecule(molecule);
			gh = CutoffWithForces(molecule, rc, Rc); 
			break;
		case CUTOFF_DOUBLECENTROID:
			rc = parameters[0];
			Rc = parameters[1];
			gh = CutoffDoubleCentroid(molecule, rc, Rc);
			break;
		default:
			cout << "UNKNOWN ALGORITHM.\n";
			break;
			}
		}
	return gh;
	}

vector<Graph> getGraphsWithToleranceLevel(AlgorithmName algorithm, string folderPath, int time, vector<double> parameters, vector<string> MinimizationLevels){
	int number_oftests = sizeof(MinimizationLevels)-1;
	vector<Graph> graphs; 
	for (auto tol : MinimizationLevels){
		string file = folderPath + "/minimize_" + tol + "_" + to_string(time) + ".data";
		string force = folderPath + "/forces_" + tol + "_" + to_string(time) + ".force";
		Reader myReader = Reader();
		if (myReader.Initialize(file)) {
			Molecule molecule = myReader.GetMoleculeFromOutputFile();
			int moleculeSize = molecule.GetNumberOfAtoms();
			graphs.push_back(getGraph(algorithm, file, force, parameters)); 
		}
	}
	return graphs;
};

int CompareSuccessiveTimesteps(AlgorithmName algorithm, string folderPath, int firstTime, int lastTimeForDifferentTimeStep, vector<double> parameters, string outputFolder){
	vector<int> TransitionTimes;
	vector<double> HashValues;
	vector<double> TimeSteps;
	string fileName = folderPath + "/minimize_tol_12_" + to_string(firstTime) + ".data";
	string ForceFileName = folderPath + "forces_tol_12_" + to_string(firstTime) + ".data";
	Graph CurrentGraph = getGraph(algorithm, fileName, ForceFileName, parameters);
	HashValues.push_back(CurrentGraph.getHashValue());
	TimeSteps.push_back(firstTime);
	int molecule_size = CurrentGraph.GetNumberOfVertices();
	vector<ErrorStats> statsForMolecules = vector<ErrorStats>();
	for(int time = firstTime+10; time <= lastTimeForDifferentTimeStep; time+=10){
		ErrorStats stats = ErrorStats();
		stats.initializeWithSize(molecule_size);

		cout << time << "\n";
		fileName = folderPath + "/minimize_tol_12_" + to_string(time) + ".data";
		ForceFileName = folderPath + "forces_tol_12_" + to_string(time) + ".data";
		Graph NextGraph = getGraph(algorithm, fileName, ForceFileName, parameters);
		
		int molecule_size = NextGraph.GetNumberOfVertices();
		if(!compareGraphs(CurrentGraph, NextGraph, stats)){
			TransitionTimes.push_back(time);
			stats.setTimestep(time);
			statsForMolecules.push_back(stats);
		}
		HashValues.push_back(NextGraph.getHashValue());
		TimeSteps.push_back(time);
		CurrentGraph = NextGraph; 
	}
	outputDataSuccessiveTimesteps(algorithm, folderPath, firstTime, lastTimeForDifferentTimeStep, parameters, TransitionTimes, HashValues, TimeSteps, outputFolder, statsForMolecules);
	return TransitionTimes.size();
};

int outputDataSuccessiveTimesteps(AlgorithmName algorithm, string folderPath, int firstTime, int lastTime, vector<double> parameters, vector<int> TransitionTimes, vector<double> HashValues, vector<double> TimeSteps, string outputFolder, vector<ErrorStats> statsForMolecules){
	string outFileName = outputFolder + "DifferentTimeStep/";
		createFolder(outFileName.c_str());
		Reader reader = Reader();
		vector<string>splitFolder = reader.split(folderPath.c_str(),'/');
		createFolder(outFileName.c_str());
		outFileName += splitFolder.at(splitFolder.size()-3) + "/";//material
		createFolder(outFileName.c_str());
		outFileName += getAlgorithmName(algorithm) + "/";
		createFolder(outFileName.c_str());
		outFileName += splitFolder.at(splitFolder.size()-2) + "_";//defect
		outFileName += splitFolder.at(splitFolder.size()-1);//temperature
		outFileName += "_t0-" + to_string(firstTime);
		outFileName += "_t1-" + to_string(lastTime);
		for (double p : parameters){
			outFileName += "_" + to_string(p);
		}
		string outFileNameTransitions = outFileName + ".transitions";
		string outFileNameHashValues = outFileName + ".hash";
	std::ofstream file = std::ofstream(outFileNameTransitions);
	if (!file)
			{
				std::cout << outFileNameTransitions << " cannot be accessed and/or written to. Terminating process";
				return -1;
		} else {
			file = std::ofstream(outFileNameTransitions);
			file << outFileNameTransitions << "\n";
			int NumberofTransitions = TransitionTimes.size();
			int NumberofComparisons = HashValues.size();
			file << NumberofComparisons << " states checked" << endl;
			file << NumberofTransitions << " state transitions detected" << endl;
			file << "\n TIMESTEPS AT WHICH TRANSITIONS OCCURED: \n";
			for(int i = 0; i < NumberofTransitions; i++){
				file << TransitionTimes.at(i) << endl;

			}
			file<< endl<<endl;
			for (int j = 0; j < statsForMolecules.size();j++) {
				file << statsForMolecules.at(j).getTimestep() << endl;

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
	}
	std::ofstream file2 = std::ofstream(outFileNameHashValues);
	if(!file2)
		{
				std::cout << outFileNameHashValues << " cannot be accessed and/or written to. Terminating process";
				return -1;
		} else {
			file2 = std::ofstream(outFileNameHashValues);
			int NumberofComparisons = HashValues.size();
			for(int j = 0; j < NumberofComparisons; j++){
				file2 << TimeSteps.at(j) << " " << HashValues.at(j) << endl;
			}
	}
	return 0;
};


bool compareGraphs(Graph min, Graph pre, ErrorStats & stats){
	cout<<"we are comparing a graph"<<endl;
	if (min == pre) return true;
	else {
		min.compareAndReturnMismatches(pre, stats);
		return false;
	}
}
double analyzeData(AlgorithmName algorithm, string folderPath, int firstTime, int lastTime, int timeStep, vector<double> parameters, vector<string> MinimizationLevels, string outputFolder, bool output){
	vector<vector<short>> sameTimes;
	vector<vector<short>> diffTimes;
	vector<vector<ErrorStats>> statsForMolecules = vector<vector<ErrorStats>>();
	int graphs_to_compare = MinimizationLevels.size();
	for (auto _ : MinimizationLevels){
		sameTimes.push_back(vector<short>());
		diffTimes.push_back(vector<short>());
		statsForMolecules.push_back(vector<ErrorStats>());
	}


	for (int time = firstTime; time <= lastTime; time+=timeStep){
		vector<Graph> graphs = getGraphsWithToleranceLevel(algorithm, folderPath, time, parameters, MinimizationLevels);
		int molecule_size = graphs[0].GetNumberOfVertices();
		for(int i = 1; i < graphs_to_compare; i++){
			ErrorStats stats = ErrorStats();
			stats.initializeWithSize(molecule_size);
			bool same = compareGraphs(graphs[0], graphs[i], stats);
			if (same)
				sameTimes[i].push_back(time);
			else {
				diffTimes[i].push_back(time);
				stats.setTimestep(timeStep);
				statsForMolecules[i].push_back(stats);
			}
		}
	}
	//output data
	for (int i = 1; i < graphs_to_compare; i++){
		if (true) 
			return outputData(algorithm, folderPath, parameters, sameTimes[i], diffTimes[i], MinimizationLevels[i], statsForMolecules[i], outputFolder);
		else
			return float(sameTimes.size())/float(sameTimes.size() + diffTimes.size());
	} 
}
double outputData(AlgorithmName algorithm, string folderPath, vector<double> parameters, vector<short> sameTimes, vector<short> diffTimes, string MinimizationLevel, vector<ErrorStats> statsForMolecules, string outputFolder){

	{
		string outFileName = outputFolder + "SameTimeStep/";
		createFolder(outFileName.c_str());
		Reader reader = Reader();
		vector<string>splitFolder = reader.split(folderPath.c_str(),'/');
		createFolder(outFileName.c_str());
		outFileName += splitFolder.at(splitFolder.size()-3) + "/";//material
		createFolder(outFileName.c_str());

		outFileName += getAlgorithmName(algorithm) + "/";
		createFolder(outFileName.c_str());
		outFileName += splitFolder.at(splitFolder.size()-2) + "_";//defect
		outFileName += MinimizationLevel + "_";
		outFileName += splitFolder.at(splitFolder.size()-1);//temperature
		for (double p : parameters){
			outFileName += "_" + to_string(p);
		}
		outFileName += ".txt";
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


/*
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
#include "errorstats.h"

/* When new algorithm is added, change 'outputData' and 'compareGraphs' accordingly. 
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
vector<Graph> getGraphs(AlgorithmName, string, string, vector<double>, vector<string>);
bool compareGraphs(Graph, Graph);
double outputData(AlgorithmName, string, vector<double>, vector<short>, vector<short>, vector<string>, vector<ErrorStats>);
Graph getGraph(AlgorithmName, string, string, vector<double>);
Graph getGraphsWithToleranceLevel(AlgorithmName, string, string, vector<double>, vector<string>);

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
	

	cout << "\nDone.";
	cin.get();

	return 0;
}

Graph getGraph(AlgorithmName algorithm, string fileName, string ForceFileName, vector<double> parameters){
	Graph gh; 
	Reader myReader = Reader();
	if (myReader.Initialize(fileName)) {
		Molecule molecule = myReader.GetMoleculeFromOutputFile();
		int moleculeSize = molecule.GetNumberOfAtoms();
		switch (algorithm)
		{ 
		case CUTOFF:
			double rc;
			rc = parameters[0];
			gh = Cutoff(molecule, rc); 
			break;
		case CUTOFF_MAYBE:
			double Rc;
			rc = parameters[0];
			Rc = parameters[1];
			gh = CutoffCentroid(molecule, rc, Rc); 
			break;
		case SHADOW:
			double S;
			rc = parameters[0];
			S = parameters[1];
			gh = Shadow(molecule, rc, S);
			break;
		case CUTOFF_FORCES:
			rc = parameters[0];
			Rc = parameters[1];
			myReader.Initialize(ForceFileName);
			myReader.AddForcesToMolecule(molecule);
			gh = CutoffWithForces(molecule, rc, Rc); 
			break;
		default:
			cout << "UNKNOWN ALGORITHM.\n";
			break;
			}
		}
	return gh;
	}

vector<Graph> getGraphsWithToleranceLevel(AlgorithmName algorithm, string folderPath, int time, vector<double> parameters, vector<string> MinimizationLevels){
	int number_oftests = sizeof(MinimizationLevels)-1;
	vector<Graph> graphs; 
	for (auto tol : MinimizationLevels){
		string file = folderPath + "/minimize_" + tol + "_" + to_string(time) + ".data";
		string force = folderPath + "/forces_" + tol + "_" + to_string(time) + ".force";
		Reader myReader = Reader();
		if (myReader.Initialize(file)) {
			Molecule molecule = myReader.GetMoleculeFromOutputFile();
			int moleculeSize = molecule.GetNumberOfAtoms();
			graphs.push_back(getGraph(algorithm, file, force, parameters)); 
		}
	}
};

double CompareSuccessiveTimesteps(AlgorithmName algorithm, string folderPath, int firstTime, int lastTime, vector<double> parameters){
	vector<int> TransitionTimes;
	vector<double> HashValues;
	string fileName = folderPath + "/minimize_tol_12_" + to_string(firstTime) + ".data";
	string ForceFileName = folderPath + "forces_tol_12_" + to_string(firstTime) + ".data";
	Graph CurrentGraph = getGraph(algorithm, fileName, ForceFileName, parameters);
	HashValues.push_back(CurrentGraph.getHashValue());
	for(int time = firstTime+10; time <= lastTime; time+10){
		fileName = folderPath + "/minimize_tol_12_" + to_string(time) + ".data";
		ForceFileName = folderPath + "forces_tol_12_" + to_string(time) + ".data";
		Graph NextGraph = getGraph(algorithm, fileName, ForceFileName, parameters);
		if(!compareGraphs(CurrentGraph, NextGraph)){
			TransitionTimes.push_back(time);
		}
		HashValues.push_back(NextGraph.getHashValue());
		CurrentGraph = NextGraph; 
	}

};

int outputDataSuccessiveTimesteps(AlgorithmName algorithm, string folderPath, int firstTime, int lastTime, vector<double> parameters, vector<int> TransitionTimes, vector<double> HashValues){
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
	string outFileNameTransitions = outFileName + "_tol_12_" + to_string(firstTime) + "_to_" + to_string(lastTime) + "_Transitions";
	string outFileNameHashValues = outFileName + "_tol_12_" + to_string(firstTime) + "_to_" + to_string(lastTime) + "_HashValues";
	std::ofstream file = std::ofstream(outFileNameTransitions);
	if (!file)
			{
				std::cout << outFileNameTransitions << " cannot be accessed and/or written to. Terminating process";
				return -1;
		} else {
			file = std::ofstream(outFileNameTransitions);
			file << outFileNameTransitions << "\n";
			int NumberofTransitions = TransitionTimes.size();
			int NumberofComparisons = HashValues.size();
			file << NumberofComparisons << " states checked" << endl;
			file << NumberofTransitions << " state transitions detected" << endl;
			file << "\n TIMESTEPS AT WHICH TRANSITIONS OCCURED: \n";
			for(int i = 0; i < NumberofTransitions; i++){
				file << TransitionTimes.at(i) << endl;
			}
	}
	std::ofstream file2 = std::ofstream(outFileNameHashValues);
	if(!file2)
		{
				std::cout << outFileNameHashValues << " cannot be accessed and/or written to. Terminating process";
				return -1;
		} else {
			file2 = std::ofstream(outFileNameHashValues);
			int NumberofComparisons = HashValues.size();
			for(int j = 0; j < NumberofComparisons; j++){
				file2 << HashValues.at(j) << endl;
			}
	}
};


vector<Graph> getGraphs(AlgorithmName algorithm, string folderPath, int time, vector<double> parameters, vector<string> MinimizationLevels){
	int number_of_tests = sizeof(MinimizationLevels)-1;
	vector<Graph> graphs;
	for (auto tol : MinimizationLevels){
		string file = folderPath + "/minimize_" + tol + "_" + to_string(time) + ".data";
		string force;
		Reader myReader = Reader();
		if (myReader.Initialize(file)) {
			Molecule molecule = myReader.GetMoleculeFromOutputFile();
			int moleculeSize = molecule.GetNumberOfAtoms();
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
				force = folderPath + "/forces_" + tol + "_" + to_string(time) + ".force";
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
	return graphs;
}

bool compareGraphs(Graph min, Graph pre){
	if (min == pre) return true;
	else {
		min.compareAndReturnMismatches(pre);
		return false;
	}
}

double analyzeData(AlgorithmName algorithm, string folderPath, int firstTime, int lastTime, int timeStep, vector<double> parameters, vector<string> MinimizationLevels, bool output){
	vector<vector<short>> sameTimes;
	vector<vector<short>> diffTimes;
	vector<ErrorStats> statsForMolecules = vector <ErrorStats>();

	for (int time = firstTime; time <= lastTime; time+=timeStep){
		ErrorStats stats = ErrorStats();
		vector<Graph> graphs = getGraphs(algorithm, folderPath, time, parameters, MinimizationLevels);
		vector<short> sameTime;
		vector<short> diffTime;

		int graphs_to_compare = sizeof(graphs);
		for(int i = 1; i <= graphs_to_compare; i++){
			
		
		
		}



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
*/