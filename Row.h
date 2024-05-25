#include <vector>
#include <string>
#include <variant>
#include <iostream>

#ifndef DATABASE_ROW_H
#define DATABASE_ROW_H

//for records
//should be done (sike)
class Row {
    using dataType = std::variant<int, float, std::string>;
    public:
    std::vector<dataType> attributes;

    explicit Row(std::vector<dataType> const& input, std::vector<std::string> types) {
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
};


#endif //DATABASE_ROW_H