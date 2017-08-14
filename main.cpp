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
#include "cutoffnoboxes.h"
#include "cutoffmaybenoboxes.h"
#include "maybetograph.h"
#include "reader.h"
#include "distancecomparison.h"
#include <windows.h>
#include <stdio.h>
#include "distribution.h"
#include "soig.h"
#include "gabrielgraph.h"
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
	CUTOFF_NOBOXES,
	CUTOFF_MAYBE,
	CUTOFF_FORCES,
	CUTOFF_MAYBE_NOBOXES,
	SANN,
	SHADOW,
	CUTOFF_DOUBLECENTROID,
	CUTOFF_DOUBLECENTROID_NOBOXES,
	SOIG,
	GABRIEL
};

string getAlgorithmName(AlgorithmName algorithm){
	switch (algorithm)
	{ 
	case CUTOFF:
		return "Cutoff";
	case CUTOFF_NOBOXES:
		return "Cutoff_NoBoxes";
	case CUTOFF_MAYBE:
		return "Cutoff_Maybe";
	case CUTOFF_MAYBE_NOBOXES:
		return "Cutoff_Maybe_NoBoxes";
	case SHADOW:
		return "Shadow";
	case CUTOFF_FORCES:
		return "Cutoff_Forces";
	case CUTOFF_DOUBLECENTROID:
		return "Cutoff_DoubleCentroid";
	case CUTOFF_DOUBLECENTROID_NOBOXES:
		return "Cutoff_DoubleCentroid_NoBoxes";
	case SOIG:
		return "Sphere_of_Influence";
	case GABRIEL:
		return "Gabriel";
	default:
		cout << "UNKNOWN ALGORITHM TO OUTPUT.\n";
		return "UNKNOWN";
	}
};

vector<double> GetParameters(AlgorithmName algorithm, string material, string defect)
{
	vector<double> parameters;
	if(algorithm == CUTOFF || algorithm == CUTOFF_NOBOXES)
	{
		if(material == "PtFCC")
		{
			double rc = 3.348;
			parameters.push_back(rc);
		}
		if(material == "SiDiamond")
		{
			double rc = 3.2;
			parameters.push_back(rc);
		}
		if(material == "MgOxide")
		{
			double rc = 2.300;
			parameters.push_back(rc);
		}
		if(material == "SiMelted")
		{
			double rc = 2.850;
			parameters.push_back(rc);
		}
		if(material == "PtNanoPart")
		{
			if(defect == "Final")
			{
				double rc = 3.100;
				parameters.push_back(rc);
			}
			if(defect == "Halfway")
			{
				double rc = 3.150;
				parameters.push_back(rc);
			}
		}
	}
	else if(algorithm == CUTOFF_MAYBE || algorithm == CUTOFF_MAYBE_NOBOXES)
	{
		if(material == "PtFCC")
		{
			double rc = 3.320;
			double Rc = 3.380;
			parameters.push_back(rc);
			parameters.push_back(Rc);
		}
		if(material == "SiDiamond")
		{
			double rc = 3.14;
			double Rc = 3.19; // placeholder
			parameters.push_back(rc);
			parameters.push_back(Rc);
		}
		if(material == "MgOxide")
		{
			double rc = 2.300;
			double Rc = 2.415;
			parameters.push_back(rc);
			parameters.push_back(Rc);
		}
		if(material == "SiMelted")
		{
			double rc = 2.850;
			double Rc = 3.000;
			parameters.push_back(rc);
			parameters.push_back(Rc);
		}
		if(material == "PtNanoPart")
		{
			if(defect == "Final")
			{
				double rc = 3.100;
				double Rc = 3.255;
				parameters.push_back(rc);
				parameters.push_back(Rc);
			}
			if(defect == "Halfway")
			{
				double rc = 3.150;
				double Rc = 3.3075;
				parameters.push_back(rc);
				parameters.push_back(Rc);
			}
		}
	}
	else if(algorithm == CUTOFF_DOUBLECENTROID || algorithm == CUTOFF_DOUBLECENTROID_NOBOXES)
	{
		if(material == "PtFCC")
		{
			double rc = 3.348;
			double Rc = 3.500;
			parameters.push_back(rc);
			parameters.push_back(Rc);
		}
		if(material == "SiDiamond")
		{
			double rc = 3.200;
			double Rc = 3.240; // placeholder
			parameters.push_back(rc);
			parameters.push_back(Rc);
		}
		if(material == "MgOxide")
		{
			double rc = 2.300;
			double Rc = 2.415;
			parameters.push_back(rc);
			parameters.push_back(Rc);
		}
		if(material == "SiMelted")
		{
			double rc = 2.850;
			double Rc = 3.000;
			parameters.push_back(rc);
			parameters.push_back(Rc);
		}
		if(material == "PtNanoPart")
		{
			if(defect == "Final")
			{
				double rc = 3.100;
				double Rc = 3.255;
				parameters.push_back(rc);
				parameters.push_back(Rc);
			}
			if(defect == "Halfway")
			{
				double rc = 3.150;
				double Rc = 3.3075;
				parameters.push_back(rc);
				parameters.push_back(Rc);
			}
		}
	}
	if (algorithm == GABRIEL){
		double theta = 1.3;
		parameters.push_back(theta);
	}
	return parameters;
};


double analyzeData(AlgorithmName, string, int, int, int, vector<double>, vector<string>, string outputFolder, bool output = false);
vector<Graph> getGraphs(AlgorithmName, string, string, vector<double>, vector<string>);
bool compareGraphs(Graph, Graph, ErrorStats &);
Graph getGraph(AlgorithmName, string, string, vector<double>);
vector<Graph> getGraphsWithToleranceLevel(AlgorithmName, string, string, vector<double>, vector<string>);
int outputDataSuccessiveTimesteps(AlgorithmName, string, int, int, vector<double>, vector<int>, vector<double>, vector<int>, string, vector<ErrorStats>);
int CompareSuccessiveTimesteps(AlgorithmName, string, int, int, vector<double>, string);

int CompareSuccessiveTimeStepsAbsolutely(string, int, int, string);
int outputDataSuccessiveTimestepsAbsolutely(string, int, int, vector<int>, vector<vector<vector<int>>>, string);

double outputData(AlgorithmName, string, vector<double>, vector<short>, vector<short>, string,  vector<ErrorStats>, string);

int outputDistributionFunction(string, string, string);

int main()
{
	AlgorithmName algorithm = CUTOFF_DOUBLECENTROID;

	/* double rc = 2.000;
	double Rc = 3.350;
	vector<double> parameters;
	parameters.push_back(rc);
	parameters.push_back(Rc); */

	//choose data to run the algorithm on
	const string datapath = "R://LANL/DataUpdatedAgain/";
	const string outputFolder = "R://LANL/AlgorithmTesting/ConsolidatedTest/";
	const string materials[] = { "MgOxide"}; // "MgOxide", "PtNanoPart", "SiMelted" }; //PtFCC", "SiDiamond" }; //{ "PtFCC"}; //"SiDiamond"};//, "PtNanoPart", "SiMelt"};
	// const string defects[] = { "Standard", "Final", "Halfway" "Extra", "Gap" };
	const string temperatures[] = { "50K", "150K", "300K" };
	vector<string> material(materials, materials + sizeof(materials) / sizeof(materials[0]));
	vector<string> temperature(temperatures, temperatures + sizeof(temperatures) / sizeof(temperatures[0]));
	vector< vector<string> > defect = vector < vector<string> >();
	for (int i = 0; i < material.size();i++) {
		defect.push_back(vector < string>());
		if (material.at(i) == "PtNanoPart") {
			defect.at(i).push_back("Final");
			defect.at(i).push_back("Halfway");
		}
		if (material.at(i) == "MgOxide") {
			defect.at(i).push_back("Standard");
		}
		if (material.at(i) == "SiMelted") {
			defect.at(i).push_back("Standard");
		}
		if (material.at(i) == "SiDiamond") {

			defect.at(i).push_back("Extra");
			defect.at(i).push_back("Gap");
		}
		if (material.at(i) == "PtFCC") {
			defect.at(i).push_back("Extra");
			defect.at(i).push_back("Gap");
		}
	}
	//string folderPath = datapath + material + "/" + defect + "/" + temperature;
	// Choose the level of minimization you want to compare to the fully minimized state. "0" = no minimization. Other options are "tol_2", "tol_4", and "tol_6" for 10^-2, etc.
	const string mini[] = { "tol_12", "tol_8", "tol_6", "tol_4", "tol_2", "0" };
	vector<string> MinimizationLevels(mini, mini + sizeof(mini) / sizeof(mini[0])); ;
	// choose timestamps. available data: from 5010 to 15000, timestep 10.
	const int firstTime = 5010;
	const int lastTime = 15000;
	const int timeStep = 10;
	const int firstTimeForDifferentTimeStep = 5010;
	const int lastTimeForDifferentTimeStep = 15000;
	const bool makeOutputFile = (timeStep == true);


	//Code for testing cutoff values
	/*
	for(double rc = 3.1; rc <= 3.2; rc +=0.05)
	{
	double Rc = 1.05*rc;
	vector<double> parameters;
	parameters.push_back(rc);
	parameters.push_back(Rc);
	string path = datapath + "PtNanoPart/Halfway/150K";
	cout << analyzeData(algorithm, path, firstTime, lastTime, timeStep, parameters, MinimizationLevels, outputFolder, makeOutputFile) << endl;

	}
	return 0;
	*/

	//Code for calculating pair distribution function
	/*
	string path = datapath + "SiDiamond/Extra/300K";
	string fileNameDistribution = path + "/minimize_tol_12_15000.data";
	int out = outputDistributionFunction(fileNameDistribution, outputFolder, path);
	*/

	// Analyze data from single file (specified above)

	int numberRanPerTemp = 10000 / timeStep;
	string folderPath;
	string description;
	string outputSummaryFile = outputFolder + "AggregateSummary.txt";
	for (int i = 0; i < material.size();i++) {
		//PtNanoPart and SiMelt don't have gap or extra so their code is done in the else statement
		// vector<double> parameters = GetParameters(algorithm, material.at(i));
		for (int j = 0;j < defect.at(i).size();j++) {
			vector<double> parameters = GetParameters(algorithm, material.at(i), defect.at(i).at(j));
			for (int k = 0;k < temperature.size();k++) {
				cout << temperature.at(k) << endl;
				//this creates the location of the files you wish to read
				folderPath = datapath;
				folderPath.append(material.at(i) + "/");
				folderPath.append(defect.at(i).at(j) + "/");
				folderPath.append(temperature.at(k));
				//create a description of the files independent of the location
				description = material.at(i);
				description.append(defect.at(i).at(j));
				description.append(temperature.at(k));
				//run the analysis function, which writes to your summary file and writes to individual files in the folder more specific information
				cout << analyzeData(algorithm, folderPath, firstTime, lastTime, timeStep, parameters, MinimizationLevels, outputFolder, makeOutputFile) << endl;
				cout << CompareSuccessiveTimesteps(algorithm, folderPath, firstTimeForDifferentTimeStep, lastTimeForDifferentTimeStep, parameters, outputFolder);
				// CompareSuccessiveTimeStepsAbsolutely(folderPath, firstTimeForDifferentTimeStep, lastTimeForDifferentTimeStep, outputFolder);

			}
		}
	};



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

int outputDistributionFunction(string fileName, string outputFolder, string folderPath)
{
	Molecule molecule;
	vector<double> distances;
	Reader myReader = Reader();
	if (myReader.Initialize(fileName)) {
		if (fileName.find("MgOxide") != std::string::npos) {
			molecule = myReader.GetMgOxideFromOutputFile();
		}
		else {
			molecule = myReader.GetMoleculeFromOutputFile();
		}
		distances = GetDistancesBetweenAllAtoms(molecule);
		string outFileName = outputFolder + "DistributionFunction/";
		createFolder(outFileName.c_str());
		Reader reader = Reader();
		vector<string>splitFolder = reader.split(folderPath.c_str(),'/');
		createFolder(outFileName.c_str());
		outFileName += splitFolder.at(splitFolder.size()-3) + "/";//material
		createFolder(outFileName.c_str());
		outFileName += splitFolder.at(splitFolder.size()-2) + "_";//defect
		outFileName += splitFolder.at(splitFolder.size()-1);//temperature
		outFileName += ".txt";
		std::ofstream file = std::ofstream(outFileName);
		if (!file)
		{
			std::cout << outFileName << " cannot be accessed and/or written to. Terminating process";
			return -1;
		} else {

			file = std::ofstream(outFileName);
			for(int i = 0; i < distances.size(); i++)
			{
				file << distances.at(i) << endl;
			}

		}
	}
	return 0;
};

Graph getGraph(AlgorithmName algorithm, string fileName, string ForceFileName, vector<double> parameters){
	Graph gh;
	Molecule molecule;
	Reader myReader = Reader();
	if (myReader.Initialize(fileName)) {
		if (fileName.find("MgOxide") != std::string::npos) {
			molecule = myReader.GetMgOxideFromOutputFile();
		}
		else {
			molecule = myReader.GetMoleculeFromOutputFile();
		}
		switch (algorithm)
		{ 
		case CUTOFF:
			double rc;
			rc = parameters[0];
			gh = Cutoff(molecule, rc); 
			break;
		case CUTOFF_NOBOXES:
			rc = parameters[0];
			gh = CutoffNoBoxes(molecule, rc);
			break;
		case CUTOFF_MAYBE:
			double Rc;
			rc = parameters[0];
			Rc = parameters[1];
			gh = CutoffCentroid(molecule, rc, Rc); 
			break;
		case CUTOFF_MAYBE_NOBOXES:
			rc = parameters[0];
			Rc = parameters[1];
			gh = CutoffCentroidNoBoxes(molecule, rc, Rc);
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
		case CUTOFF_DOUBLECENTROID_NOBOXES:
			rc = parameters[0];
			Rc = parameters[1];
			gh = CutoffDoubleCentroidNoBoxes(molecule, rc, Rc);
			break;
		case SOIG:
			{
				Soig soig = Soig();
				gh = soig.CreateGraphSoig(molecule);
				break;
			}
		case GABRIEL:
			double theta;
			theta = parameters[0];
			GabrielGraph gg;
			gh = gg.ComputeGabrielMolecule(molecule, theta);
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
			// Molecule molecule = myReader.GetMoleculeFromOutputFile();
			// int moleculeSize = molecule.GetNumberOfAtoms();
			graphs.push_back(getGraph(algorithm, file, force, parameters)); 
		}
	}
	return graphs;
};

int CompareSuccessiveTimesteps(AlgorithmName algorithm, string folderPath, int firstTime, int lastTimeForDifferentTimeStep, vector<double> parameters, string outputFolder){
	vector<int> TransitionTimes;
	vector<double> HashValues;
	vector<int> TimeSteps;
	string fileName = folderPath + "/minimize_tol_12_" + to_string(firstTime) + ".data";
	string ForceFileName = folderPath + "/forces_tol_12_" + to_string(firstTime) + ".data";
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
		ForceFileName = folderPath + "/forces_tol_12_" + to_string(time) + ".data";
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

int outputDataSuccessiveTimesteps(AlgorithmName algorithm, string folderPath, int firstTime, int lastTime, vector<double> parameters, vector<int> TransitionTimes, vector<double> HashValues, vector<int> TimeSteps, string outputFolder, vector<ErrorStats> statsForMolecules){
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
	outFileName += "times" + to_string(firstTime);
	outFileName += "-" + to_string(lastTime);
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

int CompareSuccessiveTimeStepsAbsolutely(string folderPath, int firstTimeForDifferentTimeStep, int lastTimeForDifferentTimeStep, string outputFolder)
{
	vector<int> TransitionTimes;
	vector<vector<vector <int>>> MissedAtomsAllTimeSteps;
	string fileName = folderPath + "/minimize_tol_12_" + to_string(firstTimeForDifferentTimeStep) + ".data";
	Molecule moleculeCurrent;
	Molecule moleculeNext;
	Reader myReader = Reader();
	if (myReader.Initialize(fileName)) {
		if (fileName.find("MgOxide") != std::string::npos) {
			moleculeCurrent = myReader.GetMgOxideFromOutputFile();
		}
		else {
			moleculeCurrent = myReader.GetMoleculeFromOutputFile();
		}
	}
	for(int time = firstTimeForDifferentTimeStep+10; time <= lastTimeForDifferentTimeStep; time+=10){
		cout << time << "\n";
		fileName = folderPath + "/minimize_tol_12_" + to_string(time) + ".data";
		if (myReader.Initialize(fileName)) {
			if (fileName.find("MgOxide") != std::string::npos) {
				moleculeNext = myReader.GetMgOxideFromOutputFile();
			}
			else {
				moleculeNext = myReader.GetMoleculeFromOutputFile();
			}
		}
		vector<vector<int>> MissedPairs = CompareDistancesBetweenAllAtoms(moleculeCurrent, moleculeNext);

		if(MissedPairs.size() > 0){
			TransitionTimes.push_back(time);
			MissedAtomsAllTimeSteps.push_back(MissedPairs);
		}
	}
	outputDataSuccessiveTimestepsAbsolutely(folderPath, firstTimeForDifferentTimeStep, lastTimeForDifferentTimeStep, TransitionTimes, MissedAtomsAllTimeSteps, outputFolder);
	return 0;
};

int outputDataSuccessiveTimestepsAbsolutely(string folderPath, int firstTime, int lastTime, vector<int> TransitionTimes, vector<vector<vector <int>>> MissedAtomsAllTimeSteps, string outputFolder){
	string outFileName = outputFolder + "DifferentTimeStep/";
	createFolder(outFileName.c_str());
	Reader reader = Reader();
	vector<string>splitFolder = reader.split(folderPath.c_str(),'/');
	createFolder(outFileName.c_str());
	outFileName += splitFolder.at(splitFolder.size()-3) + "/";//material
	createFolder(outFileName.c_str());
	outFileName += "Absolute/";
	createFolder(outFileName.c_str());
	outFileName += splitFolder.at(splitFolder.size()-2) + "_";//defect
	outFileName += splitFolder.at(splitFolder.size()-1);//temperature
	outFileName += "times" + to_string(firstTime);
	outFileName += "-" + to_string(lastTime);
	string outFileNameTransitions = outFileName + ".transitions";
	std::ofstream file = std::ofstream(outFileNameTransitions);
	if (!file)
	{
		std::cout << outFileNameTransitions << " cannot be accessed and/or written to. Terminating process";
		return -1;
	} else {
		file = std::ofstream(outFileNameTransitions);
		file << outFileNameTransitions << "\n";
		int NumberofTransitions = TransitionTimes.size();
		file << "1000 states checked" << endl;
		file << NumberofTransitions << " state transitions detected" << endl;
		file << "\n TIMESTEPS AT WHICH TRANSITIONS OCCURED: \n";
		for(int i = 0; i < NumberofTransitions; i++){
			file << TransitionTimes.at(i) << endl;

		}
		file<< endl<<endl;
	}
	for(int j = 0; j < TransitionTimes.size(); j++){
		file << "TimeStep: " << TransitionTimes.at(j) << endl;
		for(int k = 0; k < MissedAtomsAllTimeSteps.at(j).size(); k++)
		{
			if(MissedAtomsAllTimeSteps.at(j).at(k).size()>0)
			{
				file << "Atom " << k+1 << ":";
				for(int l = 0; l < MissedAtomsAllTimeSteps.at(j).at(k).size(); l++)
				{
					file << MissedAtomsAllTimeSteps.at(j).at(k).at(l) << " ";
				}
				file << endl;
			}
		}
	}
};


bool compareGraphs(Graph min, Graph pre, ErrorStats & stats){
	cout<<"didnt hit em back"<<endl;
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
			outputData(algorithm, folderPath, parameters, sameTimes[i], diffTimes[i], MinimizationLevels[i], statsForMolecules[i], outputFolder);
		else
			float(sameTimes.size())/float(sameTimes.size() + diffTimes.size());
	} 
	return 0;
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