#include "library.h"
#include "json/json.h"
#include <string.h>
#include <iostream>
#include <sstream>
#include <map>

using namespace std;

map<int,double> loadsum;
map<int,int> countsum;
map<string,double> originDataMap;
map<int,double> realLoadSum;
map<int,int>realCountSum;
map<int,double> tmpResultMap;

vector<string> split(string strContent,string mark) {
    string::size_type pos;
    vector<string> result;
    strContent += mark;
    int size = strContent.size();
    for(int i=0; i<size; i++)
    {
        pos = strContent.find(mark,i);
        if(pos<size)
        {
            string s = strContent.substr(i,pos-i);
            result.push_back(s);
            i = pos+mark.size()-1;
        }
    }
    return result;
}

double *initial() {
    loadsum.clear();
    countsum.clear();
    originDataMap.clear();
    tmpResultMap.clear();
    realCountSum.clear();
    realLoadSum.clear();
    double *p = new double;
//    p = NULL;
    return p;
}

int feed(int toolNo,double load, double *p) {
    if (load != 0) {
        if (countsum.count(toolNo)) {
            loadsum[toolNo] = loadsum[toolNo] + load;
            countsum[toolNo] = countsum[toolNo] + 1;
        } else {
            loadsum[toolNo] = load;
            countsum[toolNo] = 1;
        }

        stringstream ssToolNo;
        stringstream ssCountNum;
        string strToolNo;
        string strCountNum;
        string tmpToolNo = "";
        ssToolNo<<toolNo;
        ssToolNo>>strToolNo;
        ssCountNum<<countsum[toolNo];
        ssCountNum>>strCountNum;
        tmpToolNo = strToolNo + "|" + strCountNum;
        originDataMap[tmpToolNo] = load;
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
//        Json::Value tmpObj;
        int toolNo = it->first;
//        cout<<"toolNO:"<<toolNo<<endl;
        double dResult;
        dResult = loadsum[toolNo]/countsum[toolNo];
//        cout<<"load result:"<<dResult<<endl;
        tmpResultMap[toolNo] = dResult;
//        tmpObj["toolnum"] = toolNo;
//        tmpObj["load"] = dResult;
//        arrayRoot.append(tmpObj);
    }

    map<int,double>::iterator itTmpResult;
    for (itTmpResult=tmpResultMap.begin();itTmpResult != tmpResultMap.end();++itTmpResult) {
        int tmpToolNo = itTmpResult->first;
        double tmpAvar = itTmpResult->second;
        map<string,double>::iterator itOriginData;
        for (itOriginData = originDataMap.begin();itOriginData != originDataMap.end();++itOriginData) {
            map<string,double>::iterator it_backup = itOriginData;
            string tmpStrName = itOriginData->first;
            vector<string> splitData = split(tmpStrName,"|");
            string tmpStrToolNo = splitData[0];
            stringstream ssToolNo;
            int tmpiToolNo;
            ssToolNo<<tmpStrToolNo;
            ssToolNo>>tmpiToolNo;
            double tmpVal = itOriginData->second;
            if (tmpiToolNo == tmpToolNo) {
                if ((tmpAvar*3) <= tmpVal) {

                } else {
                    if (realLoadSum.count(tmpToolNo)) {
                        realLoadSum[tmpToolNo] = realLoadSum[tmpToolNo] + tmpVal;
                        realCountSum[tmpToolNo] = realCountSum[tmpToolNo] + 1;
                    } else {
                        realLoadSum[tmpToolNo] = tmpVal;
                        realCountSum[tmpToolNo] = 1;
                    }
                }
            }
        }

        map<int,double>::iterator itReal;
        for (itReal=realLoadSum.begin();itReal!=realLoadSum.end();++itReal) {
            int finalToolNo = itReal->first;
            double finalResutlt = itReal->second/realCountSum[finalToolNo];
            cout<<"final toolNo:"<<finalToolNo<<endl;
            cout<<"final load Result:"<<finalResutlt<<endl;
            Json::Value tmpObj;
            tmpObj["toolnum"] = finalToolNo;
            tmpObj["load"] = finalResutlt;
            arrayRoot.append(tmpObj);
        }

    }

    root["result"] = arrayRoot;
    string strData = root.toStyledString();
    memcpy(chData,(char *)strData.c_str(),strData.length());
    cout<<"ret result:"<<chData<<endl;
    char *retData = chData;
    return retData;
}