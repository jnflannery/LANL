#include "reader.h"
#include "Cutoff.h"

using namespace std;

int main()
{
	cout << std::boolalpha << true;

	// set paths to files to be compared
	string min = "R://LANL/SiDiamond/Extra/500K/minimized1000.xyz" ;
	string pre_min = "R://LANL/SiDiamond/Extra/500K/preminimize1000.xyz" ;

	string myFiles [2] = {min, pre_min};
	Graph myGraphs [2];

	// read files
	Reader myReader = Reader();
	for (int i = 0; i < 2; i++){
		//Reader myReader = Reader();
		if (myReader.Initialize(myFiles[i])) {
			vector<Molecule> molecules = myReader.GetMolecules();

			//apply algorithm to get graph
			myGraphs[i] = Cutoff(molecules.back(), 2.5);

		}
	}

	// compare the two graphs
	cout << "The graphs are same ? " << (myGraphs[0] == myGraphs[1]) << endl;


	std::cin.get();
	return 0;
}
