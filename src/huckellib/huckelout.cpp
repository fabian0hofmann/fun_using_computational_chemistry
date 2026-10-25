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

    std::string list_orbitals(const std::vector<int>& orbitals){
        std::string s;
        for (size_t k = 0; k < orbitals.size(); k++){
            if (k > 0) s += ", ";
            s += "orbital number " + std::to_string(orbitals[k]);
        }
        return s;
    }

    std::vector<int> get_level_orbitals(const Level& level){
        std::vector<int> orbitals;
        for (int k = 0; k < level.size; k++){
            orbitals.push_back(level.first + k);
        }
        return orbitals;
    }

    std::string signed_term(const double x, const int precision = 4){
        std::ostringstream oss;
        oss << (x < 0.0 ? "- " : "+ ") << std::fixed << std::setprecision(precision) << std::abs(x);
        return oss.str();
    }

}

void huckel_output(const Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd>& huckel_solver, const double alpha, const double beta, const int nelec){

    std::cout << std::fixed << std::setprecision(4);

    const Eigen::VectorXd eigenvalues = huckel_solver.eigenvalues().reverse();
    const int norb = static_cast<int>(eigenvalues.size());
    const std::vector<Level> levels = find_levels(eigenvalues);
    const int nlevels = static_cast<int>(levels.size());

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
    std::cout << huckel_solver.eigenvectors().rowwise().reverse() << "\n\n";
    std::cout << "----- Orbital energies -----\n\n";
    for (int i = 0; i < norb; i++){
        std::string diagram;
        switch (occupation[i]){
            case 2: diagram = "[ \u2191\u2193 ]"; break;
            case 1: diagram = "[ \u2191 ]"; break;
            default: diagram = "[  ]"; break;
        }
        const double orbital_energy = alpha + beta * eigenvalues[i];
        std::cout << " e" << i << " = alpha " << signed_term(eigenvalues[i]) << " * beta = " << orbital_energy << "   occ. = " << occupation[i] << "  " << diagram << "\n";
    }
    std::cout << "\n";

    int homo_level_idx = -1;
    for (int i = 0; i < nlevels; i++){
        if (occupation[levels[i].first] > 0) homo_level_idx = i;
    }

    std::cout << "----- Frontier orbitals -----\n\n";

    std::vector<int> lumo_orbitals;

    if (homo_level_idx >= 0){
        const auto& homo_level = levels[homo_level_idx];
        std::vector<int> homo_orbitals, somo_orbitals, empty_orbitals;
        for (int k = 0; k < homo_level.size; k++){
            const int idx = homo_level.first + k;
            if (occupation[idx] == 2) homo_orbitals.push_back(idx);
            else if (occupation[idx] == 1) somo_orbitals.push_back(idx);
            else empty_orbitals.push_back(idx);
        }
        if (!homo_orbitals.empty()) std::cout << " HOMO: " << list_orbitals(homo_orbitals) << "\n";
        if (!somo_orbitals.empty()) std::cout << " SOMO: " << list_orbitals(somo_orbitals) << "\n";
        if (!empty_orbitals.empty()) lumo_orbitals = empty_orbitals;
        else if (homo_level_idx + 1 < nlevels) lumo_orbitals = get_level_orbitals(levels[homo_level_idx + 1]);
    } else{
        std::cout << " HOMO: none (no electrons)\n";
        if (nlevels > 0) lumo_orbitals = get_level_orbitals(levels[0]);
    }

    if (!lumo_orbitals.empty()){
        std::cout << " LUMO: " << list_orbitals(lumo_orbitals) << "\n";
    }else{
        std::cout << " No LUMO (all orbitals occupied).\n";
    }
    std::cout << "\n";

    std::cout << "----- Total energy -----\n\n";
    double alpha_factor = 0;
    double beta_factor = 0;
    double total_energy = 0;
    for (int i = 0; i < norb; i++){
        alpha_factor += occupation[i];
        beta_factor += occupation[i] * eigenvalues[i];
        total_energy += occupation[i] * (alpha + beta * eigenvalues[i]);
    }
    std::cout << " E_tot = " << alpha_factor << " * alpha " << signed_term(beta_factor) << " * beta = " << total_energy << "\n\n";

    std::cout << "END OF OUTPUT" << std::endl;
}

// give me access to this repo pls
