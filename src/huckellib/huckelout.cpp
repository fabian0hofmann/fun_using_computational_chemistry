#include "huckelout.hpp"

namespace {

    constexpr double DEGENERACY_TOL = 1e-8;

    struct Level {
        int first;  // index of first orbital in this level
        int size;   // number of degenerate orbitals
    };

    // Groups consecutive (sorted) eigenvalues into degenerate levels
    std::vector<Level> find_levels(const Eigen::VectorXd& eigenvalues){
        std::vector<Level> levels;
        const int norb = static_cast<int>(eigenvalues.size());
        int i = 0;
        while (i < norb){
            int j = i + 1;
            while (j < norb && std::abs(eigenvalues[j] - eigenvalues[i]) < DEGENERACY_TOL) j++;
            levels.push_back({i, j - i});
            i = j;
        }
        return levels;
    }

    // Returns e.g. "e2, e3, e4"
    std::string orbital_list(const Level& level){
        std::string s;
        for (int k = 0; k < level.size; k++){
            if (k > 0) s += ", ";
            s += "e" + std::to_string(level.first + k);
        }
        return s;
    }

    // Prints "alpha + x * beta" or "alpha - |x| * beta" depending on the sign of x
    std::string signed_term(const double x){
        const char* sign = (x < 0.0) ? "-" : "+";
        return std::string(sign) + " " + std::to_string(std::abs(x));
    }

}

void huckel_output(const Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd>& huckel_solver, const double alpha, const double beta, const int nelec){

    const auto& eigenvalues = huckel_solver.eigenvalues().reverse();
    const int norb = static_cast<int>(eigenvalues.size());
    const std::vector<Level> levels = find_levels(eigenvalues);

    std::vector<int> occupation(norb, 0);
    int remaining = nelec;
    for (const auto& [first, size] : levels){
        if (remaining <= 0) break;
        const int n = std::min(remaining, 2 * size);
        const int singly = std::min(n, size);   // first electron in each orbital
        const int doubly = n - singly;                // second electrons, only after all are singly occupied
        for (int k = 0; k < singly; k++) occupation[first + k] = 1;
        for (int k = 0; k < doubly; k++) occupation[first + k] = 2;
        remaining -= n;
    }

    if (remaining > 0){
        std::cerr << "Error: " << nelec << " electrons do not fit into " << norb << " orbitals (max. " << 2 * norb << " electrons). " << remaining << " electron(s) could not be placed.\n";
    }

    std::cout << "\n\n";
    std::cout << "#################################################################################\n";
    std::cout << "#                                                                               #\n";
    std::cout << "#                            HÜCKEL CALCULATION RESULT                          #\n";
    std::cout << "#                                                                               #\n";
    std::cout << "#################################################################################\n\n\n" << std::endl;
    std::cout << "----- Eigenvalues -----\n\n";
    std::cout << eigenvalues << "\n\n";
    std::cout << "----- Eigenvectors (orbitals) -----\n\n";
    std::cout << huckel_solver.eigenvectors().reverse() << "\n\n";
    std::cout << "----- Orbital energies -----\n\n";
    for (int i = 0; i < norb; i++){
        std::string diagram;
        switch (occupation[i]){
            case 2: diagram = "[ \u2191\u2193 ]"; break;
            case 1: diagram = "[ \u2191 ]"; break;
            default: diagram = "[  ]"; break;
        }
        std::cout << " e" << i << " = alpha " << signed_term(eigenvalues[i]) << " * beta = " << alpha + beta * eigenvalues[i] << "   occ. = " << occupation[i] << "  " << diagram << "\n";
    }
    std::cout << "\n";

    int homo = -1;
    for (int i = 0; i < norb; i++){
        if (occupation[i] > 0) homo = i;
    }
    const int lumo = homo + 1;

    std::cout << "----- Frontier orbitals -----\n\n";
    if (homo >= 0 && occupation[homo] == 2){
        std::cout << " HOMO: e" << homo << "\n";
    }else if (homo >= 0 && occupation[homo] == 1){
        std::cout << " SOMO: e" << homo << "\n";
    }else{
        std::cout << " HOMO: none (no electrons)\n";
    }
    if (lumo < norb){
        std::cout << " LUMO: e" << lumo << "\n";
    }else{
        std::cout << " No LUMO (all orbitals occupied).\n";
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
    std::cout << " E_tot = " << alpha_factor << " * alpha " << signed_term(beta_factor) << " * beta = " << total_energy << "\n\n";

    std::cout << "END OF OUTPUT" << std::endl;
}

// give me access to this repo pls
