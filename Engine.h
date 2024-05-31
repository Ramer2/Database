#include "Patterns.h"
#include "Table.h"
#include "Database.h"

#ifndef DATABASE_ENGINE_H
#define DATABASE_ENGINE_H


#pragma once //TODO: check this


//syntax check, ast and completion of the code
class Engine {
private:
    static std::vector<std::string> ast; //abstract syntax tree (strictly the order)
    static std::map<std::string, std::vector<std::string>> parameters;

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
                std::cout << "Syntax Error: \"table\" keyword expected";
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

                    newTable.addColumn(column_name, type, null, pk);
                } else iterator++;
            }
            Database::database.push_back(newTable);
        } else if (ast[cmd_iterator] == "SELECT") {
            cmd_iterator++;
//            fmt::println("{}", parameters["SELECT"]);

            if (ast[cmd_iterator] == "INTO") {
                cmd_iterator++;
                std::cout << "not implemented";
            }

            if (ast[cmd_iterator] == "FROM") {
                cmd_iterator++;
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
                        //creating temporary table to print
                        auto tmpCol_Names = std::vector<std::string>();
                        auto tmpColumns = std::map<std::string, bool>();
                        auto tmpNullable = std::map<std::string, bool>();
                        auto tmpDataType = std::vector<std::string>();
                        auto tmpContent = std::vector<Row>();

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
                        Table("tmp", tmpCol_Names, tmpColumns, tmpNullable, tmpDataType, tmpContent).print();
                    }
                }
            }
        } else if (ast[cmd_iterator] == "INSERT_INTO") {
            cmd_iterator++;

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
                    //doesnt work for some reason
                    for (int i = 0; i < thisTable->col_names.size(); i++) {
                        if (col_names[place] == thisTable->col_names[i]) {
                            thisColPlace = i;
                            break;
                        }
                    }
                    auto type = thisTable->dataType[thisColPlace];

                    //I hope that I'm dealing with a responsible user who will not put something like "3.14afsdga" for a
                    // float column (stof and stoi don't crash, the information is simply lost)
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
            cmd_iterator++;

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
        }
    }
};


#endif //DATABASE_ENGINE_H
