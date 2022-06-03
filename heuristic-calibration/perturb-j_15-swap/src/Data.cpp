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

        std::vector<std::pair<std::pair<int, int>, char>> cadencesPerJob;
        // cadencesPerJob[c].first = original cadence
        // cadencesPerJob[c].second = job has the cadence ('1') or not ('0')
        bool cadencePerJob;

        for(int c = 0; c < cadencesSize; c++)
        {
            in >> cadencePerJob;

            if(cadencePerJob)
            {
                cadencesPerJob.push_back(std::make_pair(cadences[c], '1'));
            }
            else
            {
                cadencesPerJob.push_back(std::make_pair(cadences[c], '0'));
            }
        }

        std::sort(cadencesPerJob.begin(), cadencesPerJob.end(), compareCadencePairs);

        bool none = true;
        for(int f = 0; f < familiesSize; f++)
        {
            bool equal = true;
            
            for(int c = 0; c < cadencesSize; c++)
            {
                if(cadencesPerJob[c].second != getCadencesPerFamily(f, c) + '0')
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
            std::pair<std::vector<char>, std::vector<int>> newFamily;
            
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

bool Data::compareCadencePairs(std::pair<std::pair<int, int>, char> p1, std::pair<std::pair<int, int>, char> p2)
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
    return families[f].first[c] == '1';
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
                      (double)(cadences[c1].first + cadences[c1].second));
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

int Data::used(int j, int i, std::vector<int> &s, std::vector<double> &score, std::vector<int> &jobsPerScore,
    std::vector<std::vector<int>> &familiesPerScore, std::vector<std::vector<int>> &intersection)
{
    calculateLB(i - 1, s, score, jobsPerScore, familiesPerScore, intersection);

    int fittingJobs = ceil(lb[i - 1]/score[i]);
    if(j == i)
    {
        return fittingJobs < jobsPerScore[i] ? fittingJobs : jobsPerScore[i] ;
    }

    int min = fittingJobs < intersection[i][j] ? fittingJobs : intersection[i][j];
    return used(j, i - 1, s, score, jobsPerScore, familiesPerScore, intersection) + min;
}

void Data::calculateLB(int i, std::vector<int> &s, std::vector<double> &score, std::vector<int> &jobsPerScore,
    std::vector<std::vector<int>> &familiesPerScore, std::vector<std::vector<int>> &intersection)
{
    if(lb[i] != -1)
    {
        return;
    }

    if(i == 0)
    {
        // update primal solution
        int familiesPerScoreSize = familiesPerScore[i].size();
        int job = 0;
        for(int fIdx = 0; fIdx < familiesPerScoreSize; fIdx++)
        {
            for(; job < jobsPerScore[i]; job++)
            {
                if(familySize[familiesPerScore[i][fIdx]] == 0)
                {
                    break;
                }

                s.push_back(familiesPerScore[i][fIdx]);
                familySize[familiesPerScore[i][fIdx]]--;
            }

            if(job == jobsPerScore[i])
            {
                break;
            }
        }

        lb[i] = jobsPerScore[i];

        return;
    }
    
    calculateLB(i - 1, s, score, jobsPerScore, familiesPerScore, intersection);

    int min = ceil(lb[i - 1]/score[i]) < jobsPerScore[i] ? ceil(lb[i - 1]/score[i]) : jobsPerScore[i],
        violations = 0;
    int usedPos = 0;
    if(i == 1)
    {
        usedPos = used(i, i, s, score, jobsPerScore, familiesPerScore, intersection);
    }
    
    // j = 0 does not correspond to any cadence, therefore it is not used for violations
    for(int j = 1; j < i; j++)
    {
        usedPos = used(j, i, s, score, jobsPerScore, familiesPerScore, intersection);
        violations += intersection[i][j] < usedPos ? intersection[i][j] : usedPos;
    }

    int max = min - violations > 0 ? min - violations : 0; // number of jobs added to solution
    
    // update primal solution
    int sSize = s.size(), familiesPerScoreSize = familiesPerScore[i].size();
    int insertedJobs = 0;
    for(int p = 0; p < sSize; p++)
    {
        if(insertedJobs == max)
        {
            break;
        }

        for(int fIdx = 0; fIdx < familiesPerScoreSize; fIdx++)
        {	
            if(getFamilySize(familiesPerScore[i][fIdx]))
            {
                bool feasibleFamily = true;

                for(int c = 0; c < i; c++) // M can have jobs with cadence up to c = i - 1
                {
                    if(getCadencesPerFamily(familiesPerScore[i][fIdx], c))
                    {
                        int begin = p - getCadence(c) > 0 ? p - getCadence(c) : 0;
                        int end = p + getCadence(c) < sSize ? p + getCadence(c) : sSize;
                        
                        if(cadenceType(c) == 1)
                        {
                            for(int k = begin; k < end; k++)
                            {
                                if(getCadencesPerFamily(s[k], c))
                                {
                                    feasibleFamily = false;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            int sum = 0;

                            for(int k = begin; k < p; k++)
                            {
                                if(getCadencesPerFamily(s[k], c))
                                {
                                    sum++;
                                }
                                else
                                {
                                    sum = 0; 
                                }
                            }

                            sum++;
                            
                            if(sum > getCadence(c))
                            {
                                feasibleFamily = false;
                                break;
                            }

                            for(int k = p; k < end; k++)
                            {
                                if(getCadencesPerFamily(s[k], c))
                                {
                                    sum++;

                                    if(sum > getCadence(c))
                                    {
                                        feasibleFamily = false;
                                        break;
                                    }
                                }
                                else
                                {
                                    sum = 0; 
                                }
                            }
                        }
                    }
                }

                if(feasibleFamily)
                {
                    s.insert(s.begin() + p, familiesPerScore[i][fIdx]);
                    sSize++;

                    setFamilySize(familiesPerScore[i][fIdx], getFamilySize(familiesPerScore[i][fIdx]) - 1);

                    if(++insertedJobs == max)
                    {
                        break;
                    }
                }
            }
        }
    }

    lb[i] = lb[i - 1] + max;

    return;
}

int Data::getLowerBound()
{
    if(lb.size() > getCadencesSize())
    {
        return lb[getCadencesSize()];
    }

    return 1;
}

std::vector<int> Data::getLowerBoundSol()
{
    lb.resize(getCadencesSize() + 1, -1);
    std::vector<int> s;
    std::vector<double> score(getCadencesSize() + 1, 0);
    std::vector<int> jobsPerScore(getCadencesSize() + 1, 0);
    std::vector<std::vector<int>> familiesPerScore(getCadencesSize() + 1, std::vector<int>(0, 0));
    std::vector<std::vector<int>> intersection(getCadencesSize() + 1, std::vector<int>(getCadencesSize() + 1, 0));

    // Set scores
    for(int i = 1; i < getCadencesSize() + 1; i++)
    {
        if(cadenceType(i - 1) == 1)
        {
            score[i] = getCadence(i - 1);
        }
        else
        {
            score[i] = 1.0/getCadence(i - 1);
        }
    }

    // Set |M| and intersections
    for(int f = 0; f < getFamiliesSize(); f++)
    {
        bool noCadence = true;
        for(int i = 1; i < getCadencesSize() + 1; i++)
        {
            if(getCadencesPerFamily(f, i - 1))
            {
                noCadence = false;
                bool mostRestrictive = true;
                for(int j = i + 1; j < getCadencesSize() + 1; j++)
                {
                    if(getCadencesPerFamily(f, j - 1))
                    {
                        mostRestrictive = false;
                        break;
                    }
                }
                if(mostRestrictive)
                {
                    // Update |M|
                    jobsPerScore[i] += getFamilySize(f);
                    familiesPerScore[i].push_back(f);
                    // Update intersections
                    for(int j = 1; j < i; j++)
                    {
                        if(getCadencesPerFamily(f, j - 1))
                        {
                            intersection[i][j] += getFamilySize(f);
                        }
                    }
                }
            }
        }

        if(noCadence)
        {
            // Update |M|
            jobsPerScore[0] += getFamilySize(f);
            familiesPerScore[0].push_back(f);
        }
    }
    
    std::vector<int> familySizeReset = familySize;
    calculateLB(getCadencesSize(), s, score, jobsPerScore, familiesPerScore, intersection);
    familySize = familySizeReset;

    return s;
}