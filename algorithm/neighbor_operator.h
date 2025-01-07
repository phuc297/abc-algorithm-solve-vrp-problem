#ifndef NEIGHBOR_OPERATOR_H
#define NEIGHBOR_OPERATOR_H
#include <vector>
#include <functional>
#include <map>

using namespace std;

class NeighborOperator {
    map<int, function<vector<int>(vector<int>&, int, bool)>> operators;
public:
    NeighborOperator();
    vector<int> random_operator(vector<int> solution, const int patience = 10, const bool verbose = false);
    static vector<int> random_swap(const vector<int> solution, int patience, const bool verbose);
    static vector<int> random_swap_sub(const vector<int> solution, int patience, const bool verbose);
    static vector<int> random_reversing(const vector<int> solution, int patience, const bool verbose);
    static vector<int> random_swap_sub_reverse(const vector<int> solution, int patience, const bool verbose);
    static vector<int> random_insert_sub_reverse(const vector<int> solution, int patience, const bool verbose);
};

#endif //NEIGHBOR_OPERATOR_H
