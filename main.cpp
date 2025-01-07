#include <iostream>
#include <ostream>
#include <string>

#include "algorithm/bee_colony.h"
#include "algorithm/local_search.h"
#include "utils/visualize.h"
#include "utils/problem.h"
#include "utils/solution.h"
#include "include/matplotlibcpp.h"
#include <vector>
#include <filesystem>
#include <fstream>

namespace plt = matplotlibcpp;
namespace fs = filesystem;
using namespace std;

void write_result(string file_name, Solution vrp, const vector<int> &solution, double execution_time) {
    ofstream outfile(file_name);
    int i_truck = 0;
    const double cost = vrp.compute_solution(solution);
    for (auto i=0; i<solution.size(); i++) {
        if (i != 0) {
            outfile << "Route #" << ++i_truck << ": " ;
        }
        while (solution[i] != 0) {
            outfile << solution[i++] << " ";
        }
        if (i > 0) {
            outfile << endl;
        }
    }
    outfile << "cost " << cost << endl;
    outfile << "execution_time " << execution_time << endl;
    outfile.close();
}

void benchmarks(string folder_path) {
    vector<string> vrp_files;
    vector<double> execution_times;
    for (const auto &file : fs::directory_iterator(folder_path)) {
        if (file.is_regular_file() && file.path().extension() == ".vrp") {
            vrp_files.push_back(file.path().stem().string());
        }
    }

    string results_path = "../results" + string("/") + folder_path.back();
    if (!fs::exists(results_path)) {
        fs::create_directory(results_path);
    }
    for (int i = 0; i < 1; ++i) {
        cout << "Solve " << vrp_files[i] << endl;
        Problem problem(folder_path + "/" + vrp_files[i] + ".vrp");
        Solution solution(problem);
        clock_t tStart = clock();
        BeeColony abc_ag(problem, 20, 30, 20, 30);
        vector<int> abc_solution = abc_ag.solve(false);
        double execution_time = (double)(clock() - tStart) / CLOCKS_PER_SEC;

        string save_path = results_path + "/" + vrp_files[i] + ".out";
        write_result(save_path, solution, abc_solution, execution_time);
        cout << "--Save result at " << save_path << endl;
    }
}

void solve(string file_path, bool plot=false) {

    const Problem problem(file_path);
    const Solution solution(problem);

    const vector first_solution = solution.generate_solution();
    solution.print_solution(first_solution);
    // LocalSearch local_search(problem);
    // vector<int> ls_solution = local_search.solve(first_solution, 50, false);

    clock_t tStart = clock();
    BeeColony abc_ag(problem, 20, 30, 20, 30);
    const vector abc_solution = abc_ag.solve(true);
    double execution_time = (double)(clock() - tStart) / CLOCKS_PER_SEC;
    printf("Execution time: %.2fs\n", execution_time);
    cout << "Artificial Bee Colony solution: " << endl;
    solution.print_solution(abc_solution);

    if (plot) {
        Visualize visualize;
        visualize.plot_2solution(problem, first_solution, "Random solution", abc_solution, "Phuc solution");
    }
}

vector<double> read_result(string file_path){
    vector<double> result(2);
    ifstream file(file_path);
    if (!file.is_open()) {
        cerr << "Error opening file: " << file_path << endl;
        return result;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);

        if (line.find("cost") != string::npos) {
            string key;
            ss >> key >> result[0];
        } else if (line.find("execution_time") != string::npos) {
            string key;
            ss >> key >> result[1];
        }
    }
    file.close();
    return result;
}

void save_result_csv(string folder_path) {
    vector<string> vrp_files;
    for (const auto &file : fs::directory_iterator(folder_path)) {
        if (file.is_regular_file() && file.path().extension() == ".vrp") {
            vrp_files.push_back(file.path().stem().string());
        }
    }
    string results_path = "../results" + string("/") + folder_path.back();
    if (!fs::exists(results_path)) {
        cout << "Folder not exist" << endl;
        return;
    }
    string output_file = results_path + "/results.csv";
    ofstream csv_file(output_file);
    if (!csv_file.is_open()) {
        cout << "Error opening CSV file!" << endl;
        return;
    }
    csv_file << "phuc,Dimension,Number of Trucks,Capacity,Optimal Cost,ABC Cost,Execution Time" << endl;
    for (int i = 0; i < 4; ++i) {
        Problem problem(folder_path + "/" + vrp_files[i] + ".vrp");
        vector<double> result = read_result(results_path + "/" + vrp_files[i] + ".out");
        Solution solution(problem);
        csv_file << vrp_files[i] << ","
                 << problem.dimension << ","
                 << problem.n_trucks << ","
                 << problem.capacity << ","
                 << problem.opt_cost << ","
                 << result[0] << ","
                 << result[1] << endl;
    }
    csv_file.close();
    cout << "Results have been written to " << output_file << endl;
}

int main() {
    // solve("../benchmarks/A/A-n34-k5.vrp", true);

    // write_results();
    // benchmarks("../benchmarks/A");

    save_result_csv("../benchmarks/A");

    return 0;
}