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
     * calculate the median(double) of all features and save into median[](double)
     * @file Median.cpp
     * @result median[113]
     */
     double column[AllDataString.size() - 1]; //save all data for each feature
     double median[AllDataString[0].size()];   //all median of features
     for(int col = 0; col < AllDataString[0].size(); col++){
//         cout<<AllDataString.size();  //67
         int Effective_Number = 0;
         for(int row = 1; row < AllDataString.size(); row++){
             if (AllDataString[row][col] != "NA"){
                 double data_double;
//                 cout<< stod(AllDataString[row][col]) << " ";
                 data_double = stod(AllDataString[row][col]);
                 column[row - 1] = data_double;
//                 cout<< a[row - 1] << " ";
                 Effective_Number++;
             }
         }
         median[col] = EvaluateMedian(column, Effective_Number);
        // cout<< median[col]<<" ";
     }
     // cout << endl;


     /**
      * vector<vector<string>> --> vector<vector<double>> AllDataDouble
      * replace all Missing values with median
      * @file Median.cpp
      */
    vector<vector<double>> AllDataDouble(AllDataString.size() - 1, vector<double>(AllDataString[0].size()));
    for(int row = 1; row < AllDataString.size(); row++){
//         cout<<AllDataString.size();  //67
//        cout << AllDataString[0].size();    //113
        for(int col = 0; col < AllDataString[0].size(); col++){
            if (AllDataString[row][col] != "NA"){
                double data_double;
//                 cout<< stod(AllDataString[row][col]) << " ";
                data_double = stod(AllDataString[row][col]);
//                cout << data_double << endl;
                AllDataDouble[row - 1][col] = data_double;
//                 cout<< Vec[col] << " ";

            } else{
                AllDataDouble[row - 1][col] = median[col];
            }
           // cout << AllDataDouble[row -1][col] << " ";
        }
//        cout << AllDataDouble[0][0] << " ";
        // cout << endl;
    }

    // for(int a = 0; a < AllDataDouble.size(); a++){
    //   cout << AllDataDouble[a][112] << " ";
    // }

    /**
     * Select the combination
     * @file combination.cpp
     */
    int combm = 112;
    int combn = 110;
    vector<vector<int> > vvCombination; //Create a vector to store all combinations
    Combination(combm, combn, vvCombination);/**C(m,n), n <= m.  All combinations save to vvCombination */
    
    cout << "Combinationcount: " <<vvCombination.size() << endl;


    //create new .csv to save data, then dataset could be created
    //choose last 15 features from AllDataDouble

    // DataSetTrain(AllDataDouble, AllDataString);
    // DataSetPrediction(1, 2, 3, AllDataDouble, AllDataString);
    // CrossValidation_LOO(vvCombination, AllDataDouble, AllDataString);
   


    /**
     * Parallelization with XiTAO
	 */
    cout << "Start Parallelization!" << endl;

    int searchComb[vvCombination.size()];
    double accV[vvCombination.size()];


  // gotao_init();
	for(int i = 0; i < vvCombination.size(); i++){
		
    gotao_init();
		combTAO* combTAOx = new combTAO(combm, i, vvCombination, AllDataString, AllDataDouble);
		gotao_push(combTAOx);
		// execute the pending TAO-DAGs
  	gotao_start();  		
    gotao_fini();

  		// lossV.push_back(combTAOx);
  	searchComb[i] = i;

  	FileRead NewFile("./Temps/DataAUCTemp.csv"); 
  	vector<vector<string>> vacc = NewFile.return_strArray();
  		// double n;
  		// n = stod(vloss[0][0]);
  		// cout << vloss[0][0] << endl << endl;
  	accV[i] = stod(vacc[0][0]);

  		// cout << "The " << searchComb[i] <<"th Combination -> " << lossV[i] << endl;
	}
  // gotao_fini();


	/**
     * print the best 10 combinations
	 */
	cout << "Start loading result to Result_AUC_Featrues.csv..." << endl;
  string pathname = "./Results/" + to_string(combm) + "_" + to_string(combn) + "_" + "Result_AUC_Featrues.csv";
	ofstream Result_AUC_Featrues;
  Result_AUC_Featrues.open(pathname, ios::out);

	for(int i = 0; i < vvCombination.size(); i++){
		double comNum = 0;
		// double vtemp[20] = accV[];
		int flag = 0;
		for(int j = 0; j < vvCombination.size(); j++){
			if(accV[j] > comNum){
				comNum = accV[j];
				// vetemp[j] = -1;
				flag = j;

				// cout << vvCombination[searchComb[j]][0] <<" "
			}
		}
		
		for(int e = 0; e < vvCombination[0].size(); e++){
			int feature = vvCombination[searchComb[flag]][e] + AllDataString[0].size() - 1 - combm;
			Result_AUC_Featrues << AllDataString[0][feature] << ",";
		}
		
		Result_AUC_Featrues << ",       AUC: " << accV[flag] << endl;

		accV[flag] = -1;
	}


  	
	cout << "Done" <<endl;
  	
    return 0;
}