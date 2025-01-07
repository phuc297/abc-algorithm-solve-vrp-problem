#ifndef LOCAL_SEARCH_H
#define LOCAL_SEARCH_H
#include "../utils/solution.h"


class LocalSearch {
    Problem problem;
public:
    LocalSearch(Problem problem);
    vector<int> solve(vector<int> first_solution, int n_iter=50, bool verbose=false);
};



#endif //LOCAL_SEARCH_H
