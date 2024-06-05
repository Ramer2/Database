#include "Patterns.h"
#include "Table.h"
#include "Database.h"

#ifndef DATABASE_ENGINE_H
#define DATABASE_ENGINE_H


//syntax check, ast and completion of the code
class Engine {
private:
    static std::vector<std::string> ast; //abstract syntax tree (strictly the order)
    static std::map<std::string, std::vector<std::string>> parameters;

    static auto toLowerCase(std::string const& str) {
        auto newString = std::string();
        for (auto& letter : str) {
            newString += std::tolower(letter);
        }
        return newString;
    }

    static auto isKeyword(std::string& word) {
        return (std::regex_match(word, Patterns::select) or
                std::regex_match(word, Patterns::update) or
                std::regex_match(word, Patterns::truncate) or
                std::regex_match(word, Patterns::alter) or
                std::regex_match(word, Patterns::drop) or
                std::regex_match(word, Patterns::create) or
                std::regex_match(word, Patterns::del) or
                std::regex_match(word, Patterns::insert) or
                std::regex_match(word, Patterns::values) or
                std::regex_match(word, Patterns::from) or
                std::regex_match(word, Patterns::into) or
                std::regex_match(word, Patterns::or_) or
                std::regex_match(word, Patterns::and_) or
                std::regex_match(word, Patterns::not_) or
                std::regex_match(word, Patterns::where));
    }

    static auto isLogic(std::string& word) {
        return (std::regex_match(word, Patterns::and_) or
                std::regex_match(word, Patterns::or_) or
                std::regex_match(word, Patterns::not_));
    }

    //this method is used for WHERE and CREATE TABLE keywords
    static auto specialParameterExtraction(std::vector<std::string>& input, int& from) {
        auto parameterVector = std::vector<std::string>();

        if (isKeyword(input[from]) && !isLogic(input[from])) {
            std::cout << "Syntax Error: found a keyword, parameter expected";
            //exit the program
            exit(-1);
        }
        while (from < input.size() and !(isKeyword(input[from]) && !isLogic(input[from]))) {
            if (input[from] == "," or input[from] == ";") {
                from++;
                continue;
            }
            parameterVector.push_back(input[from]);
            from++;
        }
        return parameterVector;
    }

    static auto parameterExtraction(std::vector<std::string>& input, int& from) {
        auto parameterVector = std::vector<std::string>();
        if (isKeyword(input[from])) {
            std::cout << "Syntax Error: found a keyword, parameter expected";
            //exit the program
            exit(-1);
        }
        while (from < input.size() and !isKeyword(input[from])) {
            if (input[from] == "," or input[from] == ";") {
                from++;
                continue;
            }
            if (!input[from].empty()) parameterVector.push_back(input[from]);
            from++;
        }
        return parameterVector;
    }

    static auto conditionCheck (std::vector<std::string> const& dataType, int const& index, std::string const& logicOperator,
                                bool const& negated, std::string const& parameter, Row const& row) {
        std::cout << index << std::endl;
        if (dataType[index] == "i") {
            auto val1 = std::get<int>(row.attributes[index]);
            auto val2 = std::stoi(parameter);

            if (!negated) {
                if ((logicOperator == "=" and val1 == val2) or
                    (logicOperator == "!=" and val1 != val2) or
                    (logicOperator == ">" and val1 > val2) or
                    (logicOperator == "<" and val1 < val2) or
                    (logicOperator == ">=" and val1 >= val2) or
                    (logicOperator == "<=" and val1 <= val2)) return true;
            } else {
                if (!((logicOperator == "=" and val1 == val2) or
                      (logicOperator == "!=" and val1 != val2) or
                      (logicOperator == ">" and val1 > val2) or
                      (logicOperator == "<" and val1 < val2) or
                      (logicOperator == ">=" and val1 >= val2) or
                      (logicOperator == "<=" and val1 <= val2))) return true;
            }
        } else if (dataType[index] == "f") {
            auto val1 = std::get<float>(row.attributes[index]);
            auto val2 = std::stof(parameter);
            if (!negated) {
                if ((logicOperator == "=" and val1 == val2) or
                    (logicOperator == "!=" and val1 != val2) or
                    (logicOperator == ">" and val1 > val2) or
                    (logicOperator == "<" and val1 < val2) or
                    (logicOperator == ">=" and val1 >= val2) or
                    (logicOperator == "<=" and val1 <= val2)) return true;
            } else {
                if (!((logicOperator == "=" and val1 == val2) or
                      (logicOperator == "!=" and val1 != val2) or
                      (logicOperator == ">" and val1 > val2) or
                      (logicOperator == "<" and val1 < val2) or
                      (logicOperator == ">=" and val1 >= val2) or
                      (logicOperator == "<=" and val1 <= val2))) return true;
            }
        } else {
            auto val1 = std::get<std::string>(row.attributes[index]);
            auto val2 = parameter;
            val1 = toLowerCase(val1);
            val2 = toLowerCase(val2);
            if (!negated) {
                if ((logicOperator == "=" and val1 == val2) or
                    (logicOperator == "!=" and val1 != val2) or
                    (logicOperator == ">" and val1 > val2) or
                    (logicOperator == "<" and val1 < val2) or
                    (logicOperator == ">=" and val1 >= val2) or
                    (logicOperator == "<=" and val1 <= val2)) return true;
            } else {
                if (!((logicOperator == "=" and val1 == val2) or
                      (logicOperator == "!=" and val1 != val2) or
                      (logicOperator == ">" and val1 > val2) or
                      (logicOperator == "<" and val1 < val2) or
                      (logicOperator == ">=" and val1 >= val2) or
                      (logicOperator == "<=" and val1 <= val2))) return true;
            }
        }
        return false;
    }

public:
    static auto codeRetrieval(std::vector<std::string>& input) {

        //select
        if (std::regex_match(input[0], Patterns::select)){
            auto iterator = 1;
            ast.emplace_back("SELECT");
            parameters["SELECT"] = parameterExtraction(input, iterator);

            //from or into
            if (std::regex_match(input[iterator], Patterns::into)) {
                iterator++;
                ast.emplace_back("INTO");
                parameters["INTO"] = parameterExtraction(input, iterator);
            }
            if (std::regex_match(input[iterator], Patterns::from)) {
                iterator++;
                ast.emplace_back("FROM");
                parameters["FROM"] = parameterExtraction(input, iterator);

                if (iterator == input.size()) return;

                //where
                if (std::regex_match(input[iterator], Patterns::where)) {
                    iterator++;
                    ast.emplace_back("WHERE");
                    parameters["WHERE"] = specialParameterExtraction(input, iterator);
                }
            } else {
                std::cout << R"(Syntax Error: "FROM" or "INTO" expected)";
                exit(-1);
            }
        } else if(std::regex_match(input[0], Patterns::insert)) {
            auto iterator = 1;
            //insert into
            if (!std::regex_match(input[iterator], Patterns::into)) {
                std::cout << R"(Syntax Error: "INTO" expected)";
                exit(-1);
            }
            iterator++;
            ast.emplace_back("INSERT_INTO");
            parameters["INSERT_INTO"] = parameterExtraction(input, iterator);

            int col_counter = 0;
            for (int i = 2; i < parameters["INSERT_INTO"].size(); i++)
                if (parameters["INSERT_INTO"][i] != ")"
                    and parameters["INSERT_INTO"][i] != ","
                    and parameters["INSERT_INTO"][i] != "(") col_counter++;

            if (!std::regex_match(input[iterator], Patterns::values)) {
                std::cout << R"(Syntax Error: "VALUES" expected)";
                exit(-1);
            }
            iterator++;
            ast.emplace_back("VALUES");

            parameters["VALUES"] = parameterExtraction(input, iterator);

            int val_counter = 0;
            for (int i = 0; i < parameters["VALUES"].size(); i++)
                if (parameters["VALUES"][i] != ")"
                    and parameters["VALUES"][i] != ","
                    and parameters["VALUES"][i] != "(") val_counter++;

            if (val_counter % col_counter != 0) {
                std::cout << R"(Syntax Error: different number of given values and columns in the table)";
                exit(-1);
            }
        } else if(std::regex_match(input[0], Patterns::create)) {
            auto iterator = 1;
            if (!std::regex_match(input[iterator], Patterns::table)) {
                std::cout << R"(Syntax Error: "TABLE" expected)";
                exit(-1);
            }

            iterator++;
            ast.emplace_back("CREATE_TABLE");
            parameters["CREATE_TABLE"] = specialParameterExtraction(input, iterator);

        } else if (std::regex_match(input[0], Patterns::drop)) {
            if (std::regex_match(input[1], Patterns::table)) {
                ast.emplace_back("DROP_TABLE");
                parameters["DROP_TABLE"].emplace_back(input[2]);
            } else {
                std::cout << "Syntax Error: \"TABLE\" keyword expected";
                exit(-1);
            }
        } else if (std::regex_match(input[0], Patterns::truncate)) {
            if (std::regex_match(input[1], Patterns::table)) {
                ast.emplace_back("TRUNCATE_TABLE");
                parameters["TRUNCATE_TABLE"].emplace_back(input[2]);
            } else {
                std::cout << "Syntax Error: \"TABLE\" keyword expected";
                exit(-1);
            }
        } else {
            std::cout << "Syntax Error: Keyword expected";
            exit(-1);
        }
    }

    static auto completer() {
        auto cmd_iterator = 0;
        if (ast[cmd_iterator] == "CREATE_TABLE") {

            auto iterator = 0;
            auto tableName = parameters["CREATE_TABLE"][iterator++];
            auto newTable = Table(tableName);

            while (iterator < parameters["CREATE_TABLE"].size() - 1) {
                if (!(parameters["CREATE_TABLE"][iterator] == "," or parameters["CREATE_TABLE"][iterator] == "(" or parameters["CREATE_TABLE"][iterator] == ")")) {
                    std::string column_name = parameters["CREATE_TABLE"][iterator];
                    iterator++;

                    //i - integer, f - float, s - string
                    std::string type;
                    if (std::regex_match(parameters["CREATE_TABLE"][iterator], Patterns::integer)) {
                        type = "i";
                        iterator++;
                    } else if (std::regex_match(parameters["CREATE_TABLE"][iterator], Patterns::varchar)) {
                        type = "s";
                        iterator += 4;
                    } else if (std::regex_match(parameters["CREATE_TABLE"][iterator], Patterns::number)) {
                        type = "f";
                        iterator += 5;
                    } else if (std::regex_match(parameters["CREATE_TABLE"][iterator], Patterns::date)) {
                        type = "s";
                        iterator++;
                    } else {
                        std::cout << "Syntax Error: unknown data type";
                        exit(-1);
                    }

                    bool null;
                    bool pk;
                    bool fk = false;
                    auto reference = std::string();
                    if (std::regex_match(parameters["CREATE_TABLE"][iterator], Patterns::primary)) {
                        iterator++;
                        if (std::regex_match(parameters["CREATE_TABLE"][iterator], Patterns::key)) {
                            iterator++;
                            pk = true;
                            null = false;
                        } else {
                            std::cout << "Syntax Error: \"KEY\" keyword is missing";
                            exit(-1);
                        }
                    } else if (std::regex_match(parameters["CREATE_TABLE"][iterator], Patterns::foreign)) {
                        iterator++;
                        if (std::regex_match(parameters["CREATE_TABLE"][iterator], Patterns::key)) {
                            iterator++;
                            if (std::regex_match(parameters["CREATE_TABLE"][iterator], Patterns::references)) {
                                iterator++;
                                auto refTable = parameters["CREATE_TABLE"][iterator];
                                auto refCol = parameters["CREATE_TABLE"][iterator + 2];
                                reference = refTable + "." + refCol;
                                iterator += 3;
                                pk = false;
                                fk = true;
                                null = false; //limitation of this project: fk cannot be null

                                bool foundTable = false;
                                for (auto const& table : Database::database) {
                                    if (table.name == refTable) {
                                        foundTable = true;
                                        bool found = false;
                                        for (auto const& col : table.col_names) {
                                            if (col == refCol) {
                                                found = true;
                                                break;
                                            }
                                        }
                                        if (!found) {
                                            std::cout << "Syntax Error: reference column wasn't found";
                                            exit(-1);
                                        }
                                    }
                                }
                                if (!foundTable) {
                                    std::cout << "Syntax Error: reference table wasn't found";
                                    exit(-1);
                                }

                            } else {
                                std::cout << "Syntax Error: \"REFERENCES\" keyword is expected";
                                exit(-1);
                            }
                        } else {
                            std::cout << "Syntax Error: \"KEY\" keyword is missing";
                            exit(-1);
                        }
                    } else if (std::regex_match(parameters["CREATE_TABLE"][iterator], Patterns::null)) {
                        iterator++;
                        null = true;
                    } else if (std::regex_match(parameters["CREATE_TABLE"][iterator], Patterns::not_)) {
                        iterator++;
                        if (std::regex_match(parameters["CREATE_TABLE"][iterator], Patterns::null)) {
                            iterator++;
                            null = false;
                            pk = false;
                        } else {
                            std::cout << "Syntax Error: null expected";
                            exit(-1);
                        }
                    } else {
                        std::cout << "Syntax Error: null or primary key expected";
                        exit(-1);
                    }

                    if (fk) {
                        newTable.addReferenceColumn(column_name, type, reference);
                    } else {
                        newTable.addColumn(column_name, type, null, pk);
                    }
                } else iterator++;
            }
            Database::database.push_back(newTable);
        } else if (ast[cmd_iterator] == "SELECT") {
            cmd_iterator++;

            if (ast[cmd_iterator] == "FROM") {
                cmd_iterator++;

                //creating temporary table to print
                Table outputTable = Table("temp");
                auto tmpCol_Names = std::vector<std::string>();
                auto tmpColumns = std::map<std::string, bool>();
                auto tmpNullable = std::map<std::string, bool>();
                auto tmpDataType = std::vector<std::string>();
                auto tmpContent = std::vector<Row>();

                auto reqTables = std::vector<Table>();
                for (auto const& name : parameters["FROM"]) {
                    bool found = false;
                    for (auto const& table : Database::database) {
                        if (name == table.name) {
                            reqTables.push_back(table);
                            found = true;
                        }
                    }
                    if (!found) {
                        std::cout << "Syntax Error: unknown table " << name;
                        exit(-1);
                    }
                }

                if (parameters["SELECT"][0] == "*") {
                    if (reqTables.size() == 1) {
                        reqTables[0].print();
                    } else {
                        for (auto const& table : reqTables) {
                            tmpCol_Names.insert(tmpCol_Names.end(),table.col_names.begin(), table.col_names.end());
                            tmpColumns.insert(table.columns.begin(), table.columns.end());
                            tmpNullable.insert(table.nullable.begin(), table.nullable.end());
                            tmpDataType.insert(tmpDataType.end(), table.dataType.begin(), table.dataType.end());

                            //content combination
                            if (tmpContent.empty()) {
                                tmpContent.insert(tmpContent.end(), table.content.begin(), table.content.end());
                            } else {
                                auto prevContent = tmpContent;
                                tmpContent.clear();
                                for (auto const& prevRow : prevContent) {
                                    for (auto const& row : table.content) {
                                        auto copyPrevRow = prevRow;
                                        copyPrevRow.attributes.insert(copyPrevRow.attributes.end(), row.attributes.begin(), row.attributes.end());
                                        tmpContent.emplace_back(copyPrevRow);
                                    }
                                }
                            }
                        }
                        outputTable.col_names = tmpCol_Names;
                        outputTable.columns = tmpColumns;
                        outputTable.nullable = tmpNullable;
                        outputTable.dataType = tmpDataType;
                        outputTable.content = tmpContent;
                    }
                } else {
                    //check for column availability among tables
                    for (auto const& checkCol : parameters["SELECT"]) {
                        bool found = false;
                        for (auto const& table : reqTables) {
                            for (auto const& col : table.col_names) {
                                if (checkCol == col) {
                                    found = true;
                                    tmpCol_Names.emplace_back(checkCol);
                                }
                            }
                        }
                        if (!found) {
                            std::cout << "Syntax Error: column " << checkCol << " was not found among listed tables";
                            exit(-1);
                        }
                    }

                    for (auto const& table : reqTables) {
                        for (auto const& tmpCol : tmpCol_Names) {
                            for (auto const& col : table.columns) {
                                if (col.first == tmpCol) {
                                    tmpColumns[tmpCol] = col.second;
                                }
                            }
                        }

                        for (auto const& tmpCol : tmpCol_Names) {
                            for (auto const& col : table.nullable) {
                                if (col.first == tmpCol) {
                                    tmpNullable[tmpCol] = col.second;
                                }
                            }
                        }

                        for (auto const& tmpCol : tmpCol_Names) {
                            int pos = 0;
                            while (pos < table.col_names.size()) {
                                if (tmpCol == table.col_names[pos]) break;
                                pos++;
                            }
                            tmpDataType.emplace_back(table.dataType[pos]);
                        }

                        auto indexVector = std::vector<int>();
                        for (auto const& tmpCol : tmpCol_Names) {
                            for (int i = 0; i < table.col_names.size(); i++) {
                                if (table.col_names[i] == tmpCol) indexVector.emplace_back(i);
                            }
                        }

                        //solution, provided by ChatGPT on "c++: how to get rid of duplicates in the vector of ints"
                        //omg, this fix worked. I thought I'll have to rewrite this whole loop
                        auto last = std::unique(indexVector.begin(), indexVector.end());
                        indexVector.erase(last, indexVector.end());

                        if (tmpContent.empty()) {
                            for (auto const& row : table.content) {
                                auto tmpAttributes = std::vector<std::variant<int, float, std::string>>();
                                for (auto index : indexVector) {
                                    tmpAttributes.emplace_back(row.attributes[index]);
                                }
                                tmpContent.emplace_back(tmpAttributes);
                            }
                        } else {
                            auto prevContent = tmpContent;
                            tmpContent.clear();
                            for (auto const& prevRow : prevContent) {
                                for (auto const& row : table.content) {
                                    auto prevRowAttributes = prevRow.attributes;
                                    auto tmpAttributes = std::vector<std::variant<int, float, std::string>>();
                                    for (auto index : indexVector) {
                                        prevRowAttributes.emplace_back(row.attributes[index]);
                                    }
                                    tmpContent.emplace_back(prevRowAttributes);
                                }
                            }
                        }
                    }

                    outputTable.col_names = tmpCol_Names;
                    outputTable.columns = tmpColumns;
                    outputTable.nullable = tmpNullable;
                    outputTable.dataType = tmpDataType;
                    outputTable.content = tmpContent;
                }

                if (cmd_iterator == ast.size()) {
                    outputTable.print();
                    return;
                } else {
                    if (!std::regex_match(ast[cmd_iterator], Patterns::where)) {
                        std::cout << "Syntax Error: unknown keyword" << ast[cmd_iterator];
                        exit(-1);
                    }

                    auto iterator = 0;
                    if (parameters["WHERE"].size() > 4) {
                        auto mainOperator = std::string();

                        bool negated1 = false;
                        if (std::regex_match(parameters["WHERE"][iterator], Patterns::not_)) {
                            negated1 = true;
                            iterator++;
                        }

                        auto reqCol1 = parameters["WHERE"][iterator++];
                        auto logicOperator1 = parameters["WHERE"][iterator++];
                        auto parameter1 = parameters["WHERE"][iterator++];

                        if (!(logicOperator1 == "=" or logicOperator1 == "!=" or logicOperator1 == "<"
                              or logicOperator1 == ">" or logicOperator1 == ">=" or logicOperator1 == "<=")) {
                            std::cout << "Syntax Error: unknown operator " << logicOperator1;
                            exit(-1);
                        }

                        auto index1 = int();
                        for (int i = 0; i < outputTable.col_names.size(); i++) {
                            if (outputTable.col_names[i] == reqCol1) {
                                index1 = i;
                                break;
                            }
                        }

                        mainOperator = parameters["WHERE"][iterator++];

                        bool negated2 = false;
                        if (std::regex_match(parameters["WHERE"][iterator], Patterns::not_)) {
                            negated1 = true;
                            iterator++;
                        }

                        auto reqCol2 = parameters["WHERE"][iterator++];
                        auto logicOperator2 = parameters["WHERE"][iterator++];
                        auto parameter2 = parameters["WHERE"][iterator++];

                        if (!(logicOperator1 == "=" or logicOperator1 == "!=" or logicOperator1 == "<"
                              or logicOperator1 == ">" or logicOperator1 == ">=" or logicOperator1 == "<=")) {
                            std::cout << "Syntax Error: unknown operator " << logicOperator1;
                            exit(-1);
                        }

                        auto index2 = int();
                        for (int i = 0; i < outputTable.col_names.size(); i++) {
                            if (outputTable.col_names[i] == reqCol2) {
                                index2 = i;
                                break;
                            }
                        }

                        tmpContent = outputTable.content;
                        outputTable.content.clear();

                        if (std::regex_match(mainOperator, Patterns::and_)) {
                            for (auto const& row : tmpContent) {
                                if (conditionCheck(outputTable.dataType, index1, logicOperator1, negated1, parameter1, row) and
                                    conditionCheck(outputTable.dataType, index2, logicOperator2, negated2, parameter2, row)) {
                                    outputTable.content.emplace_back(row);
                                }
                            }
                        } else if (std::regex_match(mainOperator, Patterns::or_)) {
                            for (auto const& row : tmpContent) {
                                if (conditionCheck(outputTable.dataType, index1, logicOperator1, negated1, parameter1, row) or
                                    conditionCheck(outputTable.dataType, index2, logicOperator2, negated2, parameter2, row)) {
                                    outputTable.content.emplace_back(row);
                                }
                            }
                        }

                    } else {
                        bool negated = false;
                        if (std::regex_match(parameters["WHERE"][iterator], Patterns::not_)) {
                            negated = true;
                            iterator++;
                        }

                        auto reqCol = parameters["WHERE"][iterator++];
                        auto logicOperator = parameters["WHERE"][iterator++];
                        auto parameter = parameters["WHERE"][iterator++];

                        if (!(logicOperator == "=" or logicOperator == "!=" or logicOperator == "<"
                            or logicOperator == ">" or logicOperator == ">=" or logicOperator == "<=")) {
                            std::cout << "Syntax Error: unknown operator " << logicOperator;
                            exit(-1);
                        }

                        auto index = int();
                        for (int i = 0; i < outputTable.col_names.size(); i++) {
                            if (outputTable.col_names[i] == reqCol) {
                                index = i;
                                break;
                            }
                        }

                        tmpContent = outputTable.content;
                        outputTable.content.clear();

                        for (auto const& row : tmpContent) {
                            if (conditionCheck(outputTable.dataType, index, logicOperator, negated, parameter, row)) {
                                outputTable.content.emplace_back(row);
                            }
                        }
                    }
                    outputTable.print();
                }
            }
        } else if (ast[cmd_iterator] == "INSERT_INTO") {

            auto iterator = 0;
            std::string table_name = parameters["INSERT_INTO"][iterator++];

            Table* thisTable;
            for(auto& table : Database::database) {
                if (table.name == table_name) thisTable = &table;
            }

            auto col_num = thisTable->col_names.size();

            //check for number of columns
            if ((parameters["INSERT_INTO"].size() - 3) > col_num) {// -3 cuz of the brackets at the beginning and at the end and the table name at the beginning
                std::cout << "Syntax Error: number of columns at the input is larger than in the table";
                exit(-1);
            }

            auto col_names = std::vector<std::string>();
            iterator++;
            while (iterator < parameters["INSERT_INTO"].size() - 1) {
                col_names.emplace_back(parameters["INSERT_INTO"][iterator]);
                iterator++;
            }

            //check for columns
            if (!(col_names == thisTable->col_names)) {
                //advanced comparing
                bool isAmongParams = false;
                for (auto const& col : thisTable->col_names) {
                    for (auto const& param : col_names) {
                        if (col == param) isAmongParams = true;
                    }
                    if (!isAmongParams and !thisTable->nullable[col]) {
                        std::cout << "Syntax error: unknown column or not defined non-nullable column";
                        exit(-1);
                    }
                    isAmongParams = false;
                }
            }

            auto foreignKeyPos = std::vector<int>();
            for (int i = 0; i < col_names.size(); i++) {
                if (thisTable->fks[col_names[i]]) {
                    foreignKeyPos.emplace_back(i);
                }
            }

            auto place = 0;
            auto row = std::vector<std::variant<int, float, std::string>>();
            for (auto const& el : parameters["VALUES"]) {
                if (el == "(") {
                    continue;
                } else if (el == ")") {
                    place = 0;
                    thisTable->addPartialRow(row, col_names);
                    row.clear();
                } else {
                    auto thisColPlace = int();
                    for (int i = 0; i < thisTable->col_names.size(); i++) {
                        if (col_names[place] == thisTable->col_names[i]) {
                            thisColPlace = i;
                            break;
                        }
                    }
                    auto type = thisTable->dataType[thisColPlace];

                    //foreign values check
                    for (auto const& pos : foreignKeyPos) {
                        //checking, whether currently we are dealing with a reference column
                        if (pos == place) {
                            auto referenceTable = std::string();
                            auto referenceColumn = std::string();
                            auto reference = thisTable->references[col_names[place]];
                            bool dot = false;
                            for (char i : reference) {
                                if (i == '.') {
                                    dot = true;
                                } else if (!dot) {
                                    referenceTable += i;
                                } else {
                                    referenceColumn += i;
                                }
                            }
                            Table* refTable;
                            int reqColPos = 0;
                            for (auto& table : Database::database) {
                                if (table.name == referenceTable) {
                                    refTable = &table;
                                    break;
                                }
                            }
                            for (int i = 0; i < refTable->col_names.size(); i++) {
                                if (refTable->col_names[i] == referenceColumn) {
                                    reqColPos = i;
                                    break;
                                }
                            }

                            bool found = false;
                            for (auto const& rowCheck : refTable->content) {
                                if (type == "i") {
                                    auto val = std::stoi(el);
                                    if (std::get<int>(rowCheck.attributes[reqColPos]) == val) {
                                        found = true;
                                    }
                                } else if (type == "f") {
                                    auto val = std::stof(el);
                                    if (std::get<float>(rowCheck.attributes[reqColPos]) == val) {
                                        found = true;
                                    }
                                } else {
                                    auto val = el;
                                    if (std::get<std::string>(rowCheck.attributes[reqColPos]) == val) {
                                        found = true;
                                    }
                                }
                            }
                            if (!found) {
                                std::cout << "Syntax error: this reference value does not exist.";
                                exit(-1);
                            }
                        }
                    }

                    //I hope that I'm dealing with a responsible user who will not put something like "3.14afsdga" for a >
                    // > float column (stof and stoi don't crash, the information is simply lost)
                    if (type == "i") {
                        row.emplace_back(std::stoi(el));
                    } else if (type == "f") {
                        row.emplace_back(std::stof(el));
                    } else {
                        row.emplace_back(el);
                    }
                    place++;
                }
            }

        } else if (ast[cmd_iterator] == "DROP_TABLE") {
            auto iterator = Database::database.begin();
            while (iterator != Database::database.end()){
                if ((*iterator).name == parameters["DROP_TABLE"][0]) {
                    Database::database.erase(iterator);
                    return;
                }
                iterator++;
            }
            std::cout << "Syntax Error: table " << parameters["DROP_TABLE"][0] << " wasn't found";
            exit(-1);
        } else if (ast[cmd_iterator] == "TRUNCATE_TABLE") {
            auto table_name = parameters["TRUNCATE_TABLE"][0];
            for (auto& table : Database::database) {
                if (table.name == table_name) {
                    table.content.clear();
                    break;
                }
            }
        }
    }
};


#endif //DATABASE_ENGINE_H
