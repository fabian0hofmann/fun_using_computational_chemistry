//
// Created by fabian on 22.09.26.
//

#ifndef FUN_USING_COMPUTATIONAL_CHEMISTRY_HUCKELOUT_HPP
#define FUN_USING_COMPUTATIONAL_CHEMISTRY_HUCKELOUT_HPP

#include <Eigen/Dense>
#include <iostream>

void huckel_output(const Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd>& huckel_solver, const double alpha, const double beta, const int nelec);

#endif //FUN_USING_COMPUTATIONAL_CHEMISTRY_HUCKELOUT_HPP
