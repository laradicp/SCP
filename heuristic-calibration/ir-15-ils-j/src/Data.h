#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cmath>

class Data
{
    private:
        int dimension;
        std::vector<std::pair<int, int>> cadences;
        std::vector<std::pair<std::vector<bool>, std::vector<int>>> families;
        int cadencesSize;
        int familiesSize;
        std::vector<int> familySize;
        std::vector<std::vector<bool>> cadencesPerFamily;

        int unused(int j, int i, std::vector<double> &score, std::vector<int> &jobsPerScore, std::vector<std::vector<int>> &intersection);
        int calculateLB(int i, std::vector<double> &score, std::vector<int> &jobsPerScore, std::vector<std::vector<int>> &intersection);

    public:
        Data(std::string filePath);
        
        int getDimension();

        int cadenceType(int c);

        int getCadence(int c);

        int getFamilyMember(int f, int i);

        int getCadencesSize();

        int getFamiliesSize();
        
        void setFamilySize(int f, int size);
        int getFamilySize(int f);

        bool getCadencesPerFamily(int f, int c);

        int getUpperBound();

        int getLowerBound();
};

#endif
