#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Output
{
    double objVal;
    double time;
    Output(double ov, double t)
    {
        objVal = ov;
        time = t;
    }
};

void storeBenchmark(string filePath, vector<vector<Output>>& benchmarkVec)
{
    ifstream objFile, timeFile;
    objFile.open(filePath + "-objval");
    timeFile.open(filePath + "-time");

    if(!objFile.is_open())
    {
        std::cout << "Problem opening " << filePath << "-objval for reading." << std::endl;
        exit(1);
    }
    
    if(!timeFile.is_open())
    {
        std::cout << "Problem opening " << filePath << "-time for reading." << std::endl;
        exit(1);
    }

    vector<Output> vals;
    for(int i = 0; ; i++)
    {
        double objVal, time;
        objFile >> objVal;
        timeFile >> time;

        if(objFile.eof() || timeFile.eof())
        {
            break;
        }

        vals.push_back(Output(objVal, time));
    }

    benchmarkVec.push_back(vals);
}

int main()
{
    vector<vector<Output>> perturb5, perturb5Removal, perturb5Swap, perturb10, perturb10Removal, perturb10Swap,
        perturb15, perturb15Removal, perturb15Swap, perturb20, perturb20Removal, perturb20Swap, perturb25, perturb25Removal,
        perturb25Swap, perturb50, perturb50Removal, perturb50Swap;
    vector<string> instancesSets = {"A", "B", "C", "D"};

    int setsSize = instancesSets.size();
    int numOfInstances = 193;
    for(int set = 0; set < setsSize; set++)
    {
        storeBenchmark("heuristic-calibration/perturb-j_5/benchmark/" + instancesSets[set], perturb5);
        storeBenchmark("heuristic-calibration/perturb-j_5-removal/benchmark/" + instancesSets[set], perturb5Removal);
        storeBenchmark("heuristic-calibration/perturb-j_5-swap/benchmark/" + instancesSets[set], perturb5Swap);
        storeBenchmark("heuristic-calibration/perturb-j_10/benchmark/" + instancesSets[set], perturb10);
        storeBenchmark("heuristic-calibration/perturb-j_10-removal/benchmark/" + instancesSets[set], perturb10Removal);
        storeBenchmark("heuristic-calibration/perturb-j_10-swap/benchmark/" + instancesSets[set], perturb10Swap);
        storeBenchmark("heuristic-calibration/perturb-j_15/benchmark/" + instancesSets[set], perturb15);
        storeBenchmark("heuristic-calibration/perturb-j_15-removal/benchmark/" + instancesSets[set], perturb15Removal);
        storeBenchmark("heuristic-calibration/perturb-j_15-swap/benchmark/" + instancesSets[set], perturb15Swap);
        storeBenchmark("heuristic-calibration/perturb-j_20/benchmark/" + instancesSets[set], perturb20);
        storeBenchmark("heuristic-calibration/perturb-j_20-removal/benchmark/" + instancesSets[set], perturb20Removal);
        storeBenchmark("heuristic-calibration/perturb-j_20-swap/benchmark/" + instancesSets[set], perturb20Swap);
        storeBenchmark("heuristic-calibration/perturb-j_25/benchmark/" + instancesSets[set], perturb25);
        storeBenchmark("heuristic-calibration/perturb-j_25-removal/benchmark/" + instancesSets[set], perturb25Removal);
        storeBenchmark("heuristic-calibration/perturb-j_25-swap/benchmark/" + instancesSets[set], perturb25Swap);
        storeBenchmark("heuristic-calibration/perturb-j_50/benchmark/" + instancesSets[set], perturb50);
        storeBenchmark("heuristic-calibration/perturb-j_50-removal/benchmark/" + instancesSets[set], perturb50Removal);
        storeBenchmark("heuristic-calibration/perturb-j_50-swap/benchmark/" + instancesSets[set], perturb50Swap);
        
        // output
        ofstream objOutput, timeOutput;
        objOutput.open("heuristic-calibration/tables/objval/perturb" + instancesSets[set]);
        timeOutput.open("heuristic-calibration/tables/time/perturb" + instancesSets[set]);

        for(int i = 0; i < numOfInstances; i++)
        {
            objOutput << perturb5[set][i].objVal << "\t"
                << perturb10[set][i].objVal << "\t"
                << perturb15[set][i].objVal << "\t"
                << perturb20[set][i].objVal << "\t"
                << perturb25[set][i].objVal << "\t"
                << perturb50[set][i].objVal << endl;

            timeOutput << perturb5[set][i].time << "\t"
                << perturb10[set][i].time << "\t"
                << perturb15[set][i].time << "\t"
                << perturb20[set][i].time << "\t"
                << perturb25[set][i].time << "\t"
                << perturb50[set][i].time << endl;
        }

        objOutput.close();
        timeOutput.close();
        objOutput.open("heuristic-calibration/tables/objval/perturb-removal" + instancesSets[set]);
        timeOutput.open("heuristic-calibration/tables/time/perturb-removal" + instancesSets[set]);

        for(int i = 0; i < numOfInstances; i++)
        {
            objOutput << perturb5Removal[set][i].objVal << "\t"
                << perturb10Removal[set][i].objVal << "\t"
                << perturb15Removal[set][i].objVal << "\t"
                << perturb20Removal[set][i].objVal << "\t"
                << perturb25Removal[set][i].objVal << "\t"
                << perturb50Removal[set][i].objVal << endl;
            
            timeOutput << perturb5Removal[set][i].time << "\t"
                << perturb10Removal[set][i].time << "\t"
                << perturb15Removal[set][i].time << "\t"
                << perturb20Removal[set][i].time << "\t"
                << perturb25Removal[set][i].time << "\t"
                << perturb50Removal[set][i].time << endl;
        }

        objOutput.close();
        timeOutput.close();
        objOutput.open("heuristic-calibration/tables/objval/perturb-swap" + instancesSets[set]);
        timeOutput.open("heuristic-calibration/tables/time/perturb-swap" + instancesSets[set]);

        for(int i = 0; i < numOfInstances; i++)
        {
            objOutput << perturb5Swap[set][i].objVal << "\t"
                << perturb10Swap[set][i].objVal << "\t"
                << perturb15Swap[set][i].objVal << "\t"
                << perturb20Swap[set][i].objVal << "\t"
                << perturb25Swap[set][i].objVal << "\t"
                << perturb50Swap[set][i].objVal << endl;

            timeOutput << perturb5Swap[set][i].time << "\t"
                << perturb10Swap[set][i].time << "\t"
                << perturb15Swap[set][i].time << "\t"
                << perturb20Swap[set][i].time << "\t"
                << perturb25Swap[set][i].time << "\t"
                << perturb50Swap[set][i].time << endl;
        }

        objOutput.close();
        timeOutput.close();
    }

    vector<vector<Output>> ir5ilsj, ir5ilsj2, ir10ilsj, ir10ilsj2, ir15ilsj, ir15ilsj2;

    for(int set = 0; set < setsSize; set++)
    {
        storeBenchmark("heuristic-calibration/ir-5-ils-j/benchmark/" + instancesSets[set], ir5ilsj);
        storeBenchmark("heuristic-calibration/ir-5-ils-j_2/benchmark/" + instancesSets[set], ir5ilsj2);
        storeBenchmark("heuristic-calibration/ir-10-ils-j/benchmark/" + instancesSets[set], ir10ilsj);
        storeBenchmark("heuristic-calibration/ir-10-ils-j_2/benchmark/" + instancesSets[set], ir10ilsj2);
        storeBenchmark("heuristic-calibration/ir-15-ils-j/benchmark/" + instancesSets[set], ir15ilsj);
        storeBenchmark("heuristic-calibration/ir-15-ils-j_2/benchmark/" + instancesSets[set], ir15ilsj2);

        // output
        ofstream objOutput, timeOutput;
        objOutput.open("heuristic-calibration/tables/objval/ir-ils" + instancesSets[set]);
        timeOutput.open("heuristic-calibration/tables/time/ir-ils" + instancesSets[set]);

        for(int i = 0; i < numOfInstances; i++)
        {
            objOutput << ir5ilsj[set][i].objVal << "\t"
                << ir5ilsj2[set][i].objVal << "\t"
                << ir10ilsj[set][i].objVal << "\t"
                << ir10ilsj2[set][i].objVal << "\t"
                << ir15ilsj[set][i].objVal << "\t"
                << ir15ilsj2[set][i].objVal << endl;

            timeOutput << ir5ilsj[set][i].time << "\t"
                << ir5ilsj2[set][i].time << "\t"
                << ir10ilsj[set][i].time << "\t"
                << ir10ilsj2[set][i].time << "\t"
                << ir15ilsj[set][i].time << "\t"
                << ir15ilsj2[set][i].time << endl;
        }

        objOutput.close();
        timeOutput.close();
    }

    return 0;
}