#include "huckelout.hpp"

void huckel_output(const Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd>& huckel_solver){

    std::cout << "\n\n";
    std::cout << "#################################################################################\n";
    std::cout << "#                                                                               #\n";
    std::cout << "#                            HÜCKEL CALCULATION RESULT                          #\n";
    std::cout << "#                                                                               #\n";
    std::cout << "#################################################################################\n\n\n" << std::endl;
    std::cout << "----- Eigenvalues -----\n\n";
    std::cout << huckel_solver.eigenvalues() << "\n\n";
    std::cout << "----- Eigenvectors (orbitals) -----\n\n";
    std::cout << huckel_solver.eigenvectors() << "\n\n";

    std::cout << "END OF OUTPUT" << std::endl;
}
