#include <bits/stdc++.h>
#include "Table.h"
#include "Patterns.h"
#include "Checker.h"

auto lexicalAnalysis(std::string& str) {
    auto output = std::vector<std::string>();
    auto word = std::string();
    for (auto symbol : str) {
        if (symbol == ' ') {
            if (!word.empty()) output.push_back(word);
            word = "";
        } else if (symbol == ',' or symbol == ';' or symbol == '(' or symbol == ')') {
            if (!word.empty()) output.push_back(word);
            if (symbol == ',') output.emplace_back(",");
            else if (symbol == ';') output.emplace_back(";");
            else if (symbol == '(') output.emplace_back("(");
            else output.emplace_back(")");
            word = "";
        } else word += symbol;
    }
    if (!word.empty()) output.push_back(word);
    return output;
}

//auto syntaxCheck(std::vector<std::string> input) {
//    auto parameters = std::map<std::string, std::vector<std::string>>();
//    auto ast = std::vector<std::string>(); 
//    if (std::regex_match(*input.begin(), Patterns::select)){
//            std::cout << "select";
//            ast.emplace_back("SELECT");
//
//        } else if (std::regex_match(*input.begin(), Patterns::create)) {
//            std::cout << "create";
//        } else if (std::regex_match(*input.begin(), Patterns::drop)) {
//            std::cout << "drop";
//        } else if (std::regex_match(*input.begin(), Patterns::alter)) {
//            std::cout << "alter";
//        } else if (std::regex_match(*input.begin(), Patterns::truncate)) {
//            std::cout << "truncate";
//        } else if (std::regex_match(*input.begin(), Patterns::insert)) {
//            std::cout << "insert";
//        } else if (std::regex_match(*input.begin(), Patterns::update)) {
//            std::cout << "update";
//        } else if (std::regex_match(*input.begin(), Patterns::del)) {
//            std::cout << "delete";
//        }
//}

int main() {
    auto tables = std::map<std::string, Table>();
    while (true) {
        auto query = std::string();
        auto command = std::vector<std::string>();
        std::getline(std::cin, query);
        command = lexicalAnalysis(query);

        //output
//        for (int i = 0; i < command.size(); ++i) {
//            std::cout << i << ". " << command[i] << std::endl;
//        }
        std::cout << "----------------------------------------------------" << std::endl;
        auto checker = Checker();
        checker.syntaxCheck(command);
        return 0;
    }
}

//TODO: think how (anf if) you can implement some sort of AST (abstract syntax tree). First thought: stack
//TODO: move infinite if's (line ~32) to a separate function. I suppose this function will become the syntax check: done
//TODO: make syntax check (haha, gl)
//TODO: AST (abstract syntax tree). If you haven't lost your sanity before (wow), you will lose it here (as well as your anal virginity)