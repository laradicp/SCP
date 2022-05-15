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
void removalCL(vector<int> &s, Data &data, int beginSearch, int endSearch, list<int> &feasiblePositions, int &feasiblePositionsSize)
{
    int sSize = s.size();

    // remove candidates that are in the range between beginSearch and endSearch, which will be updated later
    for(auto iter = feasiblePositions.begin(); iter != feasiblePositions.end();)
    {
        if(*iter <= endSearch)
        {
            if(*iter >= beginSearch)
            {
                feasiblePositions.erase(iter++);
                feasiblePositionsSize--;
            }
            else
            {
                iter++;
            }
        }
        else
        {
            (*iter)--;
            iter++;
        }
    }

    // first and last feasible positions should always be first and last positions of the solution
    if(feasiblePositionsSize == 0)
    {
        feasiblePositions.push_back(0);
        feasiblePositions.push_back(sSize - 1);
        feasiblePositionsSize += 2;
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
            feasiblePositionsSize++;
        }
    }

    return;
}

void removal(vector<int> &s, Data &data, int p)
{
    data.setFamilySize(s[p], data.getFamilySize(s[p]) + 1);
    s.erase(s.begin() + p);
    return;
}

bool swapFeasibility(vector<int> &s, Data &data, int p1, int p2)
{
    int sSize = s.size();
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

    return feasiblePair;
}

void swapCL(vector<int> &s, Data &data, int beginSearch, int endSearch, list<pair<int, int>> &feasiblePairs,
    int &feasiblePairsSize)
{
    swapCL(s, data, beginSearch, endSearch, 0, endSearch, feasiblePairs, feasiblePairsSize);
    return;
}

void swapCL(vector<int> &s, Data &data, int beginSearch1, int endSearch1, int beginSearch2, int endSearch2,
    list<pair<int, int>> &feasiblePairs, int &feasiblePairsSize)
{
    // remove pairs that have any element in the range between beginSearch1/2 and endSearch1/2, which will be updated later
    int sSize = s.size();
    if(endSearch1 >= beginSearch2)
    {
        for(auto iter = feasiblePairs.begin(); iter != feasiblePairs.end();)
        {
            if(((*iter).first >= beginSearch1)&&((*iter).first < endSearch2) ||
               ((*iter).second >= beginSearch1)&&((*iter).second < endSearch2))
            {
                feasiblePairs.erase(iter++);
                feasiblePairsSize--;
            }
            else {
                iter++;
            }
        }

        for(int p1 = 0; p1 < beginSearch1; p1++)
        {
            for(int p2 = beginSearch1; p2 < endSearch2; p2++)
            {
                if(swapFeasibility(s, data, p1, p2))
                {
                    feasiblePairs.push_back(make_pair(p1, p2));
                    feasiblePairsSize++;
                }
            }
        }

        for(int p1 = beginSearch1; p1 < endSearch2; p1++)
        {
            for(int p2 = p1 + 1; p2 < sSize; p2++)
            {
                if(swapFeasibility(s, data, p1, p2))
                {
                    feasiblePairs.push_back(make_pair(p1, p2));
                    feasiblePairsSize++;
                }
            }
        }
    }
    else
    {
        for(auto iter = feasiblePairs.begin(); iter != feasiblePairs.end();)
        {
            if(((*iter).first >= beginSearch1)&&((*iter).first < endSearch1) ||
               ((*iter).second >= beginSearch1)&&((*iter).second < endSearch1) ||
               ((*iter).first >= beginSearch2)&&((*iter).first < endSearch2) ||
               ((*iter).second >= beginSearch2)&&((*iter).second < endSearch2))
            {
                feasiblePairs.erase(iter++);
                feasiblePairsSize--;
            }
            else {
                iter++;
            }
        }

        for(int p1 = 0; p1 < beginSearch1; p1++)
        {
            for(int p2 = beginSearch1; p2 < endSearch1; p2++)
            {
                if(swapFeasibility(s, data, p1, p2))
                {
                    feasiblePairs.push_back(make_pair(p1, p2));
                    feasiblePairsSize++;
                }
            }
            for(int p2 = beginSearch2; p2 < endSearch2; p2++)
            {
                if(swapFeasibility(s, data, p1, p2))
                {
                    feasiblePairs.push_back(make_pair(p1, p2));
                    feasiblePairsSize++;
                }
            }
        }

        for(int p1 = beginSearch1; p1 < endSearch1; p1++)
        {
            for(int p2 = p1 + 1; p2 < sSize; p2++)
            {
                if(swapFeasibility(s, data, p1, p2))
                {
                    feasiblePairs.push_back(make_pair(p1, p2));
                    feasiblePairsSize++;
                }
            }
        }

        for(int p1 = endSearch1; p1 < beginSearch2; p1++)
        {
            for(int p2 = beginSearch2; p2 < endSearch2; p2++)
            {
                if(swapFeasibility(s, data, p1, p2))
                {
                    feasiblePairs.push_back(make_pair(p1, p2));
                    feasiblePairsSize++;
                }
            }
        }

        for(int p1 = beginSearch2; p1 < endSearch2; p1++)
        {
            for(int p2 = p1 + 1; p2 < sSize; p2++)
            {
                if(swapFeasibility(s, data, p1, p2))
                {
                    feasiblePairs.push_back(make_pair(p1, p2));
                    feasiblePairsSize++;
                }
            }
        }
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

void perturbation(vector<int> &s, Data &data, FeasiblePairsAnalysis &feasiblePairsAnalysis)
{
    int t = rand()%2, sSize = s.size();
    int n = sSize/2 < data.getDimension()/25 ? sSize/2 : data.getDimension()/25;

    if(n == 0)
    {
        return;
    }

    if(t == 0) // removal
    {
        list<int> feasiblePositions;
        int feasiblePositionsSize = 0;
        removalCL(s, data, 1, sSize - 1, feasiblePositions, feasiblePositionsSize);

        int i = rand()%feasiblePositionsSize;
        auto iter = feasiblePositions.begin();
        while(i-- > 0)
        {
            iter++;
        }
        removal(s, data, *iter);
        sSize--;

        for(int j = 1; j < n; j++)
        {
            // update candidate list
            int beginSearch = *iter - data.getMaxCadence() > 1 ? *iter - data.getMaxCadence() : 1;
            int endSearch = *iter + data.getMaxCadence() < sSize - 1 ?
                *iter + data.getMaxCadence() : sSize - 1;
            removalCL(s, data, beginSearch, endSearch, feasiblePositions, feasiblePositionsSize);

            i = rand()%feasiblePositionsSize;
            iter = feasiblePositions.begin();
            while(i-- > 0)
            {
                iter++;
            }
            removal(s, data, *iter);
            sSize--;
        }
    }
    else // swap
    {
        list<pair<int, int>> feasiblePairs;
        int feasiblePairsSize = 0;
        swapCL(s, data, 0, sSize, feasiblePairs, feasiblePairsSize);

        if(feasiblePairsSize > 0)
        {
            feasiblePairsAnalysis.counter++;

            if(feasiblePairsSize/(double)sSize > feasiblePairsAnalysis.biggestFrac.first)
            {
                feasiblePairsAnalysis.biggestFrac.first = feasiblePairsSize/(double)sSize;
                feasiblePairsAnalysis.biggestFrac.second = feasiblePairsSize/(double)(sSize*sSize);
            }
            else if(feasiblePairsSize/(double)sSize < feasiblePairsAnalysis.smallestFrac.first)
            {
                feasiblePairsAnalysis.smallestFrac.first = feasiblePairsSize/(double)sSize;
                feasiblePairsAnalysis.smallestFrac.second = feasiblePairsSize/(double)(sSize*sSize);
            }

            feasiblePairsAnalysis.fracSum.first += feasiblePairsSize/(double)sSize;
            feasiblePairsAnalysis.fracSum.second += feasiblePairsSize/(double)(sSize*sSize);

            int i = rand()%feasiblePairsSize;
            auto iter = feasiblePairs.begin();
            while(i-- > 0)
            {
                iter++;
            }
            swap(s, (*iter).first, (*iter).second);

            for(int j = 1; j < n; j++)
            {
                int beginSearch1 = (*iter).first - data.getMaxCadence() > 0 ?
                    (*iter).first - data.getMaxCadence() : 0;
                int endSearch1 = (*iter).first + data.getMaxCadence() + 1 < sSize ?
                    (*iter).first + data.getMaxCadence() + 1 : sSize;
                int beginSearch2 = (*iter).second - data.getMaxCadence() > 0 ?
                    (*iter).second - data.getMaxCadence() : 0;
                int endSearch2 = (*iter).second + data.getMaxCadence() + 1 < sSize ?
                    (*iter).second + data.getMaxCadence() + 1 : sSize;

                swapCL(s, data, beginSearch1, endSearch1, beginSearch2, endSearch2, feasiblePairs, feasiblePairsSize);

                if(feasiblePairsSize > 0)
                {
                    feasiblePairsAnalysis.counter++;

                    if(feasiblePairsSize/(double)sSize > feasiblePairsAnalysis.biggestFrac.first)
                    {
                        feasiblePairsAnalysis.biggestFrac.first = feasiblePairsSize/(double)sSize;
                        feasiblePairsAnalysis.biggestFrac.second = feasiblePairsSize/(double)(sSize*sSize);
                    }
                    else if(feasiblePairsSize/(double)sSize < feasiblePairsAnalysis.smallestFrac.first)
                    {
                        feasiblePairsAnalysis.smallestFrac.first = feasiblePairsSize/(double)sSize;
                        feasiblePairsAnalysis.smallestFrac.second = feasiblePairsSize/(double)(sSize*sSize);
                    }

                    feasiblePairsAnalysis.fracSum.first += feasiblePairsSize/(double)sSize;
                    feasiblePairsAnalysis.fracSum.second += feasiblePairsSize/(double)(sSize*sSize);

                    int i = rand()%feasiblePairsSize;
                    iter = feasiblePairs.begin();
                    while(i-- > 0)
                    {
                        iter++;
                    }
                    swap(s, (*iter).first, (*iter).second);
                }
                else
                {
                    break;
                }
            }
        }
    }

    return;
}

int heuristic(Data data, vector<int> &bestS, FeasiblePairsAnalysis &feasiblePairsAnalysis)
{
    auto beginTime = chrono::system_clock::now();

    int seed = time(NULL);
    srand(seed);

    Data fullData = data;
    int bestSSize = 0;

    for(int i = 0; i < 15; i++)
    {
        vector<int> currentBestS;
        data = fullData;

        construction(currentBestS, data);
        int currentBestSSize = currentBestS.size();
        Data currentBestData = data;
        
        int iterMax = 2*data.getDimension();
        for(int j = 0; j < iterMax; j++)
        {
            vector<int> s = currentBestS;
            data = currentBestData;

            if(s.size() == data.getDimension())
            {
                break;
            }
            
            perturbation(s, data, feasiblePairsAnalysis);
            
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
