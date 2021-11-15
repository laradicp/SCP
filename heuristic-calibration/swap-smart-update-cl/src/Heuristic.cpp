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

bool checkSwapFeasibility(vector<int> &s, Data &data, int p1, int p2)
{
    bool feasiblePair = true;
    int sSize = s.size();

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
        return true;
    }

    return false;
}

vector<pair<int, int>> updateSwapCandidateList(vector<int> &s, Data &data, vector<pair<int, int>> &candidateList, int i)
{
    int highestCadence = 0;
    for(int c = 0; c < data.getCadencesSize(); c++)
    {
        if(data.getCadence(c) > highestCadence)
        {
            highestCadence = data.getCadence(c);
        }
    }

    vector<pair<int,int>> updatedCandidateList;
    int sSize = s.size();
    int beginFirst = candidateList[i].first - highestCadence > 0 ? candidateList[i].first - highestCadence : 0;
    int endFirst = candidateList[i].first + highestCadence + 1 < sSize ? candidateList[i].first + highestCadence + 1 : sSize;
    int beginSecond = candidateList[i].second - highestCadence > 0 ? candidateList[i].second - highestCadence : 0;
    int endSecond = candidateList[i].second + highestCadence + 1 < sSize ? candidateList[i].second + highestCadence + 1 : sSize;

    // update movements with swaped jobs
    for(int p = 0; p < beginFirst; p++)
    {
        if(checkSwapFeasibility(s, data, p, candidateList[i].first))
        {
            updatedCandidateList.push_back(make_pair(p, candidateList[i].first));
        }

        if(checkSwapFeasibility(s, data, p, candidateList[i].second))
        {
            updatedCandidateList.push_back(make_pair(p, candidateList[i].second));
        }
    }

    for(int p = endFirst; p < beginSecond; p++)
    {
        if(checkSwapFeasibility(s, data, candidateList[i].first, p))
        {
            updatedCandidateList.push_back(make_pair(candidateList[i].first, p));
        }

        if(checkSwapFeasibility(s, data, p, candidateList[i].second))
        {
            updatedCandidateList.push_back(make_pair(p, candidateList[i].second));
        }
    }

    for(int p = endSecond; p < sSize; p++)
    {
        if(checkSwapFeasibility(s, data, candidateList[i].first, p))
        {
            updatedCandidateList.push_back(make_pair(candidateList[i].first, p));
        }

        if(checkSwapFeasibility(s, data, candidateList[i].second, p))
        {
            updatedCandidateList.push_back(make_pair(candidateList[i].second, p));
        }
    }

    // update movements inside range of swaped jobs
    if(endFirst < beginSecond)
    {
        for(int p1 = beginFirst; p1 < endFirst - 1; p1++)
        {
            for(int p2 = p1 + 1; p2 < endFirst; p2++)
            {
                if(checkSwapFeasibility(s, data, p1, p2))
                {
                    updatedCandidateList.push_back(make_pair(p1, p2));
                }
            }
        }

        for(int p1 = beginSecond; p1 < endSecond - 1; p1++)
        {
            for(int p2 = p1 + 1; p2 < endSecond; p2++)
            {
                if(checkSwapFeasibility(s, data, p1, p2))
                {
                    updatedCandidateList.push_back(make_pair(p1, p2));
                }
            }
        }   
    }
    else
    {
        for(int p1 = beginFirst; p1 < endSecond - 1; p1++)
        {
            for(int p2 = p1 + 1; p2 < endSecond; p2++)
            {
                if(checkSwapFeasibility(s, data, p1, p2))
                {
                    updatedCandidateList.push_back(make_pair(p1, p2));
                }
            }
        }
    }

    int candidateListSize = candidateList.size();
    for(int j = 0; j < i; j++)
    {
        if(candidateList[j].second < candidateList[i].first - highestCadence || candidateList[j].first > candidateList[i].second + highestCadence ||
           (candidateList[j].first > candidateList[i].first + highestCadence)&&(candidateList[j].second < candidateList[i].second - highestCadence))
        {
            updatedCandidateList.push_back(candidateList[j]);
        }
    }

    for(int j = i + 1; j < candidateListSize; j++)
    {
        if(candidateList[j].second < candidateList[i].first - highestCadence || candidateList[j].first > candidateList[i].second + highestCadence ||
           (candidateList[j].first > candidateList[i].first + highestCadence)&&(candidateList[j].second < candidateList[i].second - highestCadence))
        {
            updatedCandidateList.push_back(candidateList[j]);
        }
    }

    return updatedCandidateList;
}

void perturbation(vector<int> &s, Data &data, vector<int> &perturbationType)
{
    int i = rand()%perturbationType.size(), sSize = s.size();
    
    if(perturbationType[i] == 1)
    {
        int t = sSize/2 < data.getDimension()/20 ? sSize/2 : data.getDimension()/20;
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

            int p = rand()%feasiblePositions.size();
            data.setFamilySize(s[feasiblePositions[p]], data.getFamilySize(s[feasiblePositions[p]]) + 1);
            s.erase(s.begin() + feasiblePositions[p]);
            sSize--;
        }
    }
    else
    {
        int t = sSize/2 < data.getDimension()/20 ? sSize/2 : data.getDimension()/20;
        vector<pair<int, int>> feasiblePairs;

        for(int p1 = 0; p1 < sSize - 1; p1++)
        {
            for(int p2 = p1 + 1; p2 < sSize; p2++)
            {
                if(checkSwapFeasibility(s, data, p1, p2))
                {
                    feasiblePairs.push_back(make_pair(p1, p2));
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

            for(int j = 1; j < t; j++)
            {
                feasiblePairs = updateSwapCandidateList(s, data, feasiblePairs, p);
                feasiblePairsSize = feasiblePairs.size();

                if(feasiblePairsSize)
                {
                    p = rand()%feasiblePairsSize;
                    s.insert(s.begin() + feasiblePairs[p].second, s[feasiblePairs[p].first]);
                    s.insert(s.begin() + feasiblePairs[p].first, s[feasiblePairs[p].second + 1]);

                    s.erase(s.begin() + feasiblePairs[p].first + 1);
                    s.erase(s.begin() + feasiblePairs[p].second + 1);
                }
                else
                {
                    break;
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