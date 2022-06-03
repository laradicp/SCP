#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "Data.h"
#include <string>
#include <chrono>
#include <list>

bool isInfeasible(std::vector<int> &s, Data &data);
void construction(std::vector<int> &s, Data &data);
void insertion(std::vector<int> &s, Data &data);
void removalCL(std::vector<int> &s, Data &data, int beginSearch, int endSearch, std::list<int> &feasiblePositions,
    int &feasiblePositionsSize);
void removal(std::vector<int> &s, Data &data, int p);
bool swapFeasibility(std::vector<int> &s, Data &data, int p1, int p2);
void swap(std::vector<int> &s, int p1, int p2);
void perturbation(std::vector<int> &s, Data &data, std::vector<char> &infeasibleSwapPos);
int heuristic(Data data, std::vector<int> &bestS);

#endif
