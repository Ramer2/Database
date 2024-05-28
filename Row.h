#include <vector>
#include <string>
#include <variant>
#include <iostream>

#ifndef DATABASE_ROW_H
#define DATABASE_ROW_H

//for records
//should be done
class Row {
    using dataType = std::variant<int, float, std::string>;
    public:
    std::vector<dataType> attributes;

    Row(std::vector<dataType> const& input, std::vector<std::string> types) {
        if (input.size() != types.size()) {
            std::cout << "Internal error: Row constructor.";
            exit(-1);
        }
        for (int i = 0; i < input.size(); i++) {
            if (types[i] == "i" and std::holds_alternative<int>(input[i])) {
                attributes.emplace_back(input[i]);
            } else if (types[i] == "f" and std::holds_alternative<float>(input[i])) {
                attributes.emplace_back(input[i]);
            } else if (types[i] == "s" and std::holds_alternative<std::string>(input[i])){
                attributes.emplace_back(input[i]);
            } else {
                std::cout << "Syntax Error: wrong order of datatypes.";
                exit(-1);
            }
        }
    }

    Row(std::vector<std::variant<int, float, std::string>> const& input, std::vector<std::string> const& input_cols,
        std::vector<std::string> const& types, std::vector<std::string> const& cols) {
        auto pos = 0; //for input vectors
        for (int i = 0; i < cols.size(); i++) {
            if (cols[i] == input_cols[pos]) {
                if (types[i] == "i" and std::holds_alternative<int>(input[pos])) {
                    attributes.emplace_back(input[pos]);
                } else if (types[i] == "f" and std::holds_alternative<float>(input[pos])) {
                    attributes.emplace_back(input[pos]);
                } else if (types[i] == "s" and std::holds_alternative<std::string>(input[pos])){
                    attributes.emplace_back(input[pos]);
                } else {
                    std::cout << "Syntax Error: wrong order of datatypes.";
                    exit(-1);
                }
                pos++;
            } else {
                attributes.emplace_back("null");
            }
        }
    }
};


#endif //DATABASE_ROW_H