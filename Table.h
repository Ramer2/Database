
#ifndef DATABASE_TABLE_H
#define DATABASE_TABLE_H

#include "Row.h"
#include <vector>
#include <string>
#include <map>


class Table {
public:
    std::string name;
    //a map for names of columns and whether a column is a PK
    std::map<std::string, bool> columns;
    //key: columns, values: data types (data types supported: integer, date, varchar(%), number(%, %) with a decimal point . )
    std::map<std::string, std::string> dataType;
    //key: columns, values: true (nullable column) or false (not nullable)
    std::map<std::string, bool> nullable;
    //contents
    std::vector<Row> content;

    explicit Table(std::string const& name) {
        this->name = name;
    }

    //vector contents consists of vectors, which are rows (all the data is stored in std::string type and now will be converted)
    //it is ensured, that the data inside the vector rows is in correct order
    auto contentFiller(std::vector<std::vector<std::string>> const& contents) {
        for (auto el : contents) {
            content.emplace_back(el);
        }
    }

    auto addColumn(std::string const& column_name, std::string const& type, bool const& null, bool const& pk) {
        columns[column_name] = pk;
        dataType[column_name] = type;
        nullable[column_name] = null;
    }
};


#endif //DATABASE_TABLE_H
