#include "bee_colony.h"
#include <iostream>
#include <numeric>
#include <ostream>

#include "local_search.h"
#include "neighbor_operator.h"


BeeColony::BeeColony(Problem problem, int n_epoch, int n_initials, int n_onlookers, int search_limit):
    problem(problem),
    solution(problem),
    n_epoch(n_epoch),
    n_initials(n_initials),
    n_onlookers(n_onlookers),
    search_limit(search_limit) {}

double BeeColony::fitness(vector<int> solution) {
    double f = this->solution.compute_solution(solution);
    return 1.0/(1.0+f);
}

int roulette_wheel_selection(const std::vector<double>& probs) {
    double r = static_cast<double>(rand()) / RAND_MAX;
    double sum = 0.0;
    for (int i = 0; i < probs.size(); ++i) {
        sum += probs[i];
        if (r < sum) {
            return i;
        }
    }
    return probs.size() - 1;
}

vector<int> BeeColony::solve(bool verbose) {
    vector<vector<int>> solutions(n_initials);
    for (int i = 0; i < n_initials; i++) {
        do {
            solutions[i] = solution.generate_solution();
        } while (!solution.compute_solution(solutions[i]) != 0);
        // solution.print_solution(solutions[i]);
    }
    vector<double> fitnesses(n_initials);
    for (int i = 0; i < n_initials; i++) {
        fitnesses[i] = fitness(solutions[i]);
    }
    vector<int> counters(n_initials, 0);
    LocalSearch local_search(problem);
    NeighborOperator neighbor_operator;

    for (int epoch = 0; epoch < n_epoch; epoch++) {
        if (verbose) {
            cout << "epoch " << epoch+1 << "/" << n_epoch << endl;
        }
        // for each food source apply neighbor operator
        // *enhanced with local search based on neighbors operators
        for (int i = 0; i < n_initials; i++) {
            vector<int> ls_solution = local_search.solve(solutions[i], 50, false);
            double ifitness = fitness(ls_solution);
            if (ifitness > fitnesses[i]) {
                solutions[i] = ls_solution;
                fitnesses[i] = ifitness;
                counters[i] = 0;
            }
            else {
                counters[i]++;
            }
        }

        // for each onlooker select food source
        // *based on roulette wheel choice
        vector<vector<vector<int>>> neighbor_solutions(n_initials);
        vector<double> probs(n_initials, 0);
        double sum_fitness = 0.0;
        for (int i = 0; i < n_initials; i++) {
            sum_fitness += fitnesses[i];
        }
        for (int i = 0; i < n_initials; i++) {
            probs[i] = fitnesses[i] / sum_fitness;
        }

        for (int i = 0; i < n_onlookers; i++) {
            int roulette = roulette_wheel_selection(probs);
            vector<int> neighbor_solution;
            // do {
            //     neighbor_solution = neighbor_operator.random_operator(solutions[roulette]);
            // } while (solution.check_solution(neighbor_solution));
            neighbor_solution = local_search.solve(solutions[roulette], 50, false);
            neighbor_solutions[roulette].push_back(neighbor_solution);
        }

        for (int i = 0; i < n_initials; i++) {
            if (neighbor_solutions[i].size() > 0) {
                int best_index = -1;
                double best_fitness = -1;
                for (int j = 0; j < neighbor_solutions[i].size(); j++) {
                    double jfitness = fitness(neighbor_solutions[i][j]);
                    if (jfitness > best_fitness) {
                        best_index = j;
                        best_fitness = jfitness;
                    }
                }

                if (best_fitness > fitnesses[i]) {
                    solutions[i] = neighbor_solutions[i][best_index];
                    fitnesses[i] = best_fitness;
                    counters[i] = 0;
                }
                else {
                    counters[i]++;
                }

            }
        }

        // If a solution reaches the search limit, replace with a random neighbor
        for (int i = 0; i < n_initials; i++) {
            if (counters[i] > search_limit) {
                solutions[i] = local_search.solve(solutions[i], 50, false);
            }
        }
    }

    int best_idx = std::max_element(fitnesses.begin(), fitnesses.end()) - fitnesses.begin();
    vector<int> best_solution = solutions[best_idx];
    return best_solution;
}
