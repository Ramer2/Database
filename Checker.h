#include <bits/stdc++.h>
#include "Patterns.h"

#ifndef DATABASE_CHECKER_H
#define DATABASE_CHECKER_H


#pragma once //TODO: check this


class Checker {
private:
    std::vector<std::string> ast;
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

    static auto whereParameterExtraction(std::vector<std::string>& input, int& from) {
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
    auto syntaxCheck(std::vector<std::string>& input) {
        //TODO: do all of these as functions (?)
        //select
        if (std::regex_match(input[0], Patterns::select)){
            int iterator = 1;
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
                    parameters["WHERE"] = whereParameterExtraction(input, iterator);
                }

            } else {
                std::cout << R"(Syntax Error: "FROM" or "INTO" expected)";
                return;
            }
        }// else if()
        else {
            std::cout << "Syntax Error: Keyword expected";
            exit(-1);
        }

        //output
        for(const auto& el : parameters) {
            int counter = 0;
            std::cout << (el.first + ":") << std::endl;
            for (const auto& inst : el.second) {
                std::cout << counter << ". " + inst << std::endl;
                counter++;
            }
        }
    }
};


#endif //DATABASE_CHECKER_H
