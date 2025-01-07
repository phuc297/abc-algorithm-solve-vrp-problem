#include "visualize.h"
#include "../include/matplotlibcpp.h"

namespace plt = matplotlibcpp;

vector<string> color_symbols = {
    "r",
    "g",
    "b",
    "c",
    "m",
    "y",
    "k",
    "orange",
    "pink",
    "brown",
    "gray",
    "purple",
    "indigo",
    "violet",
    "teal",
    "lime"
};

Visualize::Visualize() {
}

void Visualize::plot_solution(Problem problem, vector<int> solution, string tittle) {
    vector<double> x;
    vector<double> y;
    for (int i = 0; i < problem.dimension; i++) {
        x.push_back(problem.nodes[i].x);
        y.push_back(problem.nodes[i].y);
    }
    plt::plot(x, y, "o");

    int c_index = -1;
    for (int i = 0; i < solution.size() - 1; i++) {
        if (solution[i] == 0) {
            c_index++;
        }
        int index1 = solution[i];
        int index2 = solution[i + 1];
        vector<double> x_line = {static_cast<double>(problem.nodes[index1].x), static_cast<double>(problem.nodes[index2].x)};
        vector<double> y_line = {static_cast<double>(problem.nodes[index1].y), static_cast<double>(problem.nodes[index2].y)};
        plt::plot(x_line, y_line, {{"c", color_symbols[c_index]}, {"ls", "--"}});
    }
    int index1 = solution[solution.size() - 1];
    int index2 = solution[0];
    vector<double> x_line = {static_cast<double>(problem.nodes[index1].x), static_cast<double>(problem.nodes[index2].x)};
    vector<double> y_line = {static_cast<double>(problem.nodes[index1].y), static_cast<double>(problem.nodes[index2].y)};
    plt::plot(x_line, y_line, {{"c", color_symbols[c_index]}, {"ls", "--"}});

    plt::title(tittle);
    plt::show();
}

void Visualize::plot_2solution(Problem problem, vector<int> solution1, string tittle1, vector<int> solution2, string tittle2) {
    vector<double> x;
    vector<double> y;
    for (int i = 0; i < problem.dimension; i++) {
        x.push_back(problem.nodes[i].x);
        y.push_back(problem.nodes[i].y);
    }
    plt::figure_size(20, 12);
    plt::subplot(1,2,1);
    plt::plot(x, y, "o");
    int c_index = -1;
    for (int i = 0; i < solution1.size() - 1; i++) {
        if (solution1[i] == 0) {
            c_index++;
        }
        int index1 = solution1[i];
        int index2 = solution1[i + 1];
        vector<double> x_line = {static_cast<double>(problem.nodes[index1].x), static_cast<double>(problem.nodes[index2].x)};
        vector<double> y_line = {static_cast<double>(problem.nodes[index1].y), static_cast<double>(problem.nodes[index2].y)};
        plt::plot(x_line, y_line, {{"c", color_symbols[c_index]}, {"ls", "--"}});
    }
    int index1 = solution1[solution1.size() - 1];
    int index2 = solution1[0];
    vector<double> x_line = {static_cast<double>(problem.nodes[index1].x), static_cast<double>(problem.nodes[index2].x)};
    vector<double> y_line = {static_cast<double>(problem.nodes[index1].y), static_cast<double>(problem.nodes[index2].y)};
    plt::plot(x_line, y_line, {{"c", color_symbols[c_index]}, {"ls", "--"}});
    plt::title(tittle1);

    plt::subplot(1,2,2);
    plt::plot(x, y, "o");
    c_index = -1;
    for (int i = 0; i < solution2.size() - 1; i++) {
        if (solution2[i] == 0) {
            c_index++;
        }
        int index1 = solution2[i];
        int index2 = solution2[i + 1];
        vector<double> x_line = {static_cast<double>(problem.nodes[index1].x), static_cast<double>(problem.nodes[index2].x)};
        vector<double> y_line = {static_cast<double>(problem.nodes[index1].y), static_cast<double>(problem.nodes[index2].y)};
        plt::plot(x_line, y_line, {{"c", color_symbols[c_index]}, {"ls", "--"}});
    }
    index1 = solution2[solution2.size() - 1];
    index2 = solution2[0];
    x_line = {static_cast<double>(problem.nodes[index1].x), static_cast<double>(problem.nodes[index2].x)};
    y_line = {static_cast<double>(problem.nodes[index1].y), static_cast<double>(problem.nodes[index2].y)};
    plt::plot(x_line, y_line, {{"c", color_symbols[c_index]}, {"ls", "--"}});
    plt::title(tittle2);

    plt::show();
}
