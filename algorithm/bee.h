#ifndef BEE_H
#define BEE_H
#include "../utils/problem.h"
#include "../utils/solution.h"
#include <vector>

using namespace std;

class BeeColony {
private:
    Problem problem;
    int n_epoch;
    int n_initials;
    int n_onlookers;
    int search_limit;
public:
    BeeColony(Problem problem, int n_epoch, int n_initials, int n_onlookers, int search_limit);
    double fitness();
    vector<int> solve();
};



#endif //BEE_H
