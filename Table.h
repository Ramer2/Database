
#ifndef DATABASE_TABLE_H
#define DATABASE_TABLE_H

#include "Row.h"
#include <vector>
#include <string>


class Table {
    protected:
        std::vector<std::string> columns;
        std::vector<std::string> primaryKeys;
//        std::vector<Row>         contents;
    public:
        Table(std::vector<std::string>& cols, std::vector<std::string>& pks) {
            columns = cols;
            primaryKeys = pks;
        }
        Table()= default;
};


#endif //DATABASE_TABLE_H
