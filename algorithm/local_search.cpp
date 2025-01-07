#include "local_search.h"

#include <iostream>
#include <ostream>

#include "neighbor_operator.h"

LocalSearch::LocalSearch(Problem problem) {
    this->problem = problem;
}

vector<int> LocalSearch::solve(vector<int> first_solution, const int n_iter, bool verbose) {
    vector<int> best_solution = first_solution;
    Solution solution(problem);
    if (verbose) {
        cout << "Solving..." << endl;
        cout << "First solution:" << endl;
        solution.print_solution(first_solution);
    }
    int best_cost = solution.compute_solution(first_solution);
    NeighborOperator neighbor_operator;
    for (int i = 0; i < n_iter; i++) {
        vector<int> new_sol;
        do {
            new_sol = neighbor_operator.random_operator(first_solution, 50, false);
        } while (!solution.check_solution(new_sol, false));
        int new_cost = solution.compute_solution(new_sol);
        if (new_cost < best_cost && solution.check_solution(new_sol, false)) {
            best_cost = new_cost;
            best_solution = new_sol;
        }
    }
    return best_solution;
}
