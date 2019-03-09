#include "library.h"
#include "json/json.h"
#include <string.h>
#include <iostream>
#include <sstream>
#include <map>

using namespace std;

map<int,double> loadsum;
map<int,int> countsum;
//void hello() {
//    std::cout << "Hello, World!" << std::endl;
//}

double *initial() {
    loadsum.clear();
    countsum.clear();
    double *p = new double;
//    p = NULL;
    return p;
}

int feed(int toolNo,double load, double *p) {
    if (countsum.count(toolNo)) {
        loadsum[toolNo] = loadsum[toolNo] + load;
        countsum[toolNo] = countsum[toolNo] + 1;
    } else {
        loadsum[toolNo] = load;
        countsum[toolNo] = 1;
    }
    return 0;
}

char *result(double *p) {
    char chData[2000];
    memset(chData,0,2000);
    Json::Value root;
    Json::Value arrayRoot;
    map<int,int>::iterator it;
    for (it=countsum.begin();it!=countsum.end();++it) {
        Json::Value tmpObj;
        int toolNo = it->first;
        cout<<"toolNO:"<<toolNo<<endl;
        double dResult;
        dResult = loadsum[toolNo]/countsum[toolNo];
        cout<<"load result:"<<dResult<<endl;
        tmpObj["toolnum"] = toolNo;
        tmpObj["load"] = dResult;
        arrayRoot.append(tmpObj);
    }
    root["result"] = arrayRoot;
    string strData = root.toStyledString();
    memcpy(chData,(char *)strData.c_str(),strData.length());
    cout<<"ret result:"<<chData<<endl;
    char *retData = chData;
    return retData;
}