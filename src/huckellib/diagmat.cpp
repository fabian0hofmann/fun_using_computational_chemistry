#include "diagmat.hpp"

Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> diag_huckel_matrix(const Eigen::MatrixXd& huckel_matrix){
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> huckel_solver(huckel_matrix);
    if (huckel_solver.info() != Eigen::Success){
        throw std::logic_error("Diagonalization of Hückel matrix failed.");
    }
    return huckel_solver;
}

