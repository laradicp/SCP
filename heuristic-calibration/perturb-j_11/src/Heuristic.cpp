#include "Heuristic.h"

using namespace std;

bool isInfeasible(vector<int> &s, Data &data)
{
    int sSize = s.size();

    for(int p = 0; p < sSize; p++)
    {
        for(int c = 0; c < data.getCadencesSize(); c++)
        {
            if(data.cadenceType(c) == 1)
            {
                int sum = 0;

                if(data.getCadencesPerFamily(s[p], c))
                {
                    for(int k = 0; k < data.getCadence(c) + 1; k++)
                    {
                        if(p + k < sSize)
                        {
                            if(data.getCadencesPerFamily(s[p + k], c))
                            {
                                sum++;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                
                if(sum > 1)
                {
                    return true;
                }
            }
            else if(data.cadenceType(c) == 2)
            {    
                int sum = 0;

                if(data.getCadencesPerFamily(s[p], c))
                {
                    for(int k = 0; k < data.getCadence(c) + 1; k++)
                    {
                        if(p + k < sSize)
                        {
                            if(data.getCadencesPerFamily(s[p + k], c))
                            {
                                sum++;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }

                if(sum > data.getCadence(c))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

void construction(vector<int> &s, Data &data)
{
    vector<int> feasibleFamilies;

    for(int f = 0; f < data.getFamiliesSize(); f++)
    {
        feasibleFamilies.push_back(f);
    }

    int sSize = 0;

    while(feasibleFamilies.size())
    {
        int alfa = rand()%feasibleFamilies.size();

        s.push_back(feasibleFamilies[alfa]);
        data.setFamilySize(feasibleFamilies[alfa], data.getFamilySize(feasibleFamilies[alfa]) - 1);
        sSize++;

        feasibleFamilies.clear();

        for(int f = 0; f < data.getFamiliesSize(); f++)
        {
            if(data.getFamilySize(f))
            {
                bool feasible = true;

                for(int c = 0; c < data.getCadencesSize(); c++)
                {
                    if(data.getCadencesPerFamily(f, c))
                    {
                        int begin = sSize - data.getCadence(c) > 0 ?
                                    sSize - data.getCadence(c) : 0;    
                        
                        if(data.cadenceType(c) == 1)
                        {
                            for(int k = begin; k < sSize; k++)
                            {
                                if(data.getCadencesPerFamily(s[k], c))
                                {
                                    feasible = false;
                                    break;
                                }
                            }
                        }
                        else
                        {    
                            int sum = 1;

                            for(int k = begin; k < sSize; k++)
                            {
                                if(data.getCadencesPerFamily(s[k], c))
                                {
                                    sum++;
                                }
                            }

                            if(sum > data.getCadence(c))
                            {
                                feasible = false;
                                break;
                            }
                        }
                    }

                    if(!feasible)
                    {
                        break;
                    }
                }

                if(feasible)
                {
                    feasibleFamilies.push_back(f);
                }
            }
        }
    }
}

void insertion(vector<int> &s, Data &data)
{
    int sSize = s.size();
    int begin = rand()%sSize;

    for(int p = begin; p < sSize; p++)
    {
        for(int f = 0; f < data.getFamiliesSize(); f++)
        {	
            if(data.getFamilySize(f))
            {
                bool feasibleVersion = true;

                for(int c = 0; c < data.getCadencesSize(); c++)
                {
                    if(data.getCadencesPerFamily(f, c))
                    {
                        int begin = p - data.getCadence(c) > 0 ? p - data.getCadence(c) : 0;
                        int end = p + data.getCadence(c) < sSize ? p + data.getCadence(c) : sSize;
                        
                        if(data.cadenceType(c) == 1)
                        {
                            for(int k = begin; k < end; k++)
                            {
                                if(data.getCadencesPerFamily(s[k], c))
                                {
                                    feasibleVersion = false;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            int sum = 0;

                            for(int k = begin; k < p; k++)
                            {
                                if(data.getCadencesPerFamily(s[k], c))
                                {
                                    sum++;
                                }
                                else
                                {
                                    sum = 0; 
                                }
                            }

                            sum++;
                            
                            if(sum > data.getCadence(c))
                            {
                                feasibleVersion = false;
                                break;
                            }

                            for(int k = p; k < end; k++)
                            {
                                if(data.getCadencesPerFamily(s[k], c))
                                {
                                    sum++;

                                    if(sum > data.getCadence(c))
                                    {
                                        feasibleVersion = false;
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

                if(feasibleVersion)
                {
                    s.insert(s.begin() + p, f);
                    sSize++;

                    data.setFamilySize(f, data.getFamilySize(f) - 1);
                }
            }
        }
    }

    for(int p = 0; p < begin; p++)
    {
        for(int f = 0; f < data.getFamiliesSize(); f++)
        {	
            if(data.getFamilySize(f))
            {
                bool feasibleVersion = true;

                for(int c = 0; c < data.getCadencesSize(); c++)
                {
                    if(data.getCadencesPerFamily(f, c))
                    {
                        int begin = p - data.getCadence(c) > 0 ? p - data.getCadence(c) : 0;
                        int end = p + data.getCadence(c) < sSize ? p + data.getCadence(c) : sSize;
                        
                        if(data.cadenceType(c) == 1)
                        {
                            for(int k = begin; k < end; k++)
                            {
                                if(data.getCadencesPerFamily(s[k], c))
                                {
                                    feasibleVersion = false;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            int sum = 0;

                            for(int k = begin; k < p; k++)
                            {
                                if(data.getCadencesPerFamily(s[k], c))
                                {
                                    sum++;
                                }
                                else
                                {
                                    sum = 0; 
                                }
                            }

                            sum++;
                            
                            if(sum > data.getCadence(c))
                            {
                                feasibleVersion = false;
                                break;
                            }

                            for(int k = p; k < end; k++)
                            {
                                if(data.getCadencesPerFamily(s[k], c))
                                {
                                    sum++;

                                    if(sum > data.getCadence(c))
                                    {
                                        feasibleVersion = false;
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

                if(feasibleVersion)
                {
                    s.insert(s.begin() + p, f);
                    sSize++;

                    data.setFamilySize(f, data.getFamilySize(f) - 1);
                }
            }
        }
    }
}

void perturbation(vector<int> &s, Data &data, vector<int> &perturbationType)
{
    int i = rand()%perturbationType.size(), sSize = s.size();
    
    if(perturbationType[i] == 1)
    {
        int t = sSize/2 < data.getDimension()/11 ? sSize/2 : data.getDimension()/11;
        for(int j = 0; j < t; j++)
        {
            vector<int> feasiblePositions;
            
            feasiblePositions.push_back(0);

            for(int p = 1; p < sSize - 1; p++)
            {
                bool feasiblePosition = true;

                for(int c = 0; c < data.getCadencesSize(); c++)
                {
                    int begin = p - data.getCadence(c) > 0 ? p - data.getCadence(c) : 0; 
                    int end = p + data.getCadence(c) + 1 < sSize ? p + data.getCadence(c) + 1 : sSize;

                    if(data.cadenceType(c) == 1)
                    {
                        int lastTrueK = -data.getDimension();

                        for(int k = begin; k < p; k++)
                        {
                            if(data.getCadencesPerFamily(s[k], c))
                            {
                                if(k - lastTrueK < data.getCadence(c) + 1)
                                {
                                    feasiblePosition = false;
                                    break;
                                }
                                else
                                {
                                    lastTrueK = k;
                                }
                            }
                        }

                        lastTrueK++; //because of the position p in the middle

                        for(int k = p + 1; k < end; k++)
                        {
                            if(data.getCadencesPerFamily(s[k], c))
                            {
                                if(k - lastTrueK < data.getCadence(c) + 1)
                                {
                                    feasiblePosition = false;
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {    
                        int sum = 0;

                        for(int k = begin; k < p; k++)
                        {
                            if(data.getCadencesPerFamily(s[k], c))
                            {
                                sum++;
                            }
                            else
                            {
                                sum = 0;
                            }
                        }

                        for(int k = p + 1; k < end; k++)
                        {
                            if(data.getCadencesPerFamily(s[k], c))
                            {
                                sum++;

                                if(sum > data.getCadence(c))
                                {
                                    feasiblePosition = false;
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

                if(feasiblePosition)
                {
                    feasiblePositions.push_back(p);
                }
            }
            
            feasiblePositions.push_back(sSize - 1);

            int feasiblePositionsSize = feasiblePositions.size();

            if(feasiblePositionsSize)
            {
                int p = rand()%feasiblePositionsSize;
                data.setFamilySize(s[feasiblePositions[p]], data.getFamilySize(s[feasiblePositions[p]]) + 1);
                s.erase(s.begin() + feasiblePositions[p]);
                sSize--;
            }
        }
    }
    else
    {
        vector<pair<int, int>> feasiblePairs;

        for(int p1 = 0; p1 < sSize - 1; p1++)
        {
            for(int p2 = p1 + 1; p2 < sSize; p2++)
            {
                bool feasiblePair = true;

                for(int c = 0; c < data.getCadencesSize(); c++)
                {
                    if(data.getCadencesPerFamily(s[p1], c))
                    {
                        int begin = p2 - data.getCadence(c) > 0 ? p2 - data.getCadence(c) : 0;
                        int end = p2 + data.getCadence(c) + 1 < sSize ? p2 + data.getCadence(c) + 1 : sSize;

                        if(data.cadenceType(c) == 1)
                        {
                            for(int k = begin; k < p2; k++)
                            {
                                if(data.getCadencesPerFamily(s[k], c))
                                {
                                    feasiblePair = false;
                                    break;
                                }
                            }

                            for(int k = p2 + 1; k < end; k++)
                            {
                                if(data.getCadencesPerFamily(s[k], c))
                                {
                                    feasiblePair = false;
                                    break;
                                }
                            }
                        }
                        else
                        {    
                            int sum = 0;

                            for(int k = begin; k < p2; k++)
                            {
                                if(data.getCadencesPerFamily(s[k], c))
                                {
                                    sum++;
                                }
                                else
                                {
                                    sum = 0; 
                                }
                            }

                            sum++;
                            
                            if(sum > data.getCadence(c))
                            {
                                feasiblePair = false;
                                break;
                            }

                            for(int k = p2 + 1; k < end; k++)
                            {
                                if(data.getCadencesPerFamily(s[k], c))
                                {
                                    sum++;

                                    if(sum > data.getCadence(c))
                                    {
                                        feasiblePair = false;
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

                    if(data.getCadencesPerFamily(s[p2], c))
                    {
                        int begin = p1 - data.getCadence(c) > 0 ? p1 - data.getCadence(c) : 0;
                        int end = p1 + data.getCadence(c) + 1 < sSize ? p1 + data.getCadence(c) + 1 : sSize;

                        if(data.cadenceType(c) == 1)
                        {
                            for(int k = begin; k < p1; k++)
                            {
                                if(data.getCadencesPerFamily(s[k], c))
                                {
                                    feasiblePair = false;
                                    break;
                                }
                            }

                            for(int k = p1 + 1; k < end; k++)
                            {
                                if(data.getCadencesPerFamily(s[k], c))
                                {
                                    feasiblePair = false;
                                    break;
                                }
                            }
                        }
                        else
                        {    
                            int sum = 0;

                            for(int k = begin; k < p1; k++)
                            {
                                if(data.getCadencesPerFamily(s[k], c))
                                {
                                    sum++;
                                }
                                else
                                {
                                    sum = 0; 
                                }
                            }

                            sum++;
                            
                            if(sum > data.getCadence(c))
                            {
                                feasiblePair = false;
                                break;
                            }

                            for(int k = p1 + 1; k < end; k++)
                            {
                                if(data.getCadencesPerFamily(s[k], c))
                                {
                                    sum++;

                                    if(sum > data.getCadence(c))
                                    {
                                        feasiblePair = false;
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

                if(feasiblePair)
                {
                    pair<int, int> p = {p1, p2};
                    feasiblePairs.push_back(p);
                }
            }
        }

        int feasiblePairsSize = feasiblePairs.size();
        if(feasiblePairsSize)
        {
            int p = rand()%feasiblePairsSize;
            s.insert(s.begin() + feasiblePairs[p].second, s[feasiblePairs[p].first]);
            s.insert(s.begin() + feasiblePairs[p].first, s[feasiblePairs[p].second + 1]);

            s.erase(s.begin() + feasiblePairs[p].first + 1);
            s.erase(s.begin() + feasiblePairs[p].second + 1);

            feasiblePairs.erase(feasiblePairs.begin() + p);
            feasiblePairsSize--;

            int t = sSize/2 < data.getDimension()/11 ? sSize/2 - 1 : data.getDimension()/11 - 1;

            if(feasiblePairsSize < t)
            {
                t = feasiblePairsSize;
            }

            int k = 0;
            for(int j = 0; j < t; )
            {
                vector<int> newS = s;

                p = rand()%feasiblePairsSize;

                newS.insert(newS.begin() + feasiblePairs[p].second, newS[feasiblePairs[p].first]);
                newS.insert(newS.begin() + feasiblePairs[p].first, newS[feasiblePairs[p].second + 1]);

                newS.erase(newS.begin() + feasiblePairs[p].first + 1);
                newS.erase(newS.begin() + feasiblePairs[p].second + 1);

                if(isInfeasible(newS, data))
                {
                    k++;

                    if(k > 4)
                    {
                        k = 0;
                        j++;
                    }
                }
                else
                {
                    s = newS;

                    feasiblePairs.erase(feasiblePairs.begin() + p);
                    feasiblePairsSize--;

                    if(!feasiblePairsSize)
                    {
                        break;
                    }

                    k = 0;
                    j++;
                }
            }
        }
    }

    perturbationType.erase(perturbationType.begin() + i);

    if(perturbationType.empty())
    {
        perturbationType.push_back(1);
        perturbationType.push_back(1);
        perturbationType.push_back(2);
        perturbationType.push_back(2);
    }
}

int heuristic(Data data, vector<int> &bestS)
{
    auto beginTime = chrono::system_clock::now();

    int seed = time(NULL);
    srand(seed);

    Data fullData = data;
    int bestSSize = 0;

    vector<int> currentBestS;
    data = fullData;

    construction(currentBestS, data);
    int currentBestSSize = currentBestS.size();
    Data currentBestData = data;

    vector<int> perturbationType{1, 1, 2, 2};
    
    for(int j = 0; j < data.getDimension()/2; j++)
    {
        vector<int> s = currentBestS;
        data = currentBestData;

        if(s.size() == data.getDimension())
        {
            break;
        }
        
        perturbation(s, data, perturbationType);
        
        if(s.size() == data.getDimension())
        {
            break;
        }

        insertion(s, data);
        
        if(s.size() > currentBestSSize)
        {
            currentBestS = s;
            currentBestSSize = s.size();
            currentBestData = data;
        
            j = 0;
        }
        else
        {
            j++;
        }
    }

    if(currentBestSSize > bestSSize)
    {
        bestS = currentBestS;
        bestSSize = currentBestSSize;
    }

    chrono::duration<double> currentTime = chrono::system_clock::now() - beginTime;

    return bestSSize;
}