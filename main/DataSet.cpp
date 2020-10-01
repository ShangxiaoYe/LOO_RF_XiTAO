#include <iostream>
#include <sstream>
#include <fstream>
#include <shark/Data/Csv.h> //importing the file
#include <shark/Statistics/ROC.h>
#include <shark/Algorithms/Trainers/RFTrainer.h> //the random forest trainer
#include <shark/ObjectiveFunctions/Loss/ZeroOneLoss.h> //zero one loss for evaluation
#include <shark/ObjectiveFunctions/NegativeAUC.h> //AUC calculate



using namespace std;
using namespace shark;

void DataSetAll(int row, int combm, int i, vector<vector<int>> vi, vector<vector<double>> v, vector<vector<string>> v2, string str){
	ofstream outFile;
	outFile.open(str, ios::out);
	// outFile << "4" << endl;
	// cout << "test3" << endl;
	vector<double> VD;
	vector<vector<double>> VVd;
	for(int a = 0; a < vi[0].size(); a++){
		int j;
		vector<double> Vd;
		int b = v[0].size() -1 - combm;
		// cout<< b << endl;
		for(j = 0; j < v.size() ; j++){
			if(j != row) {
				Vd.push_back(v[j][vi[i][a] + b]);
				int aaaa = v2[0].size() - 1;
				VD.push_back(v[j][aaaa]);
			}
			// cout << "i: " << i << " j: " << j <<" v[j][vi[i][a]+b] " << v[j][vi[i][a]+b] << endl;
		}
		Vd.push_back(v[row][vi[i][a] + b]);
		VD.push_back(v[row][v2[0].size()-1]);
		VVd.push_back(Vd);
	}
		
	VVd.push_back(VD);


	for(int c = 0; c < VVd[0].size(); c++){
		for(int e = 0; e < VVd.size(); e++){
			string datasave = to_string(VVd[e][c]);
			if(e == VVd.size() - 1){
				outFile << datasave << endl;
				// cout << datasave << endl;
			} else{
				outFile << datasave << ",";
				// cout << datasave << ",";
			}
			
		}		

	}
	outFile.close();

}







double CrossValidation_LOO(int combm, int i, vector<vector<int>> vi, vector<vector<double>> vd, vector<vector<string>> vs){

	double AUC = 0;
	double result = 0;


    string path_prediction = "./Temps/DataSetAllTemp.csv";
    ofstream initprediction("./Temps/DataPredictionTemp.csv");
    initprediction.trunc;
    initprediction.close();
    
    for(int row = 0; row < vd.size(); row++){
    	DataSetAll(row, combm, i, vi, vd, vs, path_prediction);
		// cout << "DataSetPredictionTemp success"<< endl;

    	ClassificationDataset data;
		importCSV(data, path_prediction, LAST_COLUMN, ',');
		// cout<< "DataSetPredictionTemp readed" << endl;
		ClassificationDataset dataTest = splitAtElement(data,vd.size() - 1);
		RFTrainer<unsigned int> ptrainer;
		RFClassifier<unsigned int> pmodel;
		ptrainer.train(pmodel, data);
		// cout << "Random forest success" << endl;

		auto prediction = pmodel(dataTest.inputs());

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




    // vector<int> diagnose(vd.size());
    // for(int a = 0; a < vd.size(); a++){
    // 	// string diag = vs[a][vs.size() - 1];
    // 	// int diagno = stoi(diag);
    // 	// diagnose.push_back(diagno);
    // 	cout << vd[a][vd[0].size() - 1] << " ";
    // }
    // cout << endl;

    unsigned int labels[vd.size()];	//vd.size() = 66
    Data<unsigned int> label(vd.size(),0);
    for(size_t a = 0; a < vd.size(); a++){
    	labels[a] = int(vd[a][vd[0].size() - 1]);
    	label.element(a) = labels[a];
    	// cout << label.element(a) << " ";
    }

    // cout << endl;

    ////AUC test  @result is good!
    Data<RealVector> pred(10,RealVector(1));
	Data<unsigned int> lab(10,0);

	double values[10] = { 0 };
	unsigned int labs[10] = {1};
	
	for(std::size_t i=0; i<10; i++) {
		pred.element(i)(0)= values[i];
		lab.element(i) = labs[i];
	}




    NegativeAUC<unsigned int, RealVector> auc;
    const double vvv = 0.0;
    AUC = auc.eval(label, predictioncal);
    result = vvv - AUC;
    // cout << "test auc = " << result << endl;
    	

  
   
	return result;
	// cout << i << endl;
}
