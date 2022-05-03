#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

struct HeuristicData {
    int objVal;
    double time;
    HeuristicData(int ov, double t) {
        objVal = ov;
        time = t;
    }
};

HeuristicData readHeuristicFile(string filePath) {
    ifstream file;
    file.open(filePath);

    if(!file.is_open()) {
        cout << "Error opening heuristic file for reading." << endl;
        exit(1);
    }
    
    int objVal = 0;
    double time;
    string ignoreLine;

    file >> time;

    while(getline(file, ignoreLine)) {
        objVal++;
    }
    objVal--;

    file.close();

    HeuristicData heuristicData(objVal, time);

    return heuristicData;
}

void readInstancesNames(vector<string>& instancesNames) {
    ifstream file;
    file.open("instances-names");

    if(!file.is_open()) {
        cout << "Error opening instances names file for reading." << endl;
        exit(1);
    }

    for(int i = 0; i < 193; i++) {
        string line;
        getline(file, line);
        instancesNames.push_back(line);
    }

    file.close();
}

void writeObjFile(vector<HeuristicData>& heuristicData, string set) {
    ofstream file;
    file.open("objval/objval-hl" + set);

    if(!file.is_open()) {
        cout << "Error opening obj file for writing." << endl;
        exit(1);
    }

    for(int i = 0; i < 193; i++) {
        file << "Maximum profit:\t" << heuristicData[i].objVal << endl;
    }

    file.close();

    return;
}

void writeTimeFile(vector<HeuristicData>& heuristicData, string set) {
    ofstream file;
    file.open("time/time-hl" + set);

    if(!file.is_open()) {
        cout << "Error opening time file for writing." << endl;
        exit(1);
    }

    for(int i = 0; i < 193; i++) {
        file << "Time:\t" << heuristicData[i].time << endl;
    }

    file.close();

    return;
}

void writeObjTimeFiles(vector<HeuristicData>& heuristicData, string set) {
    writeObjFile(heuristicData, set);
    writeTimeFile(heuristicData, set);

    return;
}

int main() {
    vector<string> instancesNames, sets = {"A", "B", "C", "D"};

    readInstancesNames(instancesNames);

    for(int j = 0; j < 4; j++) {
        vector<HeuristicData> setData;
        for(int i = 0; i < 193; i++) {
            HeuristicData instanceData = 
                readHeuristicFile("heuristic/output/" + sets[j] + "/" + instancesNames[i]);
            setData.push_back(instanceData);
        }
        writeObjTimeFiles(setData, sets[j]);
    }

    return 0;
}