#include <string>
#include <sstream>
#include "Atom.h"
#include "Molecule.h"
#include "cutoff.h"
#include "reader.h"
#include "soig.h"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

bool compareGraphsSOIG(string, string, double);
double analyzeDataSOIG(string, int, int, int, double);

int main()
{
//choose data to run the algorithm on
const string datapath = "R://LANL/Data/";
const string material = "SiDiamond";
const string defect = "Extra";
const string temperature = "300K";
const string folderPath = datapath + material + "/" + defect + "/" + temperature;
// choose timestamps. available data: from 5010 to 15000, timestep 10.
const int firstTime = 5010;
const int lastTime = 5100;
const int timeStep = 10;
 
//set cutoff distance
// const double rc = 3.2;
cout << analyzeDataSOIG(folderPath, firstTime, lastTime, timeStep);

/*for (double rc = 3.16; rc<=3.20; rc+=0.005){
cout << rc << " " << analyzeDataCutOff(folderPath, firstTime, lastTime, timeStep, rc) << "\n";
}*/



cout << "\nDone.";
cin.get();

return 0;
}

double analyzeDataSOIG(string folderPath, int firstTime, int lastTime, int timeStep){
vector<short> sameTimes;
vector<short> diffTimes;
 
for (int time = firstTime; time <= lastTime; time+=timeStep){
string pre = folderPath + "/minimized" + to_string(time) + ".data";
string min = folderPath + "/preminimize" + to_string(time)+ ".data";
 
 

bool same = compareGraphsSOIG(pre, min);
if (same) {
sameTimes.push_back(time);
} else {
diffTimes.push_back(time);
}
cout << same;
}

//output data
string outFileName = folderPath + "/OutputSOIG.txt";
std::ofstream file = std::ofstream(outFileName);
if (!file)
{
std::cout << outFileName << " cannot be accessed and/or written to. Terminating process";
} else {
file = std::ofstream(outFileName);
file << folderPath << "\n";
file << "SOIG Algorithm \n";
int sameCount = sameTimes.size();
int totalCount = sameCount + diffTimes.size();
file <<sameCount<<" graph pairs same out of "<<totalCount<<" ("<<100*float(sameCount)/float(totalCount)<<"%)\n";

file << "\nTIMESTAMPS FOR SAME GRAPHS:\n";
for (vector<short>::iterator it = sameTimes.begin(); it != sameTimes.end(); ++it){
file << *it << "\n";
}
file << "\nTIMESTAMPS FOR DIFFERENT GRAPHS:\n";
for (vector<short>::iterator it = diffTimes.begin(); it != diffTimes.end(); ++it){
file << *it << "\n";
}
return float(sameCount)/float(totalCount);
}
return -1;
}

bool compareGraphsSOIG(string pre, string min){
string fileNames[2] = {pre, min};
Graph graphs[2];
for (int i = 0; i<2; ++i){
	Reader myReader = Reader();
	if (myReader.Initialize(fileNames[i])) {
	Molecule molecule = myReader.GetMoleculeFromOutputFile();
	Soig soig = Soig();
	vector<AtomWithRadius> sphereList = soig.ComputeSpheresSoig(molecule, molecule.GetCubeSize());
	int output = soig.CreateGraphSoig(molecule, sphereList, molecule.GetCubeSize(), g);
	graphs[i] = g;
// graphs[i] = Cutoff(molecule, rc);
}
}
return (graphs[0] == graphs[1]);
}
 
