#ifndef READMAT_HPP
#define READMAT_HPP

#include <Eigen/Dense>
#include <string>

Eigen::MatrixXd read_matrix(const std::string& filename);

#endif //READMAT_HPP
