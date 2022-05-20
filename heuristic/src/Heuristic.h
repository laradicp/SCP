#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "Data.h"
#include <string>
#include <chrono>
#include <list>

struct FeasiblePairsAnalysis {
    int counter = 0;
    std::pair<double, double> biggestFrac = std::make_pair(0, 0);
    std::pair<double, double> smallestFrac = std::make_pair(__INT_MAX__, __INT_MAX__);
    std::pair<double, double> fracSum = std::make_pair(0, 0);
};

bool isInfeasible(std::vector<int> &s, Data &data);
void construction(std::vector<int> &s, Data &data);
void insertion(std::vector<int> &s, Data &data);
void removalCL(std::vector<int> &s, Data &data, int beginSearch, int endSearch, std::list<int> &feasiblePositions,
    int &feasiblePositionsSize);
void removal(std::vector<int> &s, Data &data, int p);
bool swapFeasibility(std::vector<int> &s, Data &data, int p1, int p2);
void swapCL(std::vector<int> &s, Data &data, int beginSearch, int endSearch, std::list<std::pair<int, int>> &feasiblePairs,
    int &feasiblePairsSize);
void swapCL(std::vector<int> &s, Data &data, int beginSearch1, int endSearch1, int beginSearch2, int endSearch2,
    std::list<std::pair<int, int>> &feasiblePairs, int &feasiblePairsSize);
void swap(std::vector<int> &s, int p1, int p2);
void perturbation(std::vector<int> &s, Data &data, FeasiblePairsAnalysis &feasiblePairsAnalysis);
int heuristic(Data data, std::vector<int> &bestS, FeasiblePairsAnalysis &feasiblePairsAnalysis);

#endif
