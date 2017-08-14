#include "resultsreader.h"
#include <string>
using namespace std;
#include <sstream>
#include <vector>
#include <iterator>

std::vector<std::string> ResultsReader::split(const char *str, char c = ' ')
{
	std::vector<std::string> result;

	do
	{
		const char *begin = str;

		while (*str != c && *str)
			str++;

		result.push_back(std::string(begin, str));
	} while (0 != *str++);

	return result;
}

int ResultsReader::GetNumberOfTransitions(ifstream &file) {
	string line;
	getline(file, line);
	getline(file, line);
	int transitions;
	file >> transitions;
	return transitions;
}
vector<int> ResultsReader::GetTransitions(ifstream &file) {
	string line;
	vector<int> transitions = vector <int>();
	int num = GetNumberOfTransitions(file);
	if (num > 0) {
		getline(file, line);
		getline(file, line);
		getline(file, line);
		int tran;
		for (int i = 0;i < num;i++) {
			file >> tran;
			transitions.push_back(tran);
			getline(file, line);
		}
	}
	return transitions;
}
float ResultsReader::GetPercentCorrect(ifstream &file) {
	string line;
	getline(file, line);
	getline(file, line);

	std::vector<std::string> lineSplit = split(line.c_str(), '(');
	std::vector<std::string> percent = split(lineSplit.at(1).c_str(), '%');
	return std::stod(percent.at(0).c_str(), NULL);

}