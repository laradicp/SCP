#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>

void getDual(std::string instanceSet, std::vector<std::pair<double, int>> &dual)
{
    std::string filepath = "objval/objval-dual" + instanceSet;
    std::ifstream file, dimensionfile;
    std::vector<int> dimensions;

    dimensionfile.open("instances-dimension");

    if(!dimensionfile.is_open())
    {
        std::cout << "Problem opening dimension file for reading." << std::endl;
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
        std::cout << "Problem opening dual file for reading." << std::endl;
        exit(1);
    }

    std::string line;
    std::getline(file, line, ':');

    int i = 0;
    while(1)
    {
        std::getline(file, line, ':');
        dual.push_back(std::make_pair(std::stod(line), dimensions[i++]));

        if(file.eof())
        {
            break;
        }
    }

    file.close();
}

void getMatrixObjTimeSet(std::string instanceSet, std::vector<std::vector<std::pair<double, int>>> &matrixObjTimeSet)
{
    std::string objvalfilepath = "objval/objval-" + instanceSet;
    std::string timefilepath = "time/time-" + instanceSet;
    std::ifstream objvalfile, timefile, dimensionfile;
    std::vector<int> dimensions;

    dimensionfile.open("instances-dimension");

    if(!dimensionfile.is_open())
    {
        std::cout << "Problem opening dimension file for reading." << std::endl;
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
        std::cout << "Problem opening objval file for reading." << std::endl;
        exit(1);
    }

    std::string line;
    std::getline(objvalfile, line, ':');

    int i = 0;
    while(1)
    {
        std::getline(objvalfile, line, ':');
        matrixObjTimeSet[0].push_back(std::make_pair(std::stod(line), dimensions[i++]));

        if(objvalfile.eof())
        {
            break;
        }
    }

    objvalfile.close();

    timefile.open(timefilepath);

    if(!timefile.is_open())
    {
        std::cout << "Problem opening time file for reading." << std::endl;
        exit(1);
    }

    std::getline(timefile, line, ':');

    i = 0;
    while(1)
    {
        std::getline(timefile, line, ':');
        matrixObjTimeSet[1].push_back(std::make_pair(std::stod(line), dimensions[i++]));

        if(timefile.eof())
        {
            break;
        }
    }

    timefile.close();
}

bool compare(std::pair<double, int> p1, std::pair<double, int> p2) { return (p1.second<p2.second); }

int main()
{
    std::vector<std::string> versions{"balau", "baltu", "bhlau", "bhltu", "btlau", "btltu", "ial", "ihl", "itl", "iau", "itu", "pure"},
        instancesSets = {"A", "B", "C", "D"};
    std::vector<std::vector<std::vector<std::vector<std::pair<double, int>>>>> matrixObjTime(12,
        std::vector<std::vector<std::vector<std::pair<double, int>>>>(4, std::vector<std::vector<std::pair<double, int>>>(2,
        std::vector<std::pair<double, int>>(0, std::make_pair(0, 0)))));
    std::vector<std::vector<std::pair<double, int>>> dual(4, std::vector<std::pair<double, int>>(0, std::make_pair(0, 0))),
        dualPure(4, std::vector<std::pair<double, int>>(0, std::make_pair(0, 0)));
    int noOfInstances, sumSize, median;

    for(int j = 0; j < 4; j++)
    {
        for(int i = 0; i < 12; i++)
        {
            std::cout << versions[i] << instancesSets[j] << std::endl << std::endl;
            getMatrixObjTimeSet(versions[i] + instancesSets[j], matrixObjTime[i][j]);
            
            noOfInstances = matrixObjTime[i][j][0].size();

            for(int k = 0; k < 2; k++)
            {
                std::sort(matrixObjTime[i][j][k].begin(), matrixObjTime[i][j][k].end(), compare);
                
                std::cout << "\t" << k << ":\n\t\t";
                for(int l = 0; l < noOfInstances; l++)
                {
                    std::cout << matrixObjTime[i][j][k][l].first << "  ";
                }
                std::cout << std::endl << std::endl;
            }
        }
    }

    for(int l = 0; l < noOfInstances; l++)
    {
        sumSize += matrixObjTime[0][0][0][l].second;
    }
    median = matrixObjTime[0][0][0][96].second;

    std::vector<std::vector<int>> primal(4, std::vector<int>(noOfInstances, 0));

    std::cout << "dual:" << std::endl << std::endl;
    for(int j = 0; j < 4; j++)
    {
        getDual(instancesSets[j], dualPure[j]);
        std::sort(dualPure[j].begin(), dualPure[j].end(), compare);
        dual[j] = dualPure[j];

        std::cout << "\t" << instancesSets[j] << ":\n\t\t";
        for(int l = 0; l < noOfInstances; l++)
        {
            for(int i = 0; i < 12; i++)
            {
                if(i < 9 || i == 11)
                {
                    if(matrixObjTime[i][j][0][l].first > primal[j][l])
                    {
                        primal[j][l] = matrixObjTime[i][j][0][l].first;
                    }

                    if((matrixObjTime[i][j][0][l].first < dual[j][l].first)&&(matrixObjTime[i][j][1][l].first < 600))
                    {
                        dual[j][l].first = matrixObjTime[i][j][0][l].first;
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
                std::cout << std::endl << std::endl << "ERROR:" << std::endl;
                std::cout << "\tdual: " << dual[j][l].first << std::endl;
                std::cout << "\tprimal: " << primal[j][l] << std::endl;
                dual[j][l].first = primal[j][l];
                std::cout << "\t" << instancesSets[j] << ": " << l + 1 << std::endl;
                exit(1);
            }

            std::cout << dual[j][l].first << "  ";
        }
        std::cout << std::endl << std::endl;
    }

    std::vector<std::vector<int>> solved(12, std::vector<int>(4, 0));
    std::vector<std::vector<int>> optimal(12, std::vector<int>(4, 0));
    std::vector<std::vector<std::vector<double>>> gaps;
    std::vector<std::vector<double>> lowestGap(4, std::vector<double>(193, __DBL_MAX__));
    std::vector<std::vector<double>> gapSum(12, std::vector<double>(4, 0));
    std::vector<std::vector<double>> timeSum(12, std::vector<double>(4, 0));
    std::vector<std::vector<double>> timeOptSum(12, std::vector<double>(4, 0));

    // graphs and setup
    for(int i = 0; i < 12; i++)
    {
        std::ofstream gapfile;

        gapfile.open("graphs/gap/" + versions[i]);

        if(!gapfile.is_open())
        {
            std::cout << "Problem opening gap file for writing." << std::endl;
            exit(1);
        }

        std::ofstream timefile;

        timefile.open("graphs/time/" + versions[i]);

        if(!timefile.is_open())
        {
            std::cout << "Problem opening time file for writing." << std::endl;
            exit(1);
        }

        gapfile << "n\t&\tA\t&\tB\t&\tC\t&\tD" << std::endl;
        timefile << "n\t&\tA\t&\tB\t&\tC\t&\tD" << std::endl;

        std::vector<std::vector<double>> gapsI(4, std::vector<double>(0, 0));
        for(int l = 0; l < noOfInstances; l++)
        {
            if((l+1)%10 == 0)
            {
                gapfile << l+1;
                timefile << l+1;
            }

            for(int j = 0; j < 4; j++)
            {
                double gap;

                if(i < 9)
                {
                    gap = 100*(dual[j][l].first - matrixObjTime[i][j][0][l].first)/matrixObjTime[i][j][0][l].first;

                    solved[i][j]++;
                }
                else if(i < 11)
                {
                    gap = 100*(matrixObjTime[i][j][0][l].first - primal[j][l])/primal[j][l];

                    if(matrixObjTime[i][j][1][l].first < 600)
                    {
                        solved[i][j]++;
                    }
                }
                else
                {
                    gap = 100*(dualPure[j][l].first - primal[j][l])/primal[j][l];

                    if(matrixObjTime[i][j][0][l].first > 0)
                    {
                        solved[i][j]++;
                    }
                }

                gapSum[i][j] += gap;

                if(gap == 0)
                {
                    optimal[i][j]++;
                    timeOptSum[i][j] += matrixObjTime[i][j][1][l].first;
                }

                if(gap < lowestGap[j][l])
                {
                    lowestGap[j][l] = gap;
                }

                gapsI[j].push_back(gap);
                timeSum[i][j] += matrixObjTime[i][j][1][l].first;

                gapfile << "\t&\t" << gap;
                timefile << "\t&\t" << matrixObjTime[i][j][1][l].first;
            }

            gapfile << std::endl;
            timefile << std::endl;
        }

        gaps.push_back(gapsI);
        
        gapfile.close();
        timefile.close();
    }

    std::cout << "mean size: " << sumSize/193.0 << std::endl;
    std::cout << "median: " << median << std::endl;

    // tables
    for(int j = 0; j < 4; j++)
    {
        std::ofstream tablefile;
        tablefile.open("tables/table" + instancesSets[j]);

        if(!tablefile.is_open())
        {
            std::cout << "Problem opening table file for writing." << std::endl;
            exit(1);
        }

        tablefile << "\\hline" << std::endl;

        tablefile << "\t&\t#primal\t&\t#optimal" << "\t&\tlowest gap" <<
        "\t&\toutput\t&\tgap\t&\tT\t&\tT$_\\text{p}$ (s)\\\\" << std::endl;

        tablefile << "\\hline" << std::endl;
        
        tablefile << std::fixed;
        tablefile << std::setprecision(2);

        for(int i = 0; i < 12; i++)
        {
            tablefile << versions[i] << "\t&\t";
            
            // #primal
            tablefile << solved[i][j] << "\t&\t";

            // #optimal
            tablefile << optimal[i][j] << "\t&\t";

            // lowest gap
            int noOfLowestGaps = 0;
            for(int l = 0; l < noOfInstances; l++)
            {
                if(gaps[i][j][l] == lowestGap[j][l])
                {
                    noOfLowestGaps++;
                }
            }
            tablefile << noOfLowestGaps << "\t&\t";

            // output
            if(i < 9)
            {
                tablefile << "primal\t&\t";
            }
            else
            {
                tablefile << "dual\t&\t";
            }

            // gap
            tablefile << gapSum[i][j]/noOfInstances << "\t&\t";

            // time (solved)
            tablefile << timeSum[i][j]/noOfInstances << "\t&\t";

            // time (optimal)
            tablefile << timeOptSum[i][j]/optimal[i][j] << "\\\\" << std::endl;
        }

        tablefile << "\\hline" << std::endl;

        tablefile.close();
    }

    return 0;
}