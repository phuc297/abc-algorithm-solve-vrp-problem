#ifndef PROBLEM_H
#define PROBLEM_H
#include <string>
#include <vector>

using namespace std;

struct Node {
    int x, y;
};

class Problem {
public:
    string name;
    int n_trucks;
    int dimension;
    int capacity;
    double opt_cost;
    vector<Node> nodes;
    vector<int> demands;
    vector<vector<double>> distances;
    explicit Problem(const string& filename);
    Problem();
};

#endif //PROBLEM_H
