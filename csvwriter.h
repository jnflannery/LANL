#pragma once
#include <string>
#include "molecule.h"
#include <vector>
#include <stdio.h> 
#include "resultsreader.h"
#include <iostream>
#include <fstream>
/*
class used to consolidate data collection
*/
class CSVWriter {
private:
	vector<string> materials;
	vector<string>minimizationLevels;
	vector<string> algorithms;
	vector<string> temperatures;
	vector<string> names;
	std::string inputDirectory;
	std::string outputDirectory;
	string CreateDifferentStepInputFile(std::string material, std::string algo, std::string defect, string temp, string param);
	string CreateSameStepInputFile(std::string material, std::string algo, std::string defect, string tol, string temp, string param);
	vector< vector<string> > defects;
public:
	//creates a csv that shows transitions for all algorithms performed on a select material in one CSV
	void createCsvForDifferentTimesteps(string material, string defect);
	//Same as last but for returning percentages right for same timesteps
	void createCsvForSameTimesteps(string material, string defect);
	//function we used just to create graphs
	void createCsvForSameGraphCreation(string material, string defect);
	//gives the reader a file and it checks the validity of said file. Will output false if it can't be opened
	void initializeInputDirectory(std::string directory);
	//set a file to output to
	void seOutputDirectory(std::string directory);
};