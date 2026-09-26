#include <iostream>
#include <fstream>
#include "../inputlib/readinp.hpp"
#include "../inputlib/readmat.hpp"
#include "diagmat.hpp"
#include "huckelout.hpp"

int main() {

    std::cout << "\n\n";
    std::cout << "#################################################################################\n";
    std::cout << "#                                                                               #\n";
    std::cout << "#                        FUN USING COMPUTATIONAL CHEMISTRY                      #\n";
    std::cout << "#                                   HÜCKEL MODULE                               #\n";
    std::cout << "#                                                                               #\n";
    std::cout << "#################################################################################\n\n" << std::endl;
    std::cout << "For theoretical background, consult the \"THEORY\" document. For usage information, consult the \"README\".\n\n\n";

    const auto input = read_input_file();

    const Keyword* huckel = input.get_keyword("huckel");
    if (!huckel){
        std::cerr << "Fatal: Tried to run Hückel module without $huckel keyword in the input file." << std::endl;
        return 1;
    }

    std::string matrix_filename = "matrix";

    const auto matrix_it = huckel->parameters.find("matrix");
    if (matrix_it == huckel->parameters.end() || matrix_it->second.empty()){
        std::cout << "No matrix file name specified, defaulting to \"matrix\"." << "\n";
    }else{
        matrix_filename = matrix_it->second[0];
    }

    const auto huckel_matrix = read_matrix(matrix_filename);

    std::cout << ":: Hückel matrix ::\n"
              << huckel_matrix << "\n\n";

    const auto huckel_solver = diag_huckel_matrix(huckel_matrix);

    const auto alpha_it = huckel->parameters.find("alpha");
    if (alpha_it == huckel->parameters.end() || alpha_it->second.empty()){
        std::cerr << "Fatal: No alpha parameter found.\n";
    }
    const auto beta_it = huckel->parameters.find("beta");
    if (beta_it == huckel->parameters.end() || beta_it->second.empty()){
        std::cerr << "Fatal: No beta parameter found.\n";
    }
    const double alpha_val = stod(alpha_it->second[0]);
    const auto beta_val = stod(beta_it->second[0]);

    const auto electrons_it = huckel->parameters.find("electrons");
    if (electrons_it == huckel->parameters.end() || electrons_it->second.empty()){
        std::cerr << "Fatal: Electron number unspecified.\n";
    }
    const int electrons_val = stoi(electrons_it->second[0]);

    huckel_output(huckel_solver, alpha_val, beta_val, electrons_val);

    std::string debug_string;
    std::ifstream input_file("job");
    if (!input_file) {
        std::cerr << "Fatal in debug: No input file called 'job'." << std::endl;
    }
    while (std::getline(input_file, debug_string)) {
        std::cout << debug_string << std::endl;
    }

    return 0;
}