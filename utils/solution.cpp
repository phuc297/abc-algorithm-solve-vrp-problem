#include <iostream>
#include <vector>
#include <set>
#include "solution.h"

#include <algorithm>

#include "problem.h"

using namespace std;

Solution::Solution(const Problem &problem) {
    this->problem = problem;
}

vector<int> Solution::generate_solution(double alpha, double betta, int patience, bool verbose) const {
    const int MAXIMUM_PENALTY = 10000000;
    auto dists = problem.distances;
    auto demands = problem.demands;

    for (int itr = 0; itr < patience; itr++) {
        vector<int> i_loc;
        for (int i = 1; i < problem.dimension; i++) {
            i_loc.push_back(i);
        }

        vector<vector<int>> routes(problem.n_trucks, {0});

        int i = 0;
        while (i_loc.size() > 0){
            vector<int> route_dists;
            // Chọn ngẫu nhiên một vị trí
            int random_index = rand() % i_loc.size();
            int random_loc = i_loc[random_index];

            for (auto& route : routes) {
                int dist_to_loc = dists[route.back()][random_loc];
                int route_demand = 0;
                for (int loc : route) {
                    route_demand += demands[loc];
                }
                route_demand += demands[random_loc];

                int coef = (route_demand > problem.capacity) ? MAXIMUM_PENALTY :
                           static_cast<int>(alpha * i * route.size() +
                                            betta * max(0, route_demand - problem.capacity) +
                                            dist_to_loc);
                route_dists.push_back(coef);
            }

            auto min_iter = min_element(route_dists.begin(), route_dists.end());
            int min_index = distance(route_dists.begin(), min_iter);
            routes[min_index].push_back(random_loc);

            i_loc.erase(i_loc.begin() + random_index);
            i++;
        }

        vector<int> solution;
        for (const auto& route : routes) {
            solution.insert(solution.end(), route.begin(), route.end());
        }
        solution.push_back(0);
        if (check_solution(solution)) {
            return solution;
        }
    }
    vector<int> solution;
    return solution;
}

double Solution::compute_solution(const vector<int> &solution) const {
    double cost = 0;
    int i = 0;
    const vector<vector<double>> distances = problem.distances;
    while (i < solution.size()) {
        cost += distances[solution[i]][solution[i+1]];
        i++;
        if (i == solution.size()-1) {
            break;
        }
    }
    return cost;
}

void Solution::print_solution(const vector<int> &solution) const {
    int i_truck = 0;
    const double cost = compute_solution(solution);
    for (auto i=0; i<solution.size(); i++) {
        if (i != 0) {
            cout << "Route #" << ++i_truck << ": " ;
        }
        while (solution[i] != 0) {
            cout << solution[i++] << " ";
        }
        if (i > 0) {
            cout << endl;
        }
    }
    printf("%.4f\n", cost);
}

bool Solution::check_solution(const vector<int> &solution, const bool verbose) const {
    const int true_sol_size = problem.n_trucks + problem.dimension;
    if (true_sol_size != solution.size()) {
        if (verbose) {
            cout << "Solution size should be" << true_sol_size << endl;
        }
        return false;
    }

    if (const set<int> unique_elements(solution.begin(), solution.end()); unique_elements.size() != problem.dimension) {
        if (verbose) {
            cout << "Failed locations sanity check" << endl;
            for (int i = 0; i < problem.dimension; ++i) {
                if (!unique_elements.contains(i)) {
                    cout << "Missing: " << i << " location" << endl;
                    break;
                }
            }
        }
        return false;
    }

    vector<int> depots_index;
    for (int i = 0; i < solution.size(); i++) {
        if (solution[i] == 0) {
            depots_index.push_back(i);
        }
    }
    if (depots_index[0] != 0 || depots_index[depots_index.size()-1] != true_sol_size - 1) {
        if (verbose) {
            cout << "The end and the start should be depots" << endl;
        }
        return false;
    }
    for (int i = 0; i < depots_index.size() - 1; i++) {
        if (solution[i] == solution[i+1]) {
            if (verbose) {
                cout << "Several depots in a row" << endl;
            }
            return false;
        }
    }

    vector visited_matrx(problem.dimension, vector<int>(problem.dimension, 0));
    for (int i = 0; i < solution.size() - 1; i++) {
        const int from = solution[i];
        const int to = solution[i+1];
        visited_matrx[from][to] = 1;
    }
    // for (int i = 0; i < problem.dimension; i++) {
    //     for (int j = 0; j < problem.dimension; j++) {
    //         cout << visited_matrx[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // Check N_TRUCKS visited matrix
    int sumi0 = 0;
    int sum0j = 0;
    for (int i = 0; i < visited_matrx[0].size(); i++) {
        if (visited_matrx[0][i] == 1) {
            sumi0 += visited_matrx[0][i];
        }
    }
    for (int i = 0; i < visited_matrx[0].size(); i++) {
        if (visited_matrx[i][0] == 1) {
            sum0j += visited_matrx[i][0];
        }
    }
    if (!(sumi0 == problem.n_trucks && sum0j == problem.n_trucks)) {
        if (verbose) {
            cout << "\nVisited matrix invalidates." << endl;
        }
    }

    int sumij_i = 0;
    int sumij_j = 0;
    for (int i = 1; i < visited_matrx.size(); i++) {
        for (int j = 1; j < visited_matrx.size(); j++) {
            if (visited_matrx[i][j] == 1) {
                sumij_i += visited_matrx[i][j];
            }
        }
    }
    for (int j = 1; j < visited_matrx.size(); j++) {
        for (int i = 1; i < visited_matrx.size(); i++) {
            if (visited_matrx[i][j] == 1) {
                sumij_j += visited_matrx[i][j];
            }
        }
    }
    if (!(sumi0 == problem.n_trucks && sum0j == problem.n_trucks)) {
        if (verbose) {
            cout << "\nVisited matrix invalidates." << endl;
        }
    }
    // Check Capacity
    vector<int> solution_demands;
    for (int i = 0; i < depots_index.size() - 1; i++) {
        vector route(solution.begin()+depots_index[i], solution.begin()+depots_index[i+1]);
        int route_demand = 0;
        for (int p = 0; p < route.size(); p++) {
            route_demand += problem.demands[route[p]];
        }
        if (route_demand > problem.capacity) {
            if (verbose) {
                cout << "Capacity !" << endl;
            }
            return false;
        }
    }

    return true;
}



















