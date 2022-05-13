#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <filesystem>
#include "heuristic/src/Data.cpp"

using namespace std;

void getDual(string instanceSet, vector<pair<double, int>> &dual)
{
    string filepath = "objval/objval-dual-" + instanceSet;
    ifstream file, dimensionfile;
    vector<int> dimensions;

    dimensionfile.open("instances-dimension");

    if(!dimensionfile.is_open())
    {
        cout << "Problem opening dimension file for reading." << endl;
        exit(1);
    }

    while(1)
    {
        int dimension;

        dimensionfile >> dimension;
        dimensions.push_back(dimension);

        if(dimensionfile.eof())
        {
            break;
        }
    }

    file.open(filepath);

    if(!file.is_open())
    {
        cout << "Problem opening dual file for reading." << endl;
        exit(1);
    }

    string line;
    getline(file, line, ':');

    int i = 0;
    while(1)
    {
        getline(file, line, ':');
        dual.push_back(make_pair(stod(line), dimensions[i++]));

        if(file.eof())
        {
            break;
        }
    }

    file.close();
}

void getMatrixObjTimeSet(string instanceSet, vector<vector<pair<double, int>>> &matrixObjTimeSet)
{
    string objvalfilepath = "objval/objval-" + instanceSet;
    string timefilepath = "time/time-" + instanceSet;
    ifstream objvalfile, timefile, dimensionfile;
    vector<int> dimensions;

    dimensionfile.open("instances-dimension");

    if(!dimensionfile.is_open())
    {
        cout << "Problem opening dimension file for reading." << endl;
        exit(1);
    }

    while(1)
    {
        int dimension;

        dimensionfile >> dimension;
        dimensions.push_back(dimension);

        if(dimensionfile.eof())
        {
            break;
        }
    }

    objvalfile.open(objvalfilepath);

    if(!objvalfile.is_open())
    {
        cout << "Problem opening objval file for reading." << endl;
        exit(1);
    }

    string line;
    getline(objvalfile, line, ':');

    int i = 0;
    while(1)
    {
        getline(objvalfile, line, ':');
        matrixObjTimeSet[0].push_back(make_pair(stod(line), dimensions[i++]));

        if(objvalfile.eof())
        {
            break;
        }
    }

    objvalfile.close();

    timefile.open(timefilepath);

    if(!timefile.is_open())
    {
        cout << "Problem opening time file for reading." << endl;
        exit(1);
    }

    getline(timefile, line, ':');

    i = 0;
    while(1)
    {
        getline(timefile, line, ':');
        matrixObjTimeSet[1].push_back(make_pair(stod(line), dimensions[i++]));

        if(timefile.eof())
        {
            break;
        }
    }

    timefile.close();
}

bool compareIntInt(pair<int, int> p1, pair<int, int> p2) { return (p1.second <= p2.second); }
bool compareDoubleInt(pair<double, int> p1, pair<double, int> p2) { return (p1.second <= p2.second); }
bool compareStringInt(pair<string, int> p1, pair<string, int> p2) { return (p1.second <= p2.second); }

struct OptimalInfo{
    int obj;
    double time;
    string path;
};

int main()
{
    vector<string> versions{"al", "hl", "au", "balau", "baltu", "bhlau", "bhltu", "btlau", "btltu", "ial", "ihl", "itl", "iau", "itu", "f1", "f2"},
        folders{"", "heuristic/output/", "", "binary-algo-lb-algo-ub/benchmark/", "binary-algo-lb-trivial-ub/benchmark/", 
        "binary-heuristic-lb-algo-ub/benchmark/", "binary-heuristic-lb-trivial-ub/benchmark/", "binary-trivial-lb-algo-ub/benchmark/",
        "binary-trivial-lb-trivial-ub/benchmark/", "iterative-algo-lb/benchmark/", "iterative-heuristic-lb/benchmark/",
        "iterative-trivial-lb/benchmark/", "iterative-algo-ub/benchmark/", "iterative-trivial-ub/benchmark/", "f1/benchmark/", "f2/benchmark/"},
        instancesSets = {"A", "B", "C", "D"};
    vector<vector<vector<vector<pair<double, int>>>>> matrixObjTime(16,
        vector<vector<vector<pair<double, int>>>>(4, vector<vector<pair<double, int>>>(2,
        vector<pair<double, int>>(0, make_pair(0, 0)))));
    vector<vector<pair<double, int>>> dual(4, vector<pair<double, int>>(0, make_pair(0, 0))),
        dualF1(4, vector<pair<double, int>>(0, make_pair(0, 0))), dualF2(4, vector<pair<double, int>>(0, make_pair(0, 0)));
    int numOfInstances, sumSize, median;

    for(int j = 0; j < 4; j++)
    {
        for(int i = 0; i < 16; i++)
        {
            cout << versions[i] << instancesSets[j] << endl << endl;
            getMatrixObjTimeSet(versions[i] + instancesSets[j], matrixObjTime[i][j]);
            
            numOfInstances = matrixObjTime[i][j][0].size();

            for(int k = 0; k < 2; k++)
            {
                sort(matrixObjTime[i][j][k].begin(), matrixObjTime[i][j][k].end(), compareDoubleInt);
                
                cout << "\t" << k << ":\n\t\t";
                for(int l = 0; l < numOfInstances; l++)
                {
                    cout << matrixObjTime[i][j][k][l].first << "  ";
                }
                cout << endl << endl;
            }
        }
    }

    // fill instancesNames and instancesIDs
    vector<pair<string, int>> instancesNames;
    vector<pair<int, int>> instancesIDs;
    ifstream instancesNamesFile, dimensionsFile;
    instancesNamesFile.open("instances-names");

    if(!instancesNamesFile.is_open()) {
        cout << "Problem opening instances names file for reading." << endl;
        exit(1);
    }

    dimensionsFile.open("instances-dimension");

    if(!dimensionsFile.is_open()) {
        cout << "Problem opening dimensions file for reading." << endl;
        exit(1);
    }

    for(int l = 0; l < numOfInstances; l++)
    {
        string line;
        int dimension;
        getline(instancesNamesFile, line);
        dimensionsFile >> dimension;
        instancesNames.push_back(make_pair(line, dimension));
        instancesIDs.push_back(make_pair(l + 1, dimension));
    }

    sort(instancesNames.begin(), instancesNames.end(), compareStringInt);
    sort(instancesIDs.begin(), instancesIDs.end(), compareIntInt);

    instancesNamesFile.close();
    dimensionsFile.close();

    for(int l = 0; l < numOfInstances; l++)
    {
        sumSize += matrixObjTime[0][0][0][l].second;
    }
    median = matrixObjTime[0][0][0][96].second;

    vector<vector<int>> primal(4, vector<int>(numOfInstances, 0));

    cout << "dual:" << endl << endl;
    for(int j = 0; j < 4; j++)
    {
        getDual(versions[14] + instancesSets[j], dualF1[j]);
        getDual(versions[15] + instancesSets[j], dualF2[j]);
        sort(dualF1[j].begin(), dualF1[j].end(), compareDoubleInt);
        sort(dualF2[j].begin(), dualF2[j].end(), compareDoubleInt);
        dual[j] = dualF2[j];

        cout << "\t" << instancesSets[j] << ":\n\t\t";
        for(int l = 0; l < numOfInstances; l++)
        {
            if(dualF1[j][l].first < dual[j][l].first)
            {
                dual[j][l].first = dualF1[j][l].first;
            }

            for(int i = 0; i < 16; i++)
            {
                if(i < 2)
                {
                    if(matrixObjTime[i][j][0][l].first > primal[j][l])
                    {
                        primal[j][l] = matrixObjTime[i][j][0][l].first;
                    }
                }
                else if(i == 2)
                {
                    if(matrixObjTime[i][j][0][l].first < dual[j][l].first)
                    {
                        dual[j][l].first = matrixObjTime[i][j][0][l].first;
                    }
                }
                else if(i < 12 || i > 13)
                {
                    if(matrixObjTime[i][j][0][l].first > primal[j][l])
                    {
                        primal[j][l] = matrixObjTime[i][j][0][l].first;
                    }

                    if((matrixObjTime[i][j][0][l].first < dual[j][l].first)&&(matrixObjTime[i][j][1][l].first < 600))
                    {
                        // check if the instance is not the one solved linearly by F1
                        if(i != 14 || j != 2 || l != 191) {
                            dual[j][l].first = matrixObjTime[i][j][0][l].first;
                        }
                    }
                }
                else
                {
                    if((matrixObjTime[i][j][1][l].first < 600)&&(matrixObjTime[i][j][0][l].first > primal[j][l]))
                    {
                        primal[j][l] = matrixObjTime[i][j][0][l].first;
                    }

                    if(matrixObjTime[i][j][0][l].first < dual[j][l].first)
                    {
                        dual[j][l].first = matrixObjTime[i][j][0][l].first;
                    }
                }
            }

            if(dual[j][l].first < primal[j][l])
            {
                cout << endl << endl << "ERROR:" << endl;
                cout << "\tdual: " << dual[j][l].first << endl;
                cout << "\tprimal: " << primal[j][l] << endl;
                dual[j][l].first = primal[j][l];
                cout << "\t" << instancesSets[j] << ": " << l + 1 << endl;
                exit(1);
            }

            cout << dual[j][l].first << "  ";
        }
        cout << endl << endl;
    }

    vector<vector<int>> solved(16, vector<int>(4, 0));
    vector<vector<int>> numOfOptimals(16, vector<int>(4, 0));
    vector<vector<bool>> optimal(4, vector<bool>(numOfInstances, false));
    vector<vector<OptimalInfo>> optimalInfo(4, vector<OptimalInfo>(numOfInstances, {0, 1000, ""}));
    vector<vector<vector<double>>> gaps;
    vector<vector<double>> lowestGap(4, vector<double>(numOfInstances, __DBL_MAX__));
    vector<vector<double>> gapSum(16, vector<double>(4, 0));
    vector<vector<double>> timeSum(16, vector<double>(4, 0));
    vector<vector<double>> timeOptSum(16, vector<double>(4, 0));

    // graphs for each version and setup
    for(int i = 0; i < 16; i++)
    {
        ofstream gapfile;

        gapfile.open("graphs/gap/" + versions[i]);

        if(!gapfile.is_open())
        {
            cout << "Problem opening gap file for writing." << endl;
            exit(1);
        }

        ofstream timefile;

        timefile.open("graphs/time/" + versions[i]);

        if(!timefile.is_open())
        {
            cout << "Problem opening time file for writing." << endl;
            exit(1);
        }

        gapfile << "n\t&\tA\t&\tB\t&\tC\t&\tD" << endl;
        timefile << "n\t&\tA\t&\tB\t&\tC\t&\tD" << endl;

        vector<vector<double>> gapsI(4, vector<double>(0, 0));
        for(int l = 0; l < numOfInstances; l++)
        {
            if((l+1)%10 == 0)
            {
                gapfile << l+1;
                timefile << l+1;
            }

            for(int j = 0; j < 4; j++)
            {
                if((i > 2)&&(matrixObjTime[i][j][1][l].first < 600))
                {
                    // check if the instance is not the one solved linearly by F1
                    if(i != 14 || j != 2 || l != 191) {
                        optimal[j][l] = true;
                    }
                }

                double gap;

                if((i < 12)&&(i != 2))
                {
                    gap = 100*(dual[j][l].first - matrixObjTime[i][j][0][l].first)/matrixObjTime[i][j][0][l].first;

                    solved[i][j]++;
                }
                else if(i < 14)
                {
                    gap = 100*(matrixObjTime[i][j][0][l].first - primal[j][l])/primal[j][l];

                    if((matrixObjTime[i][j][1][l].first < 600)&&(i != 2))
                    {
                        solved[i][j]++;
                    }
                }
                else if(i == 14)
                {
                    gap = 100*(dualF1[j][l].first - primal[j][l])/primal[j][l];

                    if(matrixObjTime[i][j][0][l].first > 0)
                    {
                        solved[i][j]++;
                    }
                }
                else
                {
                    gap = 100*(dualF2[j][l].first - primal[j][l])/primal[j][l];

                    if(matrixObjTime[i][j][0][l].first > 0)
                    {
                        solved[i][j]++;
                    }
                }

                gapSum[i][j] += gap;

                if(gap < lowestGap[j][l])
                {
                    lowestGap[j][l] = gap;
                }
                
                if(((i < 12)&&(i != 2) || (i > 2)&&(matrixObjTime[i][j][1][l].first < 600))&&
                    (matrixObjTime[i][j][0][l].first > optimalInfo[j][l].obj ||
                    (matrixObjTime[i][j][0][l].first == optimalInfo[j][l].obj)&&(matrixObjTime[i][j][1][l].first < optimalInfo[j][l].time))) {
                    optimalInfo[j][l].obj = matrixObjTime[i][j][0][l].first;
                    optimalInfo[j][l].time = matrixObjTime[i][j][1][l].first;
                    optimalInfo[j][l].path = folders[i] + instancesSets[j] + "/";
                }

                gapsI[j].push_back(gap);
                timeSum[i][j] += matrixObjTime[i][j][1][l].first;

                gapfile << "\t&\t" << gap;
                timefile << "\t&\t" << matrixObjTime[i][j][1][l].first;
            }

            gapfile << endl;
            timefile << endl;
        }

        gaps.push_back(gapsI);
        
        gapfile.close();
        timefile.close();
    }

    // gap and time graphs for each instance set
    for(int j = 0; j < 4; j++) {
        ofstream gapfile;

        gapfile.open("graphs/gap/" + instancesSets[j]);

        if(!gapfile.is_open())
        {
            cout << "Problem opening gap file for writing." << endl;
            exit(1);
        }

        ofstream timefile;

        timefile.open("graphs/time/" + instancesSets[j]);

        if(!timefile.is_open())
        {
            cout << "Problem opening time file for writing." << endl;
            exit(1);
        }

        for(int i = 0; i < 16; i++) {
            gapfile << versions[i] << endl;
            timefile << versions[i] << endl;
            for(int l = 0; l < numOfInstances; l++) {
                gapfile << "(" << l + 1 << "," << gaps[i][j][l] << ")";
                timefile << "(" << l + 1 << "," << matrixObjTime[i][j][1][l].first << ")";
            }
            gapfile << endl << endl;
            timefile << endl << endl;
        }

        gapfile.close();
        timefile.close();
    }

    cout << "average size: " << sumSize/(float)numOfInstances << endl;
    cout << "median: " << median << endl;

    // optimals and best primal solutions files
    for(int j = 0; j < 4; j++)
    {
        ofstream out;
        out.open("optimals" + instancesSets[j]);

        if(!out.is_open())
        {
            cout << "Problem opening optimals file for writing." << endl;
            exit(1);
        }

        for(int l = 0; l < numOfInstances; l++)
        {
            out <<  primal[j][l];

            if(!optimal[j][l])
            {
                out <<  "*";
            }

            out << endl;
        }
        
        out.close();
    
        for(int l = 0; l < numOfInstances; l++)
        {
            Data data("instances/" + instancesSets[j] + "/" + instancesNames[l].first);
            
            if(optimalInfo[j][l].path[0] == 'h')
            {
                ifstream in;
                in.open(optimalInfo[j][l].path + instancesNames[l].first);

                if(!in.is_open())
                {
                    cout << "Problem opening best solution instance file for reading." << endl;
                    exit(1);
                }
                
                out.open("results/best-primal-sols/" + instancesSets[j] + "/" + instancesNames[l].first);

                if(!out.is_open())
                {
                    cout << "Problem opening best solution instance file for writing." << endl;
                    exit(1);
                }

                double time;
                in >> time;

                out << optimalInfo[j][l].path.substr(0, optimalInfo[j][l].path.find("/")) << endl;
                int p = 0, f;
                vector<int> iterFamily(data.getFamiliesSize(), 0);
                in >> f;
                while(!in.eof())
                {
                    out << "Position " << ++p << ":\t" << data.getFamilyMember(f, iterFamily[f]++) << endl;
                    in >> f;
                }

                out << "Maximum profit:\t" << p << endl;
                out << "Time:\t" << time << endl;

                in.close();
                out.close();
            }
            else {
                ifstream in;
                in.open(optimalInfo[j][l].path + "bm-" + to_string(instancesIDs[l].first));

                if(!in.is_open())
                {
                    cout << "Problem opening best solution instance file for reading." << endl;
                    exit(1);
                }
                
                string line;
                vector<int> s;
                while(getline(in, line))
                {
                    if(line.find("Position 1:") != string::npos)
                    {
                        s.push_back(stoi(line.substr(line.find("\t") + 1)));
                        break;
                    }
                }

                while(getline(in, line))
                {
                    if(line.find("Position") == string::npos)
                    {
                        break;
                    }
                    
                    s.push_back(stoi(line.substr(line.find("\t") + 1)));
                }

                if(optimalInfo[j][l].path.find("heuristic") != string::npos ||
                    optimalInfo[j][l].path.find("f2") != string::npos)
                {
                    vector<int> updatedS;    
                    vector<int> iterFamily(data.getFamiliesSize(), 0);
                    for(int p = 0; p < s.size(); p++)
                    {
                        if(s[p] >= data.getFamiliesSize())
                        {
                            break;
                        }

                        int v = data.getFamilyMember(s[p], iterFamily[s[p]]++);
                        if(v == -1)
                        {
                            break;
                        }
                        updatedS.push_back(v);
                    }

                    if(updatedS.size() == s.size())
                    {
                        s = updatedS;
                    }
                }
                
                out.open("results/best-primal-sols/" + instancesSets[j] + "/" + instancesNames[l].first);

                if(!out.is_open())
                {
                    cout << "Problem opening best solution instance file for writing." << endl;
                    exit(1);
                }

                out << optimalInfo[j][l].path.substr(0, optimalInfo[j][l].path.find("/")) << endl;
                for(int p = 0; p < s.size(); p++)
                {
                    out << "Position " << p + 1 << ":\t" << s[p] << endl;
                }

                out << line << endl;
                getline(in, line);
                out << line << endl;

                in.close();
                out.close();
            }
        }
    }

    // latex tables
    for(int j = 0; j < 4; j++)
    {
        ofstream tablefile;
        tablefile.open("tables/table" + instancesSets[j]);

        if(!tablefile.is_open())
        {
            cout << "Problem opening table file for writing." << endl;
            exit(1);
        }

        tablefile << "\\hline" << endl;
        tablefile << "search\t&\tlb\t&\tub\t&\t\\#primal\t&\t\\#opt\t&\t\\#lowest\t&\toutput\t&\tgap (\\%)\t&\tT (s)\t&\tT$_\\text{p}$ (s)\\\\" << endl;
        tablefile << "\\hline" << endl;
        
        tablefile << fixed;
        tablefile << setprecision(2);

        for(int i = 0; i < 16; i++)
        {
            int wordSearchSize = versions[i].size() - 1;
            for(int k = 0; k < wordSearchSize; k++)
            {
                if(wordSearchSize == 1)
                {
                    if(versions[i][0] == 'f')
                    {
                        tablefile << "\\hline" << endl;
                        tablefile << string("\\multicolumn{3}{c|}{F") + versions[i][k + 1] + "}\t&\t";
                    }
                    else if(versions[i][0] == 'h')
                    {
                        tablefile << "\\multicolumn{1}{c}{-}\t&\theuristic\t&\t\\multicolumn{1}{c|}{-}\t&\t";
                    }
                    else
                    {
                        if(versions[i][1] == 'l')
                        {
                            tablefile << "\\multicolumn{1}{c}{-}\t&\tcombinatorial\t&\t\\multicolumn{1}{c|}{-}\t&\t";
                        }
                        else
                        {
                            tablefile << "\\multicolumn{1}{c}{-}\t&\t\\multicolumn{1}{c}{-}\t&\tcombinatorial\t&\t";
                        }
                    }

                    break;
                }

                if(versions[i][k] == 'b')
                {
                    tablefile << "binary\t&\t";
                }
                else if(versions[i][k] == 'i')
                {
                    tablefile << "iterative\t&\t";

                    if(versions[i][2] == 'u')
                    {
                        tablefile << "\\multicolumn{1}{c}{-}\t&\t";
                    }
                }
                
                if(versions[i][k] == 'a')
                {
                    tablefile << "combinatorial\t&\t";
                }
                else if(versions[i][k] == 'h')
                {
                    tablefile << "heuristic\t&\t";
                }
                else if(versions[i][k] == 't')
                {
                    tablefile << "trivial\t&\t";
                }

                if((versions[i][0] == 'i')&&(versions[i][k + 1] == 'l'))
                {
                    tablefile << "\\multicolumn{1}{c|}{-}\t&\t";
                }
            }

            int noOfLowestGaps = 0;
            for(int l = 0; l < numOfInstances; l++)
            {
                if(gaps[i][j][l] == lowestGap[j][l])
                {
                    noOfLowestGaps++;

                    if(optimal[j][l])
                    {
                        numOfOptimals[i][j]++;
                        timeOptSum[i][j] += matrixObjTime[i][j][1][l].first;
                    }
                }
            }
            
            // #primal
            tablefile << solved[i][j] << "\t&\t";

            // #optimal
            tablefile << numOfOptimals[i][j] << "\t&\t";

            // lowest gap
            tablefile << noOfLowestGaps << "\t&\t";

            // output
            if((i < 12)&&(i != 2))
            {
                tablefile << "primal\t&\t";
            }
            else
            {
                tablefile << "dual\t&\t";
            }

            // gap
            tablefile << gapSum[i][j]/numOfInstances << "\t&\t";

            // time (solved)
            tablefile << timeSum[i][j]/numOfInstances << "\t&\t";

            // time (optimal)
            tablefile << timeOptSum[i][j]/numOfOptimals[i][j] << "\\\\" << endl;
        }

        tablefile << "\\hline" << endl;

        tablefile.close();
    }

    // csv tables
    for(int j = 0; j < 4; j++) {
        ofstream tablefile;
        tablefile.open("tables/obj-time-" + instancesSets[j]);

        if(!tablefile.is_open())
        {
            cout << "Problem opening table file for writing." << endl;
            exit(1);
        }

        for(int l = 0; l < numOfInstances; l++)
        {
            tablefile << instancesNames[l].first << "\t";
            for(int i = 0; i < 14; i++)
            {
                tablefile << matrixObjTime[i][j][0][l].first <<  "\t" << matrixObjTime[i][j][1][l].first << "\t";
            }
            tablefile << dualF1[j][l].first << "\t" << matrixObjTime[14][j][1][l].first << "\t";
            tablefile << dualF2[j][l].first << "\t" << matrixObjTime[14][j][1][l].first << "\t";
            tablefile << primal[j][l] << "\t" << dual[j][l].first << endl;
        }
    }

    return 0;
}