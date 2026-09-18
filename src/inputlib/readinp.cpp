#include "readinp.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

Input::Input(const std::string& filename) {
    parse(filename);
}

const Keyword* Input::get_keyword(const std::string& module_name) const {
    const auto it = keyword_index_.find(module_name);
    if (it == keyword_index_.end()) return nullptr;
    return &keywords_[it->second];
}

std::string Input::trim(const std::string& s) {
    const size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    const size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

std::vector<std::string> Input::tokenize(const std::string& s) {
    std::istringstream iss(s);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) tokens.push_back(token);
    return tokens;
}

void Input::parse(const std::string& filename) {

    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("\nFatal error: No input file found under name " + filename + ".\n\n");
    }

    std::string line;
    Keyword* current = nullptr;

    while (std::getline(file, line)) {
        std::string trimmed = trim(line);
        if (trimmed.empty()) continue;
        if (trimmed[0] == '$') {
            std::string name = trimmed.substr(1);
            if (name == "end") {
                current = nullptr;
                continue;
            }
            keywords_.push_back(Keyword{});
            current = &keywords_.back();
            current->keyword = trim(name);
            keyword_index_[current->keyword] = keywords_.size() - 1;
        } else {
            if (!current) {
                throw std::runtime_error("\nFatal: Parameter line found without active keyword. Offending line: " + line + ".\n\n");
            }
            current->raw_input.push_back(trimmed);

            auto tokens = tokenize(trimmed);
            if (!tokens.empty()) {
                std::string key = tokens.front();
                std::vector<std::string> values(tokens.begin() + 1, tokens.end());
                current->parameters[key] = values;
            }
        }
    }
}

Input read_input_file() {

    const Input input("job");

    if (const Keyword* huckel = input.get_keyword("huckel")) {
        std::cout << "\nFound Hückel module call with " << huckel->parameters.size() << " parameters. Here they come:\n\n";

        auto matrix_it = huckel->parameters.find("matrix");
        if (matrix_it != huckel->parameters.end() && !matrix_it->second.empty()) {
            std::cout << "Matrix file:\n" << matrix_it->second[0] << "\n\n";
        }

        auto alpha_it = huckel->parameters.find("alpha");
        if (alpha_it != huckel->parameters.end() && !alpha_it->second.empty()) {
            std::cout << "alpha value:\n" << alpha_it->second[0] << "\n\n";
        }

        auto beta_it = huckel->parameters.find("beta");
        if (beta_it != huckel->parameters.end() && !beta_it->second.empty()){
            std::cout << "beta value:\n" << beta_it->second[0] << "\n\n";
        }
    }

    return input;
}
