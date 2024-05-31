#ifndef DATABASE_TABLE_H
#define DATABASE_TABLE_H

#include "Row.h"
#include <map>
#include <fmt/core.h>
#include <fmt/ranges.h>

class Table {
private:
    auto PKcheck(std::vector<std::variant<int, float, std::string>> const& input) {
        auto pkColumnsIndexes = std::vector<int>();
        auto inputComparingVector = std::vector<std::variant<int, float, std::string>>();
        for (int i = 0; i < col_names.size(); i++) {
            if (columns[col_names[i]]) {
                pkColumnsIndexes.emplace_back(i);
                inputComparingVector.emplace_back(input[i]);
            }
        }

        for (auto const& row : content) {
            auto existingComparingVector = std::vector<std::variant<int, float, std::string>>();
            for (auto const& el : pkColumnsIndexes) {
                existingComparingVector.push_back(row.attributes[el]);
            }
            if (inputComparingVector == existingComparingVector) {
                std::cout << "Syntax Error: duplicate primary keys";
                exit(-1);
            }
        }
    }
public:
    std::string name;
    //vector for keeping the correct order of columns (key set)
    std::vector<std::string> col_names;

    //a map for names of columns and whether a column is a PK
    std::map<std::string, bool> columns;
    //key: columns, values: true (nullable column) or false (not nullable)
    std::map<std::string, bool> nullable;

    //key: columns, values: "i", "f", "s" for integers, floats and strings respectfully
    //data types supported: integer, date, varchar(%), number(%, %) with a decimal point
    std::vector<std::string> dataType;

    //contents
    std::vector<Row> content;

    explicit Table(std::string const& name) {
        this->name = name;
    }

    Table(std::string const& name, std::vector<std::string> const& col_names, std::map<std::string, bool> const& columns,
          std::map<std::string, bool> const& nullable, std::vector<std::string> const& dataType) {
        this->name = name;
        this->col_names = col_names;
        this->columns = columns;
        this->nullable = nullable;
        this->dataType = dataType;
    }

    Table(std::string const& name, std::vector<std::string> const& col_names, std::map<std::string, bool> const& columns,
          std::map<std::string, bool> const& nullable, std::vector<std::string> const& dataType, std::vector<Row> const& content) {
        this->name = name;
        this->col_names = col_names;
        this->columns = columns;
        this->nullable = nullable;
        this->dataType = dataType;
        this->content = content;
    }

    auto addColumn(std::string const& column_name, std::string const& type, bool const& null, bool const& pk) {
        columns[column_name] = pk;
        dataType.emplace_back(type);
        nullable[column_name] = null;
        col_names.push_back(column_name);
    }

    auto addFullRow(std::vector<std::variant<int, float, std::string>> const& input) {//add one Row
        //primary key check
        PKcheck(input);
        content.emplace_back(input, dataType);
    }

    auto addPartialRow(std::vector<std::variant<int, float, std::string>> const& input, std::vector<std::string> const& cols) {
        //primary key check
        PKcheck(input);
        content.emplace_back(input, cols, dataType, col_names);
    }

    auto print() {
        auto length = col_names.size() * 15 + (col_names.size() + 1);
        //top line
        for (int i = 0; i < length; i++) {
            if (i == 0) {
                fmt::print("\n┌");
            } else if (i == length - 1) {
                fmt::print("┐\n");
            } else if (i % 16 == 0) {
                fmt::print("┬");
            } else {
                fmt::print("─");
            }
        }
        fmt::print("│");
        for (auto const& col : col_names) {
            if (columns[col]) {
                fmt::print("{:^15}│", (col + "*"));
            } else {
                fmt::print("{:^15}│", col);
            }
        }
        for (auto const& row : content) {
            //horizontal line
            for (int i = 0; i < length; i++) {
                if (i == 0) {
                    fmt::print("\n├");
                } else if (i == length - 1) {
                    fmt::print("┤\n");
                } else if (i % 16 == 0) {
                    fmt::print("┼");
                } else {
                    fmt::print("─");
                }
            }
            fmt::print("│");
            auto counter = int();
            for (auto const& el : row.attributes) {
                counter++;
                auto index = el.index();
                switch (index) {
                    case 0: {
                        fmt::print("{:15}│", std::get<int>(el));
                        break;
                    }
                    case 1: {
                        fmt::print("{:15}│", std::get<float>(el));
                        break;
                    }
                    default: {
                        fmt::print("{:15}│", std::get<std::string>(el));
                        break;
                    }
                }
            }
        }
        //bottom line
        for (int i = 0; i < length; i++) {
            if (i == 0) {
                fmt::print("\n└");
            } else if (i == length - 1) {
                fmt::print("┘\n");
            } else if (i % 16 == 0) {
                fmt::print("┴");
            } else {
                fmt::print("─");
            }
        }
    }
};


#endif //DATABASE_TABLE_H
