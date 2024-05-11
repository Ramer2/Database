
#ifndef DATABASE_ROW_H
#define DATABASE_ROW_H

#include <vector>
#include <string>
#include "Table.h"

//for records
class Row {
    private:
        std::vector<std::string> attributes;
    public:
        explicit Row(std::vector<std::string>& input) {
            for (int i = 0; i < input.size(); i++) {
                attributes[i] = input[i];
            }
        }
        Row (std::vector<std::string>& input, std::vector<std::string>& specifier){//granted, that all the PKs are gonna be filled
            for (int i = 0, k = 0; i < attributes.size(); ++i) {
                if (attributes[i] == specifier[k]) {
                    attributes[i] = input[k];
                    k++;
                }
            }
        }
};


#endif //DATABASE_ROW_H
