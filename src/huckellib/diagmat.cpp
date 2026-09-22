#include "diagmat.hpp"

Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> diag_huckel_matrix(const Eigen::MatrixXd& huckel_matrix){
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> huckel_solver(huckel_matrix);
    if (huckel_solver.info() != Eigen::Success){
        std::cerr << "Fatal: Diagonalization of Hückel matrix failed.";
    }else{
        std::cout << "\nDiagnostic: Successfully diagonalized Hückel matrix.\n";
    }
    return huckel_solver;
}

