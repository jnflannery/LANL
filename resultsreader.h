#pragma once
#include <string>
#include "molecule.h"
#include <vector>

#include <iostream>
#include <fstream>
class ResultsReader {
public:
	int GetNumberOfTransitions(ifstream & file);
	vector<int> GetTransitions(ifstream & file);
	float GetPercentCorrect(ifstream & file);
	std::vector<std::string> split(const char *str, char c);
};