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
bool comparePair(std::pair<int, int> p1, std::pair<int, int> p2);
bool swapFeasibility(std::vector<int> &s, Data &data, int p1, int p2);
void swapCL(std::vector<int> &s, Data &data, int beginSearch, int endSearch, std::vector<std::pair<int, int>> &feasiblePairs);
void swapCL(std::vector<int> &s, Data &data, int beginSearch1, int endSearch1, int beginSearch2, int endSearch2,
    std::vector<std::pair<int, int>> &feasiblePairs);
void swap(std::vector<int> &s, int p1, int p2);
void perturbation(std::vector<int> &s, Data &data);
int heuristic(Data data, std::vector<int> &bestS);

#endif