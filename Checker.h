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
        if (std::regex_match(word, Patterns::select) or
            std::regex_match(word, Patterns::update) or
            std::regex_match(word, Patterns::truncate) or
            std::regex_match(word, Patterns::alter) or
            std::regex_match(word, Patterns::drop) or
            std::regex_match(word, Patterns::create) or
            std::regex_match(word, Patterns::del) or
            std::regex_match(word, Patterns::insert) or
            std::regex_match(word, Patterns::from) or
            std::regex_match(word, Patterns::into) or
            std::regex_match(word, Patterns::where)) {
            return true;
        }
        else return false;
    }

    static auto parameterExtraction(std::vector<std::string>& input, int& from) {
        auto parameterVector = std::vector<std::string>();

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
        //TODO: do all of these as functions
        //select
        if (std::regex_match(input[0], Patterns::select)){
            int iterator = 1;
            ast.emplace_back("SELECT");

            if (isKeyword(input[iterator])) {//TODO: put this into parameter extractor
                std::cout << "Syntax Error: " << (iterator + 1) << " word is a keyword, parameter expected";
                return;
            }

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

            } else {
                std::cout << R"(Syntax Error: "FROM" or "INTO" expected)";
                return;
            }


        }// else if()

        //output
        for(const auto& el : parameters) {
            int counter = 0;
            std::cout << (el.first + ":") << std::endl;
            for (const auto& inst : el.second) {
                std::cout << counter << ". " + inst << std::endl;
                counter++;
            }
        }


        /*if (std::regex_match(input[0], Patterns::select)){
            std::cout << "select";
            ast.emplace_back("SELECT");
            if (isKeyword(input[1])) return "Syntax Error: 2nd word is a keyword, parameter expected";
            int iterator = 1;
            while (!isKeyword(input[iterator])) {
                parameters["SELECT"].push_back(input[iterator]);
                iterator++;
            }
        } else if (std::regex_match(*input.begin(), Patterns::create)) {
            std::cout << "create";
        } else if (std::regex_match(*input.begin(), Patterns::drop)) {
            std::cout << "drop";
        } else if (std::regex_match(*input.begin(), Patterns::alter)) {
            std::cout << "alter";
        } else if (std::regex_match(*input.begin(), Patterns::truncate)) {
            std::cout << "truncate";
        } else if (std::regex_match(*input.begin(), Patterns::insert)) {
            std::cout << "insert";
        } else if (std::regex_match(*input.begin(), Patterns::update)) {
            std::cout << "update";
        } else if (std::regex_match(*input.begin(), Patterns::del)) {
            std::cout << "delete";
        } //draft*/ //draft

        /*for (int i = 0; i < input.size(); i++) {
            if (std::regex_match(input[0], Patterns::select)){
                std::cout << "select";
                ast.emplace_back("SELECT");

            } else if (std::regex_match(*input.begin(), Patterns::create)) {
                std::cout << "create";
            } else if (std::regex_match(*input.begin(), Patterns::drop)) {
                std::cout << "drop";
            } else if (std::regex_match(*input.begin(), Patterns::alter)) {
                std::cout << "alter";
            } else if (std::regex_match(*input.begin(), Patterns::truncate)) {
                std::cout << "truncate";
            } else if (std::regex_match(*input.begin(), Patterns::insert)) {
                std::cout << "insert";
            } else if (std::regex_match(*input.begin(), Patterns::update)) {
                std::cout << "update";
            } else if (std::regex_match(*input.begin(), Patterns::del)) {
                std::cout << "delete";
            }
        }*/
    }
};


#endif //DATABASE_CHECKER_H
