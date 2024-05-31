#include "Engine.h"
#include <fstream>
#include <filesystem>

auto reader() {
    auto dirPath = std::string("../tables");
    for (auto const& file : std::filesystem::directory_iterator(dirPath)) {
        std::ifstream table(file.path());
        auto input = std::string();
        auto lineCount = int();

        {
            auto line = std::string();
            while (std::getline(table, line)) lineCount++;
        }
        table.clear();
        table.seekg(0);

        //reading table name
        auto table_name = std::string();
        table >> table_name;

        //reading a set of column names
        auto col_names = std::vector<std::string>();
        while (table >> input and !(input == "1" or input == "0")) {
            col_names.emplace_back(input);
        }

        //reading PKs
        auto columns = std::map<std::string, bool>();
        {
            auto iterator = 0;
            do {
                if (input == "1")
                    columns[col_names[iterator]] = true;
                else
                    columns[col_names[iterator]] = false;

                table >> input;
                iterator++;
            } while (iterator < col_names.size());
        }

        //reading nullable fields
        auto nullable = std::map<std::string, bool>();
        {
            auto iterator = 0;
            do {
                if (input == "1")
                    nullable[col_names[iterator]] = true;
                else
                    nullable[col_names[iterator]] = false;

                table >> input;
                iterator++;
            } while (iterator < col_names.size());
        }

        //reading dataTypes
        auto dataType = std::vector<std::string>();
        {
            auto iterator = 0;
            do {
                dataType.emplace_back(input);
                table >> input;
                iterator++;
            } while (iterator < col_names.size());
        }

        //creating a new table (without any rows)
        auto newTable = Table(table_name, col_names, columns, nullable, dataType);

        for (int i = 0; i < lineCount - 5; i++) {
            auto row = std::vector<std::variant<int, float, std::string>>();
            {
                auto iterator = 0;
                do {
                    if (dataType[iterator] == "i") {
                        row.emplace_back(std::stoi(input));
                    } else if (dataType[iterator] == "f") {
                        row.emplace_back(std::stof(input));
                    } else {
                        row.emplace_back(input);
                    }
                    table >> input;
                    iterator++;
                } while (iterator < col_names.size());
            }
            newTable.addFullRow(row);
        }

        Database::database.emplace_back(newTable);
        table.close();
    }
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

            //writing nullable columns
            for (auto const& el : table.col_names)
                file << table.nullable[el] << " ";
            file << '\n';

            //writing data types
            for (auto const& el : table.dataType) {
                file << el << " ";
            }
            file << '\n';

            //writing down rows
            for (auto const& row : table.content) {
                for (int i = 0; i < row.attributes.size(); i++) {
//                    std::cout << i << ". " << row.attributes[i]. << std::endl;
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
            std::cout << e.what() << std::endl;
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
    reader();
    while (true) {
        auto query = std::string();
        auto command = std::vector<std::string>();
        std::getline(std::cin, query);
//        query = "CREATE TABLE employees (id INTEGER PRIMARY KEY, first_name VARCHAR(50) not null,  mid_name VARCHAR(50) NULL, last_name VARCHAR(75) NOT NULL, dateofbirth DATE NOT NULL);";
//        query = "INSERT INTO employees (id, first_name, last_name, dateofbirth) VALUES (1, Oleksandr, Usyk, 17.01.1987), (2, Tyson, Fury, 12.08.1988);";
//        query = "DROP TABLE employees;";
//        query = "SELECT * FROM employees, something;";
        command = lexicalAnalysis(query);

        Engine::codeRetrieval(command);
        Engine::completer();
        saver();

        return 0;
    }
}

//employees
//id first_name mid_name last_name dateofbirth
//1 0 0 0 0
//0 0 1 0 0
//i s s s s
//1 Oleksandr null Usyk 17.01.1987
//2 Tyson null Fury 12.08.1988

//TODO: implement SELECT ... FROM ... (without condition, for now)
//idea: try to do this with creating new tables

//TODO: encapsulation (private fields, getters, setters) classes: table, row, engine

//TODO: finish the commands