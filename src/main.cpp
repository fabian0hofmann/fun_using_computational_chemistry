#include <iostream>
#include <fstream>
#include "inputlib/readinp.hpp"

int main() {

    const auto input = read_input_file();

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