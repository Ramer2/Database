
#ifndef DATABASE_ROW_H
#define DATABASE_ROW_H

#include <vector>
#include <string>

//for records
class Row {
    public:
    std::vector<std::string> attributes;
    explicit Row(std::vector<std::string>& input) {
            for (auto const& el : input) attributes.push_back(el);
        }
};


#endif //DATABASE_ROW_H
