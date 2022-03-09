#include "Data.h"

Data::Data(std::string filePath)
{
    familiesSize = 0;

    std::ifstream in;
    in.open(filePath);

    if(!in.is_open())
    {
        std::cout << "Problem opening file for reading." << std::endl;
        exit(1);
    }

    in >> cadencesSize;

    std::pair<int, int> cadence;
    for(int i = 0; i < cadencesSize; i++)
    {
        in >> cadence.first;
        in >> cadence.second;

        cadences.push_back(cadence);
    }

    int i = 0;
    for( ; ; i++)
    {
        int version;
        in >> version;

        if(in.eof())
        {
            break;
        }

        std::vector<bool> cadencesOfJob;
        bool cadence;

        for(int c = 0; c < cadencesSize; c++)
        {
            in >> cadence;
            cadencesOfJob.push_back(cadence);
        }

        cadencesPerJob.push_back(cadencesOfJob);

        bool none = true;
        for(int f = 0; f < familiesSize; f++)
        {
            bool equal = true;
            
            for(int c = 0; c < cadencesSize; c++)
            {
                if(cadencesOfJob[c] != getCadencesPerFamily(f, c))
                {
                    equal = false;

                    break;
                }
            }

            if(equal)
            {
                families[f].second.push_back(version);
                familySize[f]++;

                none = false;

                break;
            }
        }

        if(none)
        {
            std::pair<std::vector<bool>, std::vector<int>> newFamily;
            newFamily.first = cadencesOfJob;
            newFamily.second.push_back(version);

            familiesSize++;
            familySize.push_back(1);

            families.push_back(newFamily);
        }

        if(in.eof())
        {
            break;
        }
    }

    dimension = i;

    in.close();
}

int Data::getDimension()
{
    return dimension;
}

int Data::cadenceType(int c)
{
    if(cadences[c].first > 1)
    {
        return 2;
    }
    
    return 1;
}

int Data::getCadence(int c)
{
    if(cadenceType(c) == 1)
    {
        return cadences[c].second;
    }

    return cadences[c].first;
}

int Data::getFamilyMember(int f, int i)
{
    if(getFamilySize(f) > i)
    {
        return families[f].second[i];
    }

    return -1;
}

int Data::getCadencesSize()
{
    return cadencesSize;
}

int Data::getFamiliesSize()
{
    return familiesSize;
}

int Data::getFamilySize(int f)
{
    return familySize[f];
}

bool Data::getCadencesPerFamily(int f, int c)
{
    return families[f].first[c];
}
bool Data::getCadencesPerJob(int i, int c)
{
    return cadencesPerJob[i][c];
}

int Data::getUpperBound()
{
    // setup
    std::vector<int> jobsPerCadence(getCadencesSize(), 0), surplus(getCadencesSize(), 0);
    std::vector<std::vector<int>> intersect(getCadencesSize(), std::vector<int>(getCadencesSize(), 0));
    for(int c1 = 0; c1 < getCadencesSize(); c1++)
    {
        for(int f = 0; f < getFamiliesSize(); f++)
        {
            if(getCadencesPerFamily(f, c1))
            {
                jobsPerCadence[c1] += getFamilySize(f);
                for(int c2 = 0; c2 < getCadencesSize(); c2++)
                {
                    if(getCadencesPerFamily(f, c2))
                    {
                        intersect[c1][c2] += getFamilySize(f);
                    }
                }
            }
        }

        surplus[c1] = jobsPerCadence[c1] - std::ceil(cadences[c1].first*getDimension()/
                      (float)(cadences[c1].first + cadences[c1].second));
        surplus[c1] = surplus[c1] > 0 ? surplus[c1] : 0;

        for(int c2 = 0; c2 < c1; c2++)
        {
            int min = surplus[c2] < intersect[c1][c2] ? surplus[c2] : intersect[c1][c2];
            surplus[c1] -= min;
            surplus[c1] = surplus[c1] > 0 ? surplus[c1] : 0;
        }
    }

    int ub = getDimension();
    for(int c = 0; c < getCadencesSize(); c++)
    {
        ub -= surplus[c];
    }

    return ub;
}