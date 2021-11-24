#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "Data.h"
#include <string>
#include <chrono>

bool isInfeasible(std::vector<int> &s, Data &data);
void construction(std::vector<int> &s, Data &data);
void insertion(std::vector<int> &s, Data &data);
void removalCL(std::vector<int> &s, Data &data, int beginSearch, int endSearch, std::vector<int> &feasiblePositions);
void removal(std::vector<int> &s, Data &data, int p);
void swapSearch(std::vector<int> &s, Data &data, int beginSearch, int endSearch, std::vector<std::pair<int, int>> &feasiblePairs,
    int &i);
void swapCL(std::vector<int> &s, Data &data, int beginSearch, int endSearch, std::vector<std::pair<int, int>> &feasiblePairs);
void swapCL(std::vector<int> &s, Data &data, int beginSearchP1, int endSearchP1, int beginSearchP2, int endSearchP2,
    std::vector<std::pair<int, int>> &feasiblePairs);
void swap(std::vector<int> &s, int p1, int p2);
void perturbation(std::vector<int> &s, Data &data, std::vector<int> &perturbationType);
int heuristic(Data data, std::vector<int> &bestS);

#endif