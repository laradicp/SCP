#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "Data.h"
#include <string>
#include <chrono>

bool isInfeasible(std::vector<int> &s, Data &data);
void construction(std::vector<int> &s, Data &data);
void insertion(std::vector<int> &s, Data &data);
void perturbation(std::vector<int> &s, Data &data, std::vector<int> &perturbationType);
int heuristic(Data data, std::vector<int> &bestS);

#endif