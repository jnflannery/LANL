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
		default:
			cout << "UNKNOWN ALGORITHM TO OUTPUT.\n";
			return "UNKNOWN";
		}
}


double analyzeData(AlgorithmName, string, int, int, int, vector<double>, vector<string>, string outputFolder, bool output = false);
vector<Graph> getGraphs(AlgorithmName, string, string, vector<double>, vector<string>, ErrorStats &);
bool compareGraphs(Graph, Graph, ErrorStats &);

double outputData(AlgorithmName, string, vector<double>, vector<short>, vector<short>, string,  vector<ErrorStats>, string);

int main()
{
	AlgorithmName algorithm = CUTOFF;

	//choose data to run the algorithm on
	const string datapath = "R://LANL/DataUpdatedAgain/";
	const string outputFolder = "R://LANL/AlgorithmTesting/ConsolidatedTest/";
	const string materials[] =  { "PtFCC", "SiDiamond"};//, "PtNanoPart", "SiMelt"};
	const string defects[] = { "Extra", "Gap" };
	const string temperatures[] = {  "50K", "300K","500K", "750K", "1000K"};
	vector<string> material(materials, materials + sizeof(materials)/sizeof(materials[0]));
	vector<string> defect(defects, defects + sizeof(defects)/sizeof(defects[0]));
	vector<string> temperature(temperatures, temperatures + sizeof(temperatures)/sizeof(temperatures[0]));

	//string folderPath = datapath + material + "/" + defect + "/" + temperature;
	// Choose the level of minimization you want to compare to the fully minimized state. "0" = no minimization. Other options are "tol_2", "tol_4", and "tol_6" for 10^-2, etc.
	const string mini[] = {"tol_12","tol_4"};//, "tol_8", "tol_6", "tol_4", "tol_2", "0"}; 
	vector<string> MinimizationLevels (mini, mini + sizeof(mini)/sizeof(mini[0])); ; 
	// choose timestamps. available data: from 5010 to 15000, timestep 10.
	const int firstTime = 5010;
	const int lastTime = 15000;
	const int timeStep = 5000;
	const bool makeOutputFile = (timeStep == 10);

	// Analyze data from single file (specified above)
	double rc = 3.348;
	double Rc = 3.380;
	vector<double> parameters;
	parameters.push_back(rc);
	parameters.push_back(Rc);
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
				cout<< analyzeData(CUTOFF_FORCES, folderPath, firstTime, lastTime, timeStep, parameters, MinimizationLevels, outputFolder, makeOutputFile) << endl;
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
			case SANN:
				Sann sann; sann = Sann();
				graphs.push_back(sann.ComputeSannMolecule(molecule));
				break;
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
		
		vector<Graph> graphs = getGraphs(algorithm, folderPath, time, parameters, MinimizationLevels);
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
		
		std::ofstream file = std::ofstream(outFileName);
		if (!file)
		{
			std::cout << outFileName << " cannot be accessed and/or written to. Terminating process";
			return -1;
		} else {
			outFileName += ".txt";
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