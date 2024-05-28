#ifndef DATABASE_TABLE_H
#define DATABASE_TABLE_H

#include "Row.h"
#include <map>
#include <fmt/core.h>
#include <fmt/ranges.h>


class Table {
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

    auto addColumn(std::string const& column_name, std::string const& type, bool const& null, bool const& pk) {
        columns[column_name] = pk;
        dataType.emplace_back(type);
        nullable[column_name] = null;
        col_names.push_back(column_name);
    }

    auto addFullRow(std::vector<std::variant<int, float, std::string>> const& input) {//add one Row
        content.emplace_back(input, dataType);
    }

    auto addPartialRow(std::vector<std::variant<int, float, std::string>> const& input, std::vector<std::string> const& cols) {
        content.emplace_back(input, cols, dataType, col_names);
    }

    //TODO: print function
    auto print() {

    }
};


#endif //DATABASE_TABLE_H
