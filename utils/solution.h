#ifndef SOLUTION_H
#define SOLUTION_H
#include <vector>
#include "problem.h"

using namespace std;

class Solution {
    Problem problem;
    // vector<vector<int>> routes;
    // vector<int> solution;
    // double cost = 0;
public:
    explicit Solution(const Problem &problem);
    vector<int> generate_solution(double alpha = 1.0,
                                   double betta = 0.5,
                                   int patience = 100,
                                   bool verbose = false) const;
    void print_solution(const vector<int> &solution) const;
    double compute_solution(const vector<int> &solution) const;
    bool check_solution(const vector<int> &solution, bool verbose = false) const;
};

#endif //SOLUTION_H
