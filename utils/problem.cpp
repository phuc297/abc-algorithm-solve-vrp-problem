#include "problem.h"
#include <cmath>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

int extract_number_trucks(const string& filename) {
    if (const size_t pos = filename.find('k'); pos != string::npos) {
        string numPart = filename.substr(pos + 1);
        string number = "";
        for (const char c : numPart) {
            if (isdigit(c)) {
                number += c;
            } else {
                break;
            }
        }
        if (!number.empty()) {
            return stoi(number);
        }
    }
    return -1;
}

Problem::Problem(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string key;

        if (line.find("NAME") != string::npos) {
            getline(ss, key, ':');
            ss >> name;
            n_trucks = extract_number_trucks(name);
        }
        else if (line.find("DIMENSION") != string::npos) {
            getline(ss, key, ':');
            ss >> dimension;
        }
        else if (line.find("CAPACITY") != string::npos) {
            getline(ss, key, ':');
            ss >> capacity;
        }
        else if (line.find("NODE_COORD_SECTION") != string::npos) {
            while (getline(file, line)) {
                ss = stringstream(line);
                int id, x, y;
                ss >> id >> x >> y;
                auto node = Node();
                node.x = x;
                node.y = y;
                nodes.push_back(node);
                if (id >= dimension) {
                    break;
                }
            }
        }
        else if (line.find("DEMAND_SECTION") != string::npos) {
            while (getline(file, line)) {
                ss = stringstream(line);
                int id, demand;
                ss >> id >> demand;
                demands.push_back(demand);
                if (id >= dimension) {
                    break;
                }
            }
        }
        else if (line.find("COMMENT") != string::npos) {
            // Tìm kiếm "Optimal value" trong COMMENT
            regex optimalRegex(R"(\bOptimal value: (\d+)\b)");
            smatch match;

            if (regex_search(line, match, optimalRegex)) {
                opt_cost = stoi(match[1]); // Lưu giá trị vào thuộc tính của class
            }
        }
    }

    file.close();

    const int n = dimension;
    distances = vector(n, vector<double>(n, -1));
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            const int dx = nodes[i].x - nodes[j].x;
            const int dy = nodes[i].y - nodes[j].y;
            distances[i][j] = sqrt(dx * dx + dy * dy);
            distances[j][i] = sqrt(dx * dx + dy * dy);
        }
    }
}

Problem::Problem(): n_trucks(0), dimension(0), capacity(0) {
}
