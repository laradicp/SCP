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

        std::vector<std::pair<std::pair<int, int>, bool>> cadencesPerJob;
        // cadencesPerJob[c].first = original cadence
        // cadencesPerJob[c].second = job has the cadence (1) or not (0)
        bool cadencePerJob;

        for(int c = 0; c < cadencesSize; c++)
        {
            in >> cadencePerJob;
            cadencesPerJob.push_back(std::make_pair(cadences[c], cadencePerJob));
        }

        std::sort(cadencesPerJob.begin(), cadencesPerJob.end(), compareCadencePairs);

        bool none = true;
        for(int f = 0; f < familiesSize; f++)
        {
            bool equal = true;
            
            for(int c = 0; c < cadencesSize; c++)
            {
                if(cadencesPerJob[c].second != getCadencesPerFamily(f, c))
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
            
            for(int c = 0; c < cadencesSize; c++)
            {
                newFamily.first.push_back(cadencesPerJob[c].second);
            }
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

    std::sort(cadences.begin(), cadences.end(), compareCadences);
    dimension = i;

    in.close();

    maxCadence = 0;
    for(int c = 0; c < cadencesSize; c++)
    {
        if(getCadence(c) > maxCadence)
        {
            maxCadence = getCadence(c);
        }
    }
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

int Data::getMaxCadence()
{
    return maxCadence;
}

bool Data::compareCadencePairs(std::pair<std::pair<int, int>, bool> p1, std::pair<std::pair<int, int>, bool> p2)
{
    return compareCadences(p1.first, p2.first);
}

bool Data::compareCadences(std::pair<int, int> c1, std::pair<int, int> c2)
{
    if(c1.first > 1)
    {
        if(c2.first > 1)
        {
            return c1.first > c2.first;
        }

        return true;
    }

    if(c2.first > 1)
    {
        return false;
    }

    return c1.second < c2.second;
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

int Data::used(int j, int i, std::vector<double> &score, std::vector<int> &jobsPerScore, std::vector<std::vector<int>> &intersection)
{
    int fittingJobs = ceil(calculateLB(i - 1, score, jobsPerScore, intersection)/score[i]);
    if(j == i)
    {
        return fittingJobs < jobsPerScore[i] ? fittingJobs : jobsPerScore[i] ;
    }
    else
    {
        int min = fittingJobs < intersection[i][j] ? fittingJobs : intersection[i][j];
        return used(j, i - 1, score, jobsPerScore, intersection) + min;
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
        int previousLB = calculateLB(i - 1, score, jobsPerScore, intersection), 
            min = ceil(previousLB/score[i]) < jobsPerScore[i] ? ceil(previousLB/score[i]) : jobsPerScore[i],
            violations = 0;
        for(int j = 1; j < i; j++)
        {
            int usedPos = used(j, i, score, jobsPerScore, intersection);
            violations += intersection[i][j] < usedPos ? intersection[i][j] : usedPos;
        }
        int max = min - violations > 0 ? min - violations : 0;
        return previousLB + max;        
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