#include "Table.h"
#include "Patterns.h"
#include "Engine.h"
#include "Database.h"
#include <fstream>
#include <filesystem>

auto reader() {

}

auto saver() {
    auto dirPath = std::string("../tables");
    for (auto const& file : std::filesystem::directory_iterator(dirPath))
        std::filesystem::remove(file);

    for (auto& table : Database::database) {
        auto file_name = table.name;
        try {
            auto file = std::ofstream("../tables/" + file_name + ".txt");
            //writing table name
            file << table.name + '\n';

            //writing column names
            for (auto const& el : table.col_names)
                file << el << " ";
            file << '\n';

            //writing primary keys
            for (auto const& el : table.col_names)
                file << table.columns[el] << " ";
            file << '\n';

            for (auto const& el : table.dataType)
                file << el << " ";
            file << '\n';

            for (auto const& row : table.content) {
                for (int i = 0; i < row.attributes.size(); i++) {
                    if (table.dataType[i] == "i") {
                        file << std::get<int>(row.attributes[i]) << " ";
                    } else if (table.dataType[i] == "f") {
                        file << std::get<float>(row.attributes[i]) << " ";
                    } else if (table.dataType[i] == "s") {
                        file << std::get<std::string>(row.attributes[i]) << " ";
                    }
                }
                file << '\n';
            }
            file.close();
        } catch (std::exception const& e) {
            std::cout << "Problem, while saving files. Contact your personal programmer.";
            exit(-1);
        }
    }
}

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
        query = "CREATE TABLE employees (id INTEGER PRIMARY KEY, first_name VARCHAR(50) not null, last_name VARCHAR(75) NOT NULL, mid_name VARCHAR(50) NULL, dateofbirth DATE NOT NULL);";
//        query = "INSERT INTO table_name (col1, col2, col3) VALUES (val1, val2, val3), (val4, val5, val6);";
        command = lexicalAnalysis(query);
        auto engine = Engine();
        engine.codeRetrieval(command);
        engine.completer();

        std::vector<std::variant<int, float, std::string>> input1;
        input1.emplace_back(1);
        input1.emplace_back("Oleksandr");
        input1.emplace_back("null");
        input1.emplace_back("Usyk");
        input1.emplace_back("17.01.1987");
        std::vector<std::variant<int, float, std::string>> input2;
        input2.emplace_back(2);
        input2.emplace_back("Tyson");
        input2.emplace_back("Luke");
        input2.emplace_back("Fury");
        input2.emplace_back("12.08.1988");

        for (auto const& el : Database::database[0].dataType) {
            std::cout << el << " ";
        }
        std::cout << std::endl;

        //database output
        int counter = 0;
        for (auto& table : Database::database) {
            table.addRow(input1);
            table.addRow(input2);
            std::cout << counter++ << ". " << table.name;
        }

        saver();

        return 0;
    }
}

//TODO: implement saving
//TODO: implement reading

//TODO: encapsulation (private fields, getters, setters) classes: table, row, engine
//TODO: finish the commands