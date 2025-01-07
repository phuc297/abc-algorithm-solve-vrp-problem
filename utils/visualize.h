#ifndef VISUALIZE_H
#define VISUALIZE_H
#include "problem.h"
#include "solution.h"

using namespace std;

class Visualize {
public:
    Visualize();
    void plot_solution(Problem problem, vector<int> solution, string tittle);
    void plot_2solution(Problem problem, vector<int> solution1, string tittle1, vector<int> solution2, string tittle2);
};



#endif //VISUALIZE_H
