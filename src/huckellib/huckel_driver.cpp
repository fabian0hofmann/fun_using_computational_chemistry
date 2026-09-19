#include <iostream>
#include <fstream>
#include "../inputlib/readinp.hpp"
#include "../inputlib/readmat.hpp"

int main() {

    const auto input = read_input_file();

    const Keyword* huckel = input.get_keyword("huckel");
    if (!huckel){
        std::cerr << "Fatal: Tried to run Hückel module without $huckel keyword in the input file." << std::endl;
        return 1;
    }

    std::string matrix_filename = "matrix";

    const auto matrix_it = huckel->parameters.find("matrix");
    if (matrix_it == huckel->parameters.end() || matrix_it->second.empty()){
        std::cout << "\nNo matrix file name specified, defaulting to \"matrix\"." << "\n";
    }else{
        matrix_filename = matrix_it->second[0];
    }

    const auto huckel_matrix = read_matrix(matrix_filename);

    std::cout << "\nRead Hückel matrix:\n"
              << huckel_matrix << "\n\n";

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