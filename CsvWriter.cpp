#include "csvwriter.h"


string CSVWriter::CreateDifferentStepInputFile(std::string material, std::string algo, std::string defect, string temp)
{
	string file=inputDirectory;
	file += "DifferentTimeStep/";
	file += material + "/";
	file += algo + "/";
	file += defect + "_";
	file += temp + "times5010-15000";
	if (algo=="Cutoff") {
		file += "_3.348000";
	}
	else if (algo == "Cutoff_DoubleCentroid") {
		file += "_3.348000_3.500000";
	}
	else if (algo == "Cutoff_Maybe") {
		file += "_3.320000_3.380000";
	}
	file += ".transitions";
	return file;
}
string CSVWriter::CreateSameStepInputFile(std::string material, std::string algo, std::string defect, string tol, string temp)
{
	string file = inputDirectory;
	file += "SameTimeStep/";
	file += material + "/";
	file += algo + "/";
	file += defect + "_";
	file += tol + "_";
	file += temp;
	if (algo == "Cutoff") {
		file += "_3.348000";
	}
	else if (algo == "Cutoff_DoubleCentroid") {
		file += "_3.348000_3.500000";
	}
	else if (algo == "Cutoff_Maybe") {
		file += "_3.320000_3.380000";
	}
	file += ".txt";
	cout << file << endl;
	return file;
}
void CSVWriter::createCsvForSameTimesteps(string material, string defect) {
	ResultsReader reader = ResultsReader();
	double score;
	string outputFile = outputDirectory + material + defect + ".csv";
	cout << outputFile << endl;
	std::ofstream file = std::ofstream(outputFile);
	//ifstream inputFile;
	vector<vector < vector< double> >  >scores = vector < vector <vector <double> > >();
	file << ",";
	for(int j = 0;j < algorithms.size();j++) {
		for (int k = 0; k < minimizationLevels.size();k++) {
			file << algorithms.at(j) + "_" + minimizationLevels.at(k) + ",";
		}
	}
	file << "\n";
	for (int i = 0;i < temperatures.size();i++) {
		file << temperatures.at(i)+",";
		scores.push_back(vector <vector<double>>());
		for (int j = 0;j < algorithms.size();j++) {
			scores.at(i).push_back(vector<double>());
			for (int k = 0; k < minimizationLevels.size();k++) {
				std::ifstream myFile(CreateSameStepInputFile(material, algorithms.at(j), defect, minimizationLevels.at(k), temperatures.at(i)));
				if (!myFile)
				{
					file << "";
				}
				else {
					score = reader.GetPercentCorrect(myFile);
					file << score;
				}
				file << ",";
			}
		}
		file << "\n";
	}
	file.close();
	
}
void CSVWriter::initializeInputDirectory(std::string directory)
{
	const string materialarray[] = { "PtNanoPart","MgOxide", "SiMelted", "PtFCC", "SiDiamond" };
	const string temperaturearray[] = { "50K", "150K", "300K","500K", "750K", "1000K" };
	//ADD MORE ALGOS LATER
	const string algorithmarray[] = { "Jump", "Sann","Cutoff", "Cutoff_Maybe", "Gabriel", "Cutoff_DoubleCentroid","Sphere_of_Influence"};
	algorithms = vector<string>(algorithmarray, algorithmarray + sizeof(algorithmarray) / sizeof(algorithmarray[0]));
	vector<string> material (materialarray, materialarray + sizeof(materialarray) / sizeof(materialarray[0]));
	materials = material;
	vector<string> temperature(temperaturearray, temperaturearray + sizeof(temperaturearray) / sizeof(temperaturearray[0]));
	temperatures = temperature;
	defects = vector < vector<string> >();
	for (int i = 0; i < material.size();i++) {
		defects.push_back(vector < string>());
		if (material.at(i) == "PtNanoPart") {
			defects.at(i).push_back("Final");
			defects.at(i).push_back("Halfway");
		}
		if (material.at(i) == "MgOxide") {
			defects.at(i).push_back("Standard");
		}
		if (material.at(i) == "SiMelted") {
			defects.at(i).push_back("Standard");
		}
		if (material.at(i) == "SiDiamond") {

			defects.at(i).push_back("Extra");
			defects.at(i).push_back("Gap");
		}
		if (material.at(i) == "PtFCC") {
			defects.at(i).push_back("Extra");
			defects.at(i).push_back("Gap");
		}
	}
	//string folderPath = datapath + material + "/" + defect + "/" + temperature;
	// Choose the level of minimization you want to compare to the fully minimized state. "0" = no minimization. Other options are "tol_2", "tol_4", and "tol_6" for 10^-2, etc.
	const string mini[] = { "tol_12", "tol_8", "tol_4", "tol_2", "0" };
	minimizationLevels = vector<string>(mini, mini + sizeof(mini) / sizeof(mini[0]));
	inputDirectory = directory;
}

void CSVWriter::seOutputDirectory(std::string directory)
{
	outputDirectory = directory;
}
