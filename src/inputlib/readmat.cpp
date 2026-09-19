#include "readmat.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace {

    std::string trim(const std::string& s) {
        const size_t start = s.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) return "";
        const size_t end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }

    std::vector<double> parse_row(const std::string& line) {
        std::istringstream iss(line);
        std::vector<double> values;
        double value;
        while (iss >> value) {
            values.push_back(value);
        }
        return values;
    }

}

Eigen::MatrixXd read_matrix(const std::string& filename) {

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("\nFatal error: No input file found under name " + filename + ".\n\n");
    }

    std::vector<std::vector<double>> rows;
    std::string line;
    bool inside_block = false;

    while (std::getline(file, line)) {
        const std::string trimmed = trim(line);
        if (trimmed.empty()) continue;

        if (trimmed == "$matrix") {
            inside_block = true;
            continue;
        }
        if (trimmed == "$end") {
            if (inside_block) break; // matrix block finished
            continue;
        }

        if (inside_block) {
            rows.push_back(parse_row(trimmed));
        }
    }

    if (rows.empty()) {
        throw std::runtime_error("\nFatal error: No $matrix block found in file " + filename + ".\n\n");
    }

    const size_t n_cols = rows.front().size();
    for (size_t i = 0; i < rows.size(); ++i) {
        if (rows[i].size() != n_cols) {
            throw std::runtime_error("\nFatal error: Inconsistent row length in matrix block at row " +
                                      std::to_string(i + 1) + ".\n\n");
        }
    }

    Eigen::MatrixXd matrix(rows.size(), n_cols);
    for (size_t i = 0; i < rows.size(); ++i) {
        for (size_t j = 0; j < n_cols; ++j) {
            matrix(static_cast<Eigen::Index>(i), static_cast<Eigen::Index>(j)) = rows[i][j];
        }
    }

    return matrix;
}