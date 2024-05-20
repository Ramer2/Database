#include <bits/stdc++.h>
#include "Table.h"
#include "Patterns.h"
#include "Engine.h"
#include "Database.h"

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

int main() {
    while (true) {
        auto query = std::string();
        auto command = std::vector<std::string>();
//        std::getline(std::cin, query);
        query = "CREATE TABLE employees (id INTEGER PRIMARY KEY, first_name NUMBER(20, 2) not null, last_name VARCHAR(75) NOT NULL, mid_name VARCHAR(50) NULL, dateofbirth DATE NOT NULL);";
        command = lexicalAnalysis(query);

        //output
//        for (int i = 0; i < command.size(); ++i) {
//            std::cout << i << ". " << command[i] << std::endl;
//        }
        std::cout << "----------------------------------------------------" << std::endl;
        auto engine = Engine();
        engine.codeRetrieval(command);
        engine.completer();

        //database output
        int counter = 0;
        for (auto const& el : Database::database) {
            for (auto const& col : el.columns) {
                std::cout << counter++ << ". " << col.first << " " << col.second << std::endl;
            }
        }

        return 0;
    }
}
//TODO: CREATE TABLE command (with readings of data type and nullable fields)
//TODO: figure out how to store tables so that they are available everywhere in the project

//TODO: AST (abstract syntax tree). If you haven't lost your sanity before (wow), you will lose it here (as well as your anal virginity)
//TODO: finish syntax check (haha, gl)

//TODO: encapsulation (private fields, getters, setters) classes: table, row, engine
