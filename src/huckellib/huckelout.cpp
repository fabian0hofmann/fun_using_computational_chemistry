#include "huckelout.hpp"

void huckel_output(const Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd>& huckel_solver, const double alpha, const double beta, const int nelec){

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
    std::cout << "----- Orbital energies -----\n\n";
    for (int i = 0; i < huckel_solver.eigenvalues().size(); i++){
        std::cout << "  e" << i << " = " << huckel_solver.eigenvalues()[i] << "\n";
    }
    std::cout << "\n";
    std::cout << "----- Total energy -----\n\n";
    double alpha_factor = 0;
    double beta_factor = 0;
    double total_energy = 0;
    for (int i = 0; i < nelec / 2; i++){
        alpha_factor += 2;
        beta_factor += 2 * huckel_solver.eigenvalues()[i];
        total_energy += 2.0 * (alpha + beta * huckel_solver.eigenvalues()[i]);
    }
    std::cout << "E_tot = " << alpha_factor << " * alpha + " << beta_factor << " * beta = " << total_energy << "\n\n";

    std::cout << "END OF OUTPUT" << std::endl;
}
