#include "neighbor_operator.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <functional>
#include <map>
#include <ranges>
using namespace std;

NeighborOperator::NeighborOperator() {
    operators[1] = &NeighborOperator::random_swap;
    operators[2] = &NeighborOperator::random_swap_sub;
    operators[5] = &NeighborOperator::random_reversing;
    operators[6] = &NeighborOperator::random_swap_sub_reverse;
    operators[7] = &NeighborOperator::random_insert_sub_reverse;
}

vector<int> NeighborOperator::random_operator(vector<int> solution, const int patience, const bool verbose) {
    vector<int> keys;
    for (const auto &fst: operators | views::keys) {
        keys.push_back(fst);
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, keys.size() - 1);

    const int choice = keys[dist(gen)];
    return operators[choice](solution, patience, verbose);

}

vector<int> NeighborOperator::random_swap(const vector<int> solution, int patience, const bool verbose) {
    if (verbose) cout << "random swap" << endl;
    vector<int> new_solution = solution;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, solution.size() - 2);

    while (patience-- > 0) {
        int i = dist(gen);
        int j = dist(gen);
        if (i != j && new_solution[i] != 0 && new_solution[j] != 0) {
            swap(new_solution[i], new_solution[j]);
            break;
        }
    }
    return new_solution;
}

vector<int> NeighborOperator::random_swap_sub(const vector<int> solution, int patience, const bool verbose) {
    if (verbose) cout << "random swap of subsequence" << endl;
    vector<int> new_solution = solution;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist_k(2, 6);
    uniform_int_distribution<> dist(1, solution.size() - 3);

    while (patience-- > 0) {
        int k = dist_k(gen);
        int i = dist(gen);
        int j = dist(gen);

        if (abs(i - j) > k && new_solution[i] != 0 && new_solution[j] != 0) {
            vector<int> sub_i(new_solution.begin() + i, new_solution.begin() + i + k);
            vector<int> sub_j(new_solution.begin() + j, new_solution.begin() + j + k);

            if ((i + k <= j || j + k <= i) && i + k < new_solution.size() && j + k < new_solution.size()) {
                ranges::copy(sub_j, new_solution.begin() + i);
                ranges::copy(sub_i, new_solution.begin() + j);
                break;
            }
        }
    }
    return new_solution;
}

vector<int> NeighborOperator::random_reversing(const vector<int> solution, int patience, const bool verbose) {
    if (verbose) cout << "random reversing a subsequence" << endl;
    vector<int> new_solution = solution;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, solution.size() - 2);

    while (patience-- > 0) {
        int i = dist(gen);
        int j = dist(gen);

        if (i != j) {
            if (i > j) swap(i, j);
            reverse(new_solution.begin() + i, new_solution.begin() + j + 1);
            break;
        }
    }
    return new_solution;
}

vector<int> NeighborOperator::random_swap_sub_reverse(const vector<int> solution, int patience, const bool verbose) {
    if (verbose) cout << "random swap of reversed subsequence" << endl;
    vector<int> new_solution = solution;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist_k(2, 6);
    uniform_int_distribution<> dist(1, solution.size() - 3);

    while (patience-- > 0) {
        int k = dist_k(gen);
        int i = dist(gen);
        int j = dist(gen);

        if (abs(i - j) > k && new_solution[i] != 0 && new_solution[j] != 0) {
            vector<int> sub_i(new_solution.begin() + i, new_solution.begin() + i + k);
            vector<int> sub_j(new_solution.begin() + j, new_solution.begin() + j + k);

            ranges::reverse(sub_i);
            ranges::reverse(sub_j);

            if ((i + k <= j || j + k <= i) && i + k < new_solution.size() && j + k < new_solution.size()) {
                ranges::copy(sub_j, new_solution.begin() + i);
                ranges::copy(sub_i, new_solution.begin() + j);
                break;
            }

        }
    }
    return new_solution;
}

vector<int> NeighborOperator::random_insert_sub_reverse(const vector<int> solution, int patience, const bool verbose) {
    if (verbose) cout << "random insertion of reversed subsequence" << endl;
    vector<int> new_solution = solution;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist_k(2, 6);
    uniform_int_distribution<> dist(1, solution.size() - 3);

    while (patience-- > 0) {
        int k = dist_k(gen);
        int i = dist(gen);
        int j = dist(gen);

        if (abs(i - j) > k) {
            if (j + k <= solution.size() && i + k <= solution.size()) {
                vector<int> sub_j(new_solution.begin() + j, new_solution.begin() + j + k);
                ranges::reverse(sub_j);

                if (i < j) {
                    new_solution.erase(new_solution.begin() + j, new_solution.begin() + j + k);
                    new_solution.insert(new_solution.begin() + i, sub_j.begin(), sub_j.end());
                } else {
                    new_solution.insert(new_solution.begin() + i, sub_j.begin(), sub_j.end());
                    new_solution.erase(new_solution.begin() + j + k, new_solution.begin() + j + 2 * k);
                }
                break;
            }

        }
    }
    return new_solution;
}
