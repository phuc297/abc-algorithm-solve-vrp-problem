#ifndef BEE_COLONY_H
#define BEE_COLONY_H
#include "../utils/problem.h"
#include "../utils/solution.h"
#include <vector>


class BeeColony {
    Problem problem;
    Solution solution;
    int n_epoch;
    int n_initials;
    int n_onlookers;
    int search_limit;
public:
    BeeColony(Problem problem, int n_epoch, int n_initials, int n_onlookers, int search_limit);
    double fitness(vector<int> solution);
    vector<int> solve(bool verbose=false);
};



#endif //BEE_COLONY_H
