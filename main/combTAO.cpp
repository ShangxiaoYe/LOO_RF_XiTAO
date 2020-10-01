#include <iostream>
#include "xitao.h"
#include "DataSet.cpp"

using namespace std;

class combTAO : public AssemblyTask {
  // the data
  int combm;
  int str;
  vector<vector<int>> vi;
  vector<vector<string>> vc;
  vector<vector<double>> vd;

public:
  combTAO(int combm, int str, vector<vector<int>> vi, vector<vector<string>> vc, 
    vector<vector<double>> vd) : AssemblyTask(1), combm(combm),str(str),vi(vi),vc(vc),vd(vd) { }

  void execute(int nthread) {
    int tid = nthread - leader;
    if(tid == 0){
      

      // cout << "Test" << endl;
      double auc = CrossValidation_LOO(combm, str, vi, vd, vc);
      // cout << acc << " Test" <<endl;

      ofstream outFile;
      outFile.open("./Temps/DataAUCTemp.csv", ios::out);
      outFile << auc << endl;
      outFile.close();
      
    }
    // cout << endl;
  }
  void cleanup () { }
};

