//
// Created by yeshangxiao on 2020/8/28.
//

#ifndef MAIN_FILEREAD_H
#define MAIN_FILEREAD_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>


using namespace std;
class FileRead{

private:
    vector<vector<string>> strArray;
    string str="";

public:
    FileRead(string temp) :str(temp){
    }

    bool read_file(){
        ifstream inFile(str, ios::in);
        string lineStr;
        char flag=',';
        if(inFile.fail()){
            cout <<"Read files fail....."<<endl;
            return false;
        }

        while (getline(inFile, lineStr)) {
            stringstream ss(lineStr);
            string str;
            vector<string> lineArray;
            // cut apart by flag
            while (getline(ss, str, flag))
                lineArray.push_back(str);
            strArray.push_back(lineArray);
        }
        return true;
    }


    /**
     * 将整个容器返回  Return the whole vector
     * @return vector<vector<string>>
     */
    vector<vector<string>> return_strArray(){
        if(!read_file()){
            cout<<"No array!"<<endl;
            cout<<"print array fail!"<<endl;

        }
        vector<string>::iterator it;
        vector<vector<string>>::iterator iter;
        vector<string> vec_tmp;

        return strArray;
    }
};


#endif //MAIN_FILEREAD_H
