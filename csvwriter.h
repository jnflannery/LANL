#pragma once
#include <string>
#include "molecule.h"
#include <vector>
#include <stdio.h> 
#include "resultsreader.h"
#include <iostream>
#include <fstream>
class CSVWriter {
private:
	vector<string> materials;
	vector<string>minimizationLevels;
	vector<string> algorithms;
	vector<string> temperatures;
	std::string inputDirectory;
	std::string outputDirectory;
	vector< vector<string> > defects;
public:
	string CreateDifferentStepInputFile(std::string material, std::string algo, std::string defect, string temp);
	string CreateDifferentStepInputFile(std::string material, std::string algo, std::string defect, string temp, string param);
	string CreateSameStepInputFile(std::string material, std::string algo, std::string defect, string tol, string temp, string param);
	string CreateSameStepInputFile(std::string material, std::string algo, std::string defect, string tol, string temp);
	void createCsvForDifferentTimesteps(string material, string defect);
	void createCsvForSameTimesteps(string material, string defect);
	//gives the reader a file and it checks the validity of said file. Will output false if it can't be opened
	void initializeInputDirectory(std::string directory);
	void seOutputDirectory(std::string directory);
};