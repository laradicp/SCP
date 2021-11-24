#include "Heuristic.h"

using namespace std;

bool isInfeasible(vector<int> &s, Data &data)
{
    int sSize = s.size();

    for(int p = 0; p < sSize; p++)
    {
        for(int c = 0; c < data.getCadencesSize(); c++)
        {
            if(data.getCadencesPerFamily(s[p], c))
            {
                int sum = 1;
                int end = p + data.getCadence(c) + 1 < sSize ? p + data.getCadence(c) + 1 : sSize;
                for(int k = p + 1; k < end; k++)
                {
                    if(data.getCadencesPerFamily(s[k], c))
                    {
                        sum++;
                    }
                }

                if(data.cadenceType(c) == 1)
                {
                    if(sum > 1)
                    {
                        return true;
                    }
                }
                else
                {
                    if(sum > data.getCadence(c))
                    {
                        return true;
                    }
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

    return;
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
                bool feasibleFamily = true;

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
                                feasibleFamily = false;
                                break;
                            }

                            for(int k = p; k < end; k++)
                            {
                                if(data.getCadencesPerFamily(s[k], c))
                                {
                                    sum++;

                                    if(sum > data.getCadence(c))
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
                bool feasibleFamily = true;

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
                                feasibleFamily = false;
                                break;
                            }

                            for(int k = p; k < end; k++)
                            {
                                if(data.getCadencesPerFamily(s[k], c))
                                {
                                    sum++;

                                    if(sum > data.getCadence(c))
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
                    s.insert(s.begin() + p, f);
                    sSize++;

                    data.setFamilySize(f, data.getFamilySize(f) - 1);
                }
            }
        }
    }

    return;
}

// for removal, beginSearch > 0 and endSearch < sSize, because there is never a search in these positions
void removalCL(vector<int> &s, Data &data, int beginSearch, int endSearch, vector<int> &feasiblePositions)
{
    int feasiblePositionsSize = feasiblePositions.size(), sSize = s.size();

    // remove candidates that are in the range between beginSearch and endSearch, which will be updated later
    for(int i = 1; i < feasiblePositionsSize; i++)
    {
        if(feasiblePositions[i] <= endSearch)
        {
            if(feasiblePositions[i] >= beginSearch)
            {
                feasiblePositions.erase(feasiblePositions.begin() + i);
                i--;
                feasiblePositionsSize--;
            }
        }
        else
        {
            feasiblePositions[i]--;
        }
    }

    // first and last feasible positions should always be first and last positions of the solution
    if(feasiblePositionsSize == 0)
    {
        feasiblePositions.push_back(0);
        feasiblePositions.push_back(sSize - 1);
        feasiblePositionsSize += 2; // this variable is not used from now on
    }

    for(int p = beginSearch; p < endSearch; p++)
    {
        bool feasiblePosition = true;

        for(int c = 0; c < data.getCadencesSize(); c++)
        {
            int begin = p - data.getCadence(c) > 0 ? p - data.getCadence(c) : 0; 
            int end = p + data.getCadence(c) + 1 < sSize ? p + data.getCadence(c) + 1 : sSize;

            if(data.cadenceType(c) == 1)
            {
                if(!data.getCadencesPerFamily(s[p], c)) // if s[p] has cadence c, there is no job with cadence c in the range
                {
                    int lastTrueK = -data.getDimension();

                    for(int k = begin; k < p; k++)
                    {
                        if(data.getCadencesPerFamily(s[k], c))
                        {
                            lastTrueK = k;
                        }
                    }

                    for(int k = p + 1; k < end; k++)
                    {
                        if(data.getCadencesPerFamily(s[k], c))
                        {
                            if(k - lastTrueK - 1 < data.getCadence(c) + 1)
                            // check if the positions in the middle are less than data.getCadence(1) + 1
                            {
                                feasiblePosition = false;
                                break;
                            }
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

    sort(feasiblePositions.begin(), feasiblePositions.end());

    return;
}

void removal(vector<int> &s, Data &data, int p)
{
    data.setFamilySize(s[p], data.getFamilySize(s[p]) + 1);
    s.erase(s.begin() + p);
    return;
}

void swapSearch(vector<int> &s, Data &data, int beginSearch, int endSearch, vector<pair<int, int>> &feasiblePairs, int &i)
{
    int sSize = s.size();
    for(int p1 = beginSearch; p1 < endSearch - 1; p1++)
    {
        for(int p2 = p1 + 1; p2 < endSearch; p2++)
        {
            bool feasiblePair = true;

            for(int c = 0; c < data.getCadencesSize(); c++)
            {
                // if both jobs or none have cadence c, there is no need to check feasibility
                if(data.getCadencesPerFamily(s[p1], c))
                {
                    if(!data.getCadencesPerFamily(s[p2], c))
                    {
                        int begin = p2 - data.getCadence(c) > 0 ? p2 - data.getCadence(c) : 0;
                        int end = p2 + data.getCadence(c) + 1 < sSize ? p2 + data.getCadence(c) + 1 : sSize;

                        if(data.cadenceType(c) == 1)
                        {
                            if(p1 > begin)
                            {
                                begin = p1 + data.getCadence(c) + 1;
                                // there is no job with cadence c closer to p1 than data.getCadence(c) positions
                            }

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
                            if(p1 > begin)
                            {
                                begin = p1 + 1;
                                // position p1 will be filled by s[p2], which doesn't have cadence c
                            }

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
                }
                else
                {
                    if(data.getCadencesPerFamily(s[p2], c))
                    {
                        int begin = p1 - data.getCadence(c) > 0 ? p1 - data.getCadence(c) : 0;
                        int end = p1 + data.getCadence(c) + 1 < sSize ? p1 + data.getCadence(c) + 1 : sSize;

                        if(data.cadenceType(c) == 1)
                        {
                            if(p2 < end)
                            {
                                end = p2 - data.getCadence(c);
                                // there is no job with cadence c closer to p2 than data.getCadence(c) positions
                            }

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
                            if(p2 < end)
                            {
                                end = p2;
                                // position p2 will be filled by s[p1], which doesn't have cadence c
                            }
                            
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
            }

            if(feasiblePair)
            {
                feasiblePairs.insert(feasiblePairs.begin() + i++, make_pair(p1, p2));
            }
        }
    }

    return;
}

void swapCL(vector<int> &s, Data &data, int beginSearch, int endSearch, vector<pair<int, int>> &feasiblePairs)
{
    int i = 0;
    swapSearch(s, data, beginSearch, endSearch, feasiblePairs, i);
    return;
}

void swapCL(vector<int> &s, Data &data, int beginSearchP1, int endSearchP1, int beginSearchP2, int endSearchP2,
    vector<pair<int, int>> &feasiblePairs)
{
    // remove candidates that are in the range between beginSearchP1/P2 and endSearchP1/P2, which will be updated later
    int feasiblePairsSize = feasiblePairs.size();

    if(endSearchP1 >= beginSearchP2)
    {
        int i = 0; // i should store the position in which new pairs will start to be inserted, so the vector keeps sorted

        for(; i < feasiblePairsSize; i++)
        {
            if((feasiblePairs[i].first >= beginSearchP1)&&(feasiblePairs[i].first < endSearchP2) ||
               (feasiblePairs[i].second >= beginSearchP1)&&(feasiblePairs[i].second < endSearchP2))
            {
                feasiblePairs.erase(feasiblePairs.begin() + i);
                i--;
                feasiblePairsSize--;
            }
            else if(feasiblePairs[i].first >= endSearchP2)
            {
                break;
            }
        }

        swapSearch(s, data, beginSearchP1, endSearchP2, feasiblePairs, i);
    }
    else
    {
        int i1 = 0; // i1 should store the position in which new pairs will start to be inserted around p1,
                    // so the vector keeps sorted
        for(; i1 < feasiblePairsSize; i1++)
        {
            if((feasiblePairs[i1].first >= beginSearchP1)&&(feasiblePairs[i1].first < endSearchP1) ||
               (feasiblePairs[i1].second >= beginSearchP1)&&(feasiblePairs[i1].second < endSearchP1))
            {
                feasiblePairs.erase(feasiblePairs.begin() + i1);
                i1--;
                feasiblePairsSize--;
            }
            else if(feasiblePairs[i1].first >= endSearchP1)
            {
                break;
            }
        }

        swapSearch(s, data, beginSearchP1, endSearchP1, feasiblePairs, i1);

        feasiblePairsSize = feasiblePairs.size();

        int i2 = i1; // i2 should store the position in which new pairs will start to be inserted around p2,
                     // so the vector keeps sorted
        for(; i2 < feasiblePairsSize; i2++)
        {
            if((feasiblePairs[i2].first >= beginSearchP2)&&(feasiblePairs[i2].first < endSearchP2) ||
               (feasiblePairs[i2].second >= beginSearchP2)&&(feasiblePairs[i2].second < endSearchP2))
            {
                feasiblePairs.erase(feasiblePairs.begin() + i2);
                i2--;
                feasiblePairsSize--;
            }
            else if(feasiblePairs[i2].first >= endSearchP2)
            {
                break;
            }
        }

        swapSearch(s, data, beginSearchP2, endSearchP2, feasiblePairs, i2);
    }

    return;
}

void swap(vector<int> &s, int p1, int p2)
{
    s.insert(s.begin() + p2, s[p1]);
    s.insert(s.begin() + p1, s[p2 + 1]);
    s.erase(s.begin() + p1 + 1);
    s.erase(s.begin() + p2 + 1);
    return;
}

void perturbation(vector<int> &s, Data &data, vector<int> &perturbationType)
{
    int t = rand()%perturbationType.size(), sSize = s.size();
    
    if(perturbationType[t] == 1)
    {
        int n = sSize/2 < data.getDimension()/20 ? sSize/2 : data.getDimension()/20;

        if(n == 0)
        {
            return;
        }

        vector<int> feasiblePositions;
        removalCL(s, data, 1, sSize - 1, feasiblePositions);

        int i = rand()%feasiblePositions.size();
        removal(s, data, feasiblePositions[i]);
        sSize--;

        for(int j = 1; j < n; j++)
        {
            // update candidate list
            int beginSearch = feasiblePositions[i] - data.getMaxCadence() > 1 ? feasiblePositions[i] - data.getMaxCadence() : 1;
            int endSearch = feasiblePositions[i] + data.getMaxCadence() < sSize - 1 ?
                feasiblePositions[i] + data.getMaxCadence() : sSize - 1;
            removalCL(s, data, beginSearch, endSearch, feasiblePositions);

            i = rand()%feasiblePositions.size();
            removal(s, data, feasiblePositions[i]);
            sSize--;
        }
    }
    else
    {
        vector<pair<int, int>> feasiblePairs;
        swapCL(s, data, 0, sSize, feasiblePairs);

        int feasiblePairsSize = feasiblePairs.size();
        if(feasiblePairsSize)
        {
            int i = rand()%feasiblePairsSize;
            swap(s, feasiblePairs[i].first, feasiblePairs[i].second);

            feasiblePairs.erase(feasiblePairs.begin() + i);
            feasiblePairsSize--;

            int n = sSize/2 < data.getDimension()/20 ? sSize/2 : data.getDimension()/20;

            if(feasiblePairsSize < n - 1)
            {
                n = feasiblePairsSize;
            }

            int k = 0;
            vector<pair<int,int>> pairsStorage;
            for(int j = 1; j < n; )
            {
                vector<int> newS = s;
                i = rand()%feasiblePairsSize;
                swap(newS, feasiblePairs[i].first, feasiblePairs[i].second);

                if(isInfeasible(newS, data))
                {
                    k++;

                    if(k > 4)
                    {
                        k = 0;
                        j++;
                    }
                    
                    pairsStorage.push_back(feasiblePairs[i]);
                    feasiblePairs.erase(feasiblePairs.begin() + i);
                    feasiblePairsSize--;

                    if(!feasiblePairsSize)
                    {
                        break;
                    }
                }
                else
                {
                    s = newS;

                    int pairsStorageSize = pairsStorage.size();
                    for(int l = 0; l < pairsStorageSize; l++)
                    {
                        feasiblePairs.push_back(pairsStorage[l]);
                        feasiblePairsSize++;
                    }

                    pairsStorage.clear();

                    feasiblePairs.erase(feasiblePairs.begin() + i);
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

    perturbationType.erase(perturbationType.begin() + t);

    if(perturbationType.empty())
    {
        perturbationType.push_back(1);
        perturbationType.push_back(1);
        perturbationType.push_back(2);
        perturbationType.push_back(2);
    }

    return;
}

int heuristic(Data data, vector<int> &bestS)
{
    auto beginTime = chrono::system_clock::now();

    int seed = time(NULL);
    srand(seed);

    Data fullData = data;
    int bestSSize = 0;

    for(int i = 0; i < 5; i++)
    {
        vector<int> currentBestS;
        data = fullData;

        construction(currentBestS, data);
        int currentBestSSize = currentBestS.size();
        Data currentBestData = data;

        vector<int> perturbationType{1, 1, 2, 2};
        
        for(int j = 0; j < data.getDimension(); j++)
        {
            vector<int> s = currentBestS;
            data = currentBestData;

            if(s.size() == data.getDimension())
            {
                break;
            }
            
            perturbation(s, data, perturbationType);

            if(isInfeasible(s, data))
            {
                cout << "INFEASIBILITY DETECTED" << endl;
                exit(1);
            }
            
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

        if(bestSSize == data.getDimension())
        {
            break;
        }

        chrono::duration<double> currentTime = chrono::system_clock::now() - beginTime;
        if(currentTime.count() > 600)
        {
            break;
        }
    }

    return bestSSize;
}