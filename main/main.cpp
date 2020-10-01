#include <stdlib.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>


#include "combination.cpp"
#include "FileRead.h"
#include "Median.cpp"
#include "combTAO.cpp"
// #include "DataSet.cpp"

using namespace std;
// using namespace shark;

int main(){
    /**
     * read the .csv and save data(string) into a vector
     * @file FileRead.h
     */
    FileRead fileRead("./Data/DATA_Viral_BacMen_112.csv");
    vector<vector<string>> AllDataString = fileRead.return_strArray();   //all data from .csv save in AllDataString data type "string"

  

    /**
     * Select the combination
     * @file combination.cpp
     */
    int combm = 12;
    int combn = 3;
    vector<vector<int> > vvCombination; //Create a vector to store all combinations
    Combination(combm, combn, vvCombination);/**C(m,n), n <= m.  All combinations save to vvCombination */
    
    cout << "Combinationcount: " <<vvCombination.size() << endl;


    /**
     * Parallelization with XiTAO
	   */
    
    cout << "Start Parallelization!" << endl;

    int searchComb[vvCombination.size()];
    vector<double> AUCresults;
    
  // gotao_init();
	for(int i = 0; i < vvCombination.size(); i++){
		
    gotao_init();
		combTAO* combTAOx = new combTAO(combm, i, vvCombination, AllDataString, AUCresults);
		gotao_push(combTAOx);
		// execute the pending TAO-DAGs
  	gotao_start();  		
    gotao_fini();

  		// lossV.push_back(combTAOx);
  	searchComb[i] = i;

	}
  // gotao_fini();


  
	/**
     * write the best AUC combinations
	 */
	cout << "Start loading result to Result_AUC_Featrues.csv..." << endl;
  string pathname = "./Results/" + to_string(combm) + "_" + to_string(combn) + "_" + "Result_AUC_Featrues.csv";
	ofstream Result_AUC_Featrues;
  Result_AUC_Featrues.open(pathname, ios::out);

	for(int i = 0; i < AUCresults.size(); i++){
		double comNum = 0;
		// double vtemp[20] = accV[];
		int flag = 0;
		for(int j = 0; j < AUCresults.size(); j++){
			if(AUCresults[j] > comNum){
				comNum = AUCresults[j];
				// vetemp[j] = -1;
				flag = j;

				// cout << vvCombination[searchComb[j]][0] <<" "
			}
		}
		
		for(int e = 0; e < vvCombination[0].size(); e++){
			int feature = vvCombination[searchComb[flag]][e] + AllDataString[0].size() - 1 - combm;
			Result_AUC_Featrues << AllDataString[0][feature] << ",";
		}
		
		Result_AUC_Featrues << ",       AUC: " << AUCresults[flag] << endl;

		AUCresults[flag] = -1;
	}


  	
	cout << "Done" <<endl;
  	
    return 0;
}