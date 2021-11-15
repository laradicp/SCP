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

        std::vector<bool> cadencesPerJob;
        bool cadencePerJob;

        for(int c = 0; c < cadencesSize; c++)
        {
            in >> cadencePerJob;
            cadencesPerJob.push_back(cadencePerJob);
        }

        bool none = true;
        for(int f = 0; f < familiesSize; f++)
        {
            bool equal = true;
            
            for(int c = 0; c < cadencesSize; c++)
            {
                if(cadencesPerJob[c] != getCadencesPerFamily(f, c))
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
            newFamily.first = cadencesPerJob;
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

void Data::setFamilySize(int f, int size)
{
    familySize[f] = size;
}
int Data::getFamilySize(int f)
{
    return familySize[f];
}

bool Data::getCadencesPerFamily(int f, int c)
{
    return families[f].first[c];
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

int Data::unused(int j, int i, std::vector<double> &score, std::vector<int> &jobsPerScore, std::vector<std::vector<int>> &intersection)
{
    if(j == 0)
    {
        return 0;
    }
    else if(j == i)
    {
        int previousLB = calculateLB(j - 1, score, jobsPerScore, intersection);
        return std::ceil(1/score[j]*previousLB) - jobsPerScore[j] > 0 ? std::ceil(1/score[j]*previousLB) - jobsPerScore[j] : 0;
    }
    else
    {
        int previousUnusedJ = unused(j, i - 1, score, jobsPerScore, intersection);
        int usedInI = previousUnusedJ < intersection[i][j] ? previousUnusedJ : intersection[i][j];
        return previousUnusedJ - usedInI;
    }
}

int Data::calculateLB(int i, std::vector<double> &score, std::vector<int> &jobsPerScore, std::vector<std::vector<int>> &intersection)
{
    if(i == 0)
    {
        return jobsPerScore[i];
    }
    else
    {
        int previousLB = calculateLB(i - 1, score, jobsPerScore, intersection), violations = 0;
        int maxFit = std::ceil(1/score[i]*previousLB) < jobsPerScore[i] ? std::ceil(1/score[i]*previousLB) : jobsPerScore[i];
        for(int j = 0; j < i; j++)
        {
            int intersectionMinusUnused = intersection[i][j] - unused(j, i - 1, score, jobsPerScore, intersection);
            violations += intersectionMinusUnused > 0 ? intersectionMinusUnused : 0;
        }
        int fit = maxFit - violations > 0 ? maxFit - violations : 0;
        return previousLB + fit;        
    }
}

int Data::getLowerBound()
{
    std::vector<double> score(getCadencesSize() + 1, 0);
    std::vector<int> jobsPerScore(getCadencesSize() + 1, 0);
    std::vector<std::vector<int>> intersection(getCadencesSize() + 1, std::vector<int>(getCadencesSize() + 1, 0));

    // Set scores
    for(int c = 1; c < getCadencesSize() + 1; c++)
    {
        if(cadenceType(c - 1) == 1)
        {
            score[c] = getCadence(c - 1);
        }
        else
        {
            score[c] = 1.0/getCadence(c - 1);
        }
    }

    // Set |M| and intersections
    for(int f = 0; f < getFamiliesSize(); f++)
    {
        bool noCadence = true;
        for(int c1 = 1; c1 < getCadencesSize() + 1; c1++)
        {
            if(getCadencesPerFamily(f, c1 - 1))
            {
                noCadence = false;
                bool mostRestrictive = true;
                for(int c2 = c1 + 1; c2 < getCadencesSize() + 1; c2++)
                {
                    if(getCadencesPerFamily(f, c2 - 1))
                    {
                        mostRestrictive = false;
                        break;
                    }
                }
                if(mostRestrictive)
                {
                    // Update |M|
                    jobsPerScore[c1] += getFamilySize(f);
                    // Update intersections
                    for(int c2 = 1; c2 < c1; c2++)
                    {
                        if(getCadencesPerFamily(f, c2 - 1))
                        {
                            intersection[c1][c2] += getFamilySize(f);
                        }
                    }
                }
            }
        }
        if(noCadence)
        {
            // Update |M|
            jobsPerScore[0] += getFamilySize(f);
        }
    }
    
    return calculateLB(getCadencesSize(), score, jobsPerScore, intersection);
}