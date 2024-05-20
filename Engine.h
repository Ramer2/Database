#include <bits/stdc++.h>
#include "Patterns.h"
#include "Table.h"
#include "Database.h"

#ifndef DATABASE_ENGINE_H
#define DATABASE_ENGINE_H


#pragma once //TODO: check this


//syntax check, ast and completion of the code
class Engine {
private:
    std::vector<std::string> ast; //abstract syntax tree (strictly the order)
    std::map<std::string, std::vector<std::string>> parameters;

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
            parameterVector.push_back(input[from]);
            from++;
        }

        return parameterVector;
    }
public:
    auto codeRetrieval(std::vector<std::string>& input) {

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

        } else {
            std::cout << "Syntax Error: Keyword expected";
            exit(-1);
        }

        //output
//        for(const auto& el : parameters) {
//            int counter = 0;
//            std::cout << (el.first + ":") << std::endl;
//            for (const auto& inst : el.second) {
//                std::cout << counter << ". " + inst << std::endl;
//                counter++;
//            }
//        }

        //ast output
//        for (auto el : ast) {
//            std::cout << el << std::endl;
//        }
    }

    //const std::map<std::string, std::vector<std::string>>& parameters, const std::vector<std::string>& ast
    auto completer() {

        auto cmd_iterator = 0;
        if (ast[cmd_iterator] == "CREATE_TABLE") {
            cmd_iterator++;

            //respective parameters output
//            int counter = 0;
//            for (auto const& el : parameters["CREATE_TABLE"]) {
//                std::cout << counter++ << ". " << el << std::endl;
//            }

            auto iterator = 0;
            auto tableName = parameters["CREATE_TABLE"][iterator++];
            auto newTable = Table(tableName);

            std::map<std::string, bool> columns;
            std::map<std::string, std::string> dataType;
            std::map<std::string, bool> nullable;

            while (iterator < parameters["CREATE_TABLE"].size() - 1) {
                if (!(parameters["CREATE_TABLE"][iterator] == "," or parameters["CREATE_TABLE"][iterator] == "(" or parameters["CREATE_TABLE"][iterator] == ")")) {

                    //TODO: redo this sheiet (maybe try with functions)
                    std::string column_name = parameters["CREATE_TABLE"][iterator];
                    iterator++;

                    std::string type;
                    if (std::regex_match(parameters["CREATE_TABLE"][iterator], Patterns::integer)) {
                        type = "i";
                        iterator++;
                    } else if (std::regex_match(parameters["CREATE_TABLE"][iterator], Patterns::varchar)) {
                        type = "v" + parameters["CREATE_TABLE"][iterator + 2];
                        iterator += 4;
                    } else if (std::regex_match(parameters["CREATE_TABLE"][iterator], Patterns::number)) {
                        type = "n" + parameters["CREATE_TABLE"][iterator + 2] + "/" + parameters["CREATE_TABLE"][iterator + 3];
                        iterator += 5;
                    } else if (std::regex_match(parameters["CREATE_TABLE"][iterator], Patterns::date)) {
                        type = "d";
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

                    //TODO: fix this sheiet
                    newTable.addColumn(column_name, type, null, pk);
                } else iterator++;
            }
            Database::database.push_back(newTable);
        }

//        CREATE TABLE employees (
//                id            INTEGER         PRIMARY KEY,
//                first_name    NUMBER(20, 2)   not null,
//                last_name     VARCHAR(75)     not null,
//                mid_name      VARCHAR(50)     not null,
//                dateofbirth   DATE            not null
//        );


//        if (ast[cmd_iterator] == "SELECT") {
//            cmd_iterator++;
//
//
//            if (ast[cmd_iterator] == "INFO") {
//                cmd_iterator++;
//
//            }
//
//            if (ast[cmd_iterator] == "FROM") {
//                cmd_iterator++;
//
//            }
//        } else if (ast[cmd_iterator] == "INSERT_INTO") {
//            cmd_iterator++;
//
//        }
    }
};


#endif //DATABASE_ENGINE_H
