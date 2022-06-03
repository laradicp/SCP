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
    if(s[p1] == s[p2])
    {
        return false;
    }

    if(p1 > p2)
    {
        int aux = p1;
        p1 = p2;
        p2 = aux;
    }

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

void swap(vector<int> &s, int p1, int p2)
{
    int aux = s[p1];
    s[p1] = s[p2];
    s[p2] = aux;
    return;
}

void perturbation(vector<int> &s, Data &data, vector<char> &infeasibleSwapPos)
{
    int t = 1, sSize = s.size();
    int n = sSize/2 < data.getDimension()/10 ? sSize/2 : data.getDimension()/10;

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
        list<int> computedPos;
        for(int j = 0; j < n; j++)
        {
            int p1 = rand()%sSize;
            while(infeasibleSwapPos[p1] == '1') // it will find a '0' because n = min(sSize/2, data.getDimension()/10)
            {
                if(++p1 == sSize)
                {
                    p1 = 0;
                }
            }

            infeasibleSwapPos[p1] = '1';
            computedPos.push_back(p1);
            
            int begin = rand()%sSize;
            for(int p2 = begin; p2 < sSize; p2++)
            {
                if((infeasibleSwapPos[p2] == '0')&&(swapFeasibility(s, data, p1, p2)))
                {
                    swap(s, p1, p2);

                    // clean infeasibleSwapPos
                    for(auto iter = computedPos.begin(); iter != computedPos.end();)
                    {
                        infeasibleSwapPos[*iter] = '0';
                        computedPos.erase(iter++);
                    }
                    
                    break;
                }
            }

            if(!computedPos.empty()) // if swap not already performed, continue search
            {
                for(int p2 = 0; p2 < begin; p2++)
                {
                    if((infeasibleSwapPos[p2] == '0')&&(swapFeasibility(s, data, p1, p2)))
                    {
                        swap(s, p1, p2);

                        // clean infeasibleSwapPos
                        for(auto iter = computedPos.begin(); iter != computedPos.end();)
                        {
                            infeasibleSwapPos[*iter] = '0';
                            computedPos.erase(iter++);
                        }
                        
                        break;
                    }
                }
            }
        }

        // clean infeasibleSwapPos
        for(auto iter = computedPos.begin(); iter != computedPos.end();)
        {
            infeasibleSwapPos[*iter] = '0';
            computedPos.erase(iter++);
        }
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

    vector<char> infeasibleSwapPos(data.getDimension(), '0');

    vector<int> currentBestS;
    data = fullData;

    construction(currentBestS, data);
    int currentBestSSize = currentBestS.size();
    Data currentBestData = data;
    
    int iterMax = data.getDimension()/2;
    for(int j = 0; j < iterMax; j++)
    {
        vector<int> s = currentBestS;
        data = currentBestData;

        if(s.size() == data.getDimension())
        {
            break;
        }
        
        perturbation(s, data, infeasibleSwapPos);
        
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
