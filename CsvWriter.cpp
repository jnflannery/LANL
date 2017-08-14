#include "csvwriter.h"
string GetParameters(string algorithm, string material, string defect)
{
	vector<double> parameters;
	if (algorithm == "Cutoff")
	{
		if (material == "PtFCC")
		{
			double rc = 3.348;
			parameters.push_back(rc);
		}
		if (material == "SiDiamond")
		{
			double rc = 3.2;
			parameters.push_back(rc);
		}
		if (material == "MgOxide")
		{
			double rc = 2.300;
			parameters.push_back(rc);
		}
		if (material == "SiMelted")
		{
			double rc = 2.850;
			parameters.push_back(rc);
		}
		if (material == "PtNanoPart")
		{
			if (defect == "Final")
			{
				double rc = 3.100;
				parameters.push_back(rc);
			}
			if (defect == "Halfway")
			{
				double rc = 3.150;
				parameters.push_back(rc);
			}
		}
	}
	if (algorithm == "Cutoff_Maybe" || algorithm == "Cutoff_Forces")
	{
		if (material == "PtFCC")
		{
			double rc = 3.320;
			double Rc = 3.380;
			parameters.push_back(rc);
			parameters.push_back(Rc);
		}
		if (material == "SiDiamond")
		{
			double rc = 3.14;
			double Rc = 3.19; // placeholder
			parameters.push_back(rc);
			parameters.push_back(Rc);
		}
		if (material == "MgOxide")
		{
			double rc = 2.300;
			double Rc = 2.415;
			parameters.push_back(rc);
			parameters.push_back(Rc);
		}
		if (material == "SiMelted")
		{
			double rc = 2.850;
			double Rc = 3.000;
			parameters.push_back(rc);
			parameters.push_back(Rc);
		}
		if (material == "PtNanoPart")
		{
			if (defect == "Final")
			{
				double rc = 3.100;
				double Rc = 3.255;
				parameters.push_back(rc);
				parameters.push_back(Rc);
			}
			if (defect == "Halfway")
			{
				double rc = 3.150;
				double Rc = 3.3075;
				parameters.push_back(rc);
				parameters.push_back(Rc);
			}
		}
	}
	if (algorithm == "Cutoff_DoubleCentroid")
	{
		if (material == "PtFCC")
		{
			double rc = 3.348;
			double Rc = 3.500;
			parameters.push_back(rc);
			parameters.push_back(Rc);
		}
		if (material == "SiDiamond")
		{
			double rc = 3.200;
			double Rc = 3.240; // placeholder
			parameters.push_back(rc);
			parameters.push_back(Rc);
		}
		if (material == "MgOxide")
		{
			double rc = 2.300;
			double Rc = 2.415;
			parameters.push_back(rc);
			parameters.push_back(Rc);
		}
		if (material == "SiMelted")
		{
			double rc = 2.850;
			double Rc = 3.000;
			parameters.push_back(rc);
			parameters.push_back(Rc);
		}
		if (material == "PtNanoPart")
		{
			if (defect == "Final")
			{
				double rc = 3.100;
				double Rc = 3.255;
				parameters.push_back(rc);
				parameters.push_back(Rc);
			}
			if (defect == "Halfway")
			{
				double rc = 3.150;
				double Rc = 3.3075;
				parameters.push_back(rc);
				parameters.push_back(Rc);
			}
		}
	}
	string ret = "";
	for (int i = 0;i < parameters.size();i++) {
		ret += "_";
		ret+=to_string(parameters.at(i));
	}
	return ret;
};


string CSVWriter::CreateDifferentStepInputFile(std::string material, std::string algo, std::string defect, string temp, string param)
{
	string file = inputDirectory;
	file += "DifferentTimeStep/";
	file += material + "/";
	file += algo + "/";
	file += defect + "_";
	file += temp + "times5010-15000";
	if (algo == "Cutoff") {
		file += param;
	}
	else if (algo == "Cutoff_DoubleCentroid") {
		file += param;
	}
	else if (algo == "Cutoff_Maybe"|| algo == "Cutoff_Forces") {
		file += param;
	}
	else if (algo == "Gabriel") {
		file += "_1.300000";
	}
	file += ".transitions";
	return file;
}
string CSVWriter::CreateSameStepInputFile(std::string material, std::string algo, std::string defect, string tol, string temp, string param)
{
	string file = inputDirectory;
	file += "SameTimeStep/";
	file += material + "/";
	file += algo + "/";
	file += defect + "_";
	file += tol + "_";
	file += temp;
	if (algo == "Cutoff") {
		file += param;
	}
	else if (algo == "Cutoff_DoubleCentroid") {
		file += param;
	}
	else if (algo == "Cutoff_Maybe" || algo == "Cutoff_Forces") {
		file += param;
	}
	else if (algo == "Gabriel") {
		file += "_1.300000";
	}
	file += ".txt";
	cout << file << endl;
	return file;
}
void CSVWriter::createCsvForSameTimesteps(string material, string defect) {
	ResultsReader reader = ResultsReader();
	double score;
	string outputFile = outputDirectory + material + defect + "_same.csv";
	cout << outputFile << endl;
	std::ofstream file = std::ofstream(outputFile);
	//ifstream inputFile;
	vector<vector < vector< double> >  >scores = vector < vector <vector <double> > >();
	file << ",";
	for (int j = 0;j < algorithms.size();j++) {
		for (int k = 1; k < minimizationLevels.size();k++) {
			file << names.at(j) + "_" + minimizationLevels.at(k) + ",";
		}
	}
	string params;
	file << "\n";
	for (int i = 0;i < temperatures.size();i++) {
		file << temperatures.at(i) + ",";
		scores.push_back(vector <vector<double>>());
		for (int j = 0;j < algorithms.size();j++) {
			if (algorithms.at(j).find("utoff") != std::string::npos) {
				params = GetParameters(algorithms.at(j), material, defect);
			}
			for (int k = 1; k < minimizationLevels.size();k++) {
				std::ifstream myFile(CreateSameStepInputFile(material, algorithms.at(j), defect, minimizationLevels.at(k), temperatures.at(i), params));
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
void CSVWriter::createCsvForSameGraphCreation(string material, string defect) {
	ResultsReader reader = ResultsReader();
	double score;
	string outputFile = outputDirectory + "graph/" + material + defect + "_same.csv";
	cout << outputFile << endl;
	std::ofstream file = std::ofstream(outputFile);
	//ifstream inputFile;
	vector<vector < vector< double> >  >scores = vector < vector <vector <double> > >();
	file << ","; 
	vector<string> minimization;

	ResultsReader read;
		
	
	string params;
	file << "\n";
	for (int i = 0;i < temperatures.size();i++) {
		file << temperatures.at(i) + ",";
		for (int k = 1; k < minimizationLevels.size();k++) {
			if (k == 1) {
				file << "unminimized" << ",";
			}
			else {
				minimization = read.split(minimizationLevels.at(k).c_str(), '_');
				file << "e-" << minimization.at(1) + ",";
			}
		}
		file << "\n";
		scores.push_back(vector <vector<double>>());
		for (int j = 0;j < algorithms.size();j++) {
			file << names.at(j) + ",";
			if (algorithms.at(j).find("utoff") != std::string::npos) {
				params = GetParameters(algorithms.at(j), material, defect);
			}
			for (int k = 1; k < minimizationLevels.size();k++) {
				std::ifstream myFile(CreateSameStepInputFile(material, algorithms.at(j), defect, minimizationLevels.at(k), temperatures.at(i), params));
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
			file << "\n";
		}
		file << "\n";
	}
	file.close();

}
void CSVWriter::createCsvForDifferentTimesteps(string material, string defect) {
	ResultsReader reader = ResultsReader();
	int score;
	string outputFile = outputDirectory + material + defect + "_transitions.csv";
	cout << outputFile << endl;
	std::ofstream file = std::ofstream(outputFile);
	//ifstream inputFile;
	vector<vector <int>  >scores = vector <vector <int> >();
	file << ",";
	for (int j = 0;j < algorithms.size();j++) {
		file << names.at(j) + ",";
	}
	string params;
	file << "\n";
	for (int i = 0;i < temperatures.size();i++) {
		file << temperatures.at(i) + ",";
		scores.push_back(vector<int>());
		for (int j = 0;j < algorithms.size();j++) {
			if (algorithms.at(j).find("utoff") != std::string::npos) {
				params = GetParameters(algorithms.at(j), material, defect);
			}
				std::ifstream myFile(CreateDifferentStepInputFile(material, algorithms.at(j), defect, temperatures.at(i),params));
				if (!myFile)
				{
					file << "";
				}
				else {
					score = reader.GetNumberOfTransitions(myFile);
					file << score;
				}
				file << ",";
			
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
	const string algorithmarray[] = { "Cutoff", "Cutoff_Maybe","Cutoff_Forces","Cutoff_DoubleCentroid", "Sann", "Jump", "Sphere_of_Influence" ,"Gabriel" };
	const string algonamearray[] = { "Cutoff", "Cutoff: Centroid","Cutoff: Atomic Forces","Cutoff: Double Centroid", "SANN", "Gap Selection", "Sphere of Influence" ,"Gabriel Graph" };
	algorithms = vector<string>(algorithmarray, algorithmarray + sizeof(algorithmarray) / sizeof(algorithmarray[0]));
	names = vector<string>(algonamearray, algonamearray + sizeof(algonamearray) / sizeof(algonamearray[0]));
	vector<string> material(materialarray, materialarray + sizeof(materialarray) / sizeof(materialarray[0]));
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
	const string mini[] = { "tol_12","0" , "tol_2","tol_4", "tol_6","tol_8" };
	minimizationLevels = vector<string>(mini, mini + sizeof(mini) / sizeof(mini[0]));
	inputDirectory = directory;
}

void CSVWriter::seOutputDirectory(std::string directory)
{
	outputDirectory = directory;
}
