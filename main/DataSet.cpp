#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>

#include <shark/Data/Csv.h> //importing the file
#include <shark/Statistics/ROC.h>
#include <shark/Algorithms/Trainers/RFTrainer.h> //the random forest trainer
#include <shark/ObjectiveFunctions/Loss/ZeroOneLoss.h> //zero one loss for evaluation
#include <shark/ObjectiveFunctions/NegativeAUC.h> //AUC calculate



using namespace std;
using namespace shark;


void DataSetAll(int row, int combm, int i, vector<vector<int>> vi, /*vector<vector<double>> v,*/ vector<vector<string>> v2, string str){
	double column[v2.size() - 2]; //save all data for each features  65
    double median[vi[0].size()];   //median of vi[0].size() features  3  4 ...
    int b = v2[0].size() - combm - 2;
    for(int col = 0; col < vi[0].size(); col++){
    	
        int Effective_Number = 0;
        for(int r = 1; r < v2.size(); r++){
            if (v2[r][vi[r][col] + b] != "NA" && r != row){
                double data_double;
//                 cout<< stod(AllDataString[row][col]) << " ";
                data_double = stod(v2[r][vi[r][col] + b]);
                column[r - 1] = data_double;
//                 cout<< a[r - 1] << " ";
                Effective_Number++;
            }
        }
        median[col] = EvaluateMedian(column, Effective_Number);
        // cout<< "median_" << median[col]<<" ";
    }
    // cout << endl;

    ofstream outFile;
	outFile.open(str, ios::out);
    for(int a  = 0; a < v2.size() - 1; a++){
    	if(a != row){
	    	for(int j = 0; j < vi[0].size(); j++){
	    		if(v2[a + 1][vi[row][j] + b] == "NA") {
	    			outFile << median[j] << ",";
	    		} else{
	    			outFile << v2[a +1][vi[row][j] + b] << ",";
	    		}			
	    	}
	    	outFile << v2[a +1][v2[0].size() - 1] << endl;
    	}
    }
    for(int j = 0; j < vi[0].size(); j++){
    	if(v2[row + 1][vi[row][j]] == "NA") {
	    	outFile << median[j] << ",";
	    } else{
	    	outFile << v2[row +1][vi[row][j] + b] << ",";
	    }
    }
    outFile << v2[row + 1][v2[0].size() - 1] << endl;
	

	outFile.close();

}







double CrossValidation_LOO(int combm, int i, vector<vector<int>> vi,  vector<vector<string>> vs){

	double AUC = 0;
	double result = 0;


    string path_prediction = "./Temps/DataSetAllTemp.csv";
    ofstream initprediction("./Temps/DataPredictionTemp.csv");
    initprediction.trunc;
    initprediction.close();
    
    // Data<RealVector> predictioncalculate(vs.size() - 1, RealVector(1));
    for(int row = 0; row < vs.size() - 1; row++){
    	DataSetAll(row, combm, i, vi, /*vd,*/ vs, path_prediction);
		// cout << "DataSetPredictionTemp success"<< endl;

    	ClassificationDataset data;
		importCSV(data, path_prediction, LAST_COLUMN, ',');
		// cout<< "DataSetPredictionTemp readed" << endl;
		ClassificationDataset dataTest = splitAtElement(data,vs.size() - 2);
		RFTrainer<unsigned int> ptrainer;
		RFClassifier<unsigned int> pmodel;
		ptrainer.train(pmodel, data);
		// cout << "Random forest success" << endl;

		auto prediction = pmodel(dataTest.inputs());
		// predictioncalcul.element(a)(0) = prediction;

		ofstream outFile;
		outFile.open("./Temps/DataPredictionTemp.csv", ios::app);
		outFile << prediction;
		outFile.close();
		// cout << "Write prediction into csv" << endl;
    }


	FileRead Datapredictiontemp("./Temps/DataPredictionTemp.csv");
    vector<vector<string>> datapredictiontemp = Datapredictiontemp.return_strArray();  
    vector<vector<double>> datapredictiontempd(datapredictiontemp.size(), vector<double>(datapredictiontemp[0].size()));
    for(int a = 0; a < datapredictiontemp.size(); a++){
    	for(int b = 0; b < datapredictiontemp[0].size(); b++){
    		datapredictiontempd[a][b] = stod(datapredictiontemp[a][b]); 
    		// cout << datapredictiontempd[a][b] << " ";
    	}
    	// cout << endl;
    }
    // cout << "read prediction" << endl;

    double predictionvalues[datapredictiontempd.size()];
    for(int a = 0; a < datapredictiontempd.size(); a++){
    	predictionvalues[a] = datapredictiontempd[a][0];
    	// cout << predictionvalues[a] << " ";
    }

    // cout << endl;
    int num = datapredictiontempd.size();
    Data<RealVector> predictioncal(num, RealVector(1));
    for(size_t a = 0; a < num; a++){
    	predictioncal.element(a)(0) = predictionvalues[a];
    	// cout << predictioncal.element(a)(0) << " ";
    }
    // cout << endl;
	// // cout << prediction;

    unsigned int labels[vs.size() - 1];	//vd.size() = 66
    Data<unsigned int> label(vs.size() - 1,0);
    for(size_t a = 0; a < vs.size() - 1; a++){
    	double ddddd = stod(vs[a + 1][vs[0].size() - 1]);
    	labels[a] = int(ddddd);
    	// labels[a] = stoi(vs[a][vs[0].size() - 1]);
    	label.element(a) = labels[a];
    	// cout << label.element(a) << " ";
    }

    // cout << endl;


    //AUC 
    NegativeAUC<unsigned int, RealVector> auc;
    const double vvv = 0.0;
    AUC = auc.eval(label, predictioncal);
    result = vvv - AUC;
    // cout << "test auc = " << result << endl;
    	

  
   
	return result;
	// cout << i << endl;
}
