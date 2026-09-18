//
// Created by fabian on 18.09.26.
//

#ifndef READINP_HPP
#define READINP_HPP

#include <string>
#include <vector>
#include <map>

struct Keyword {
    std::string keyword;
    std::vector<std::string> raw_input;
    std::map<std::string, std::vector<std::string>> parameters;
};

class Input {

    public:

        explicit Input(const std::string& filename);

        [[nodiscard]] const Keyword* get_keyword(const std::string& module_name) const;

    private:
        std::vector<Keyword> keywords_;
        std::map<std::string, size_t> keyword_index_;

        static std::string trim(const std::string& s);
        static std::vector<std::string> tokenize(const std::string& s);

        void parse(const std::string& filename);
};

Input read_input_file();

#endif //READINP_HPP
