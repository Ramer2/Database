//!1948 lines!
// https://github.com/Ramer2/Database

#include "Engine.h"

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

        //reading FKs
        auto fks = std::map<std::string, bool>();
        auto references = std::map<std::string, std::string>();
        auto fkCols = std::vector<std::string>();
        {
            auto iterator = 0;
            do {
                if (input == "1") {
                    fks[col_names[iterator]] = true;
                    fkCols.push_back(col_names[iterator]);
                } else {
                    fks[col_names[iterator]] = false;
                }
                table >> input;
                iterator++;
            } while (iterator < col_names.size());

            if (!fkCols.empty()) {
                iterator = 0;
                do {
                    std::string refCol = input;
                    table >> input;
                    references[refCol] = input;
                    table >> input;
                    iterator++;
                } while (iterator < fkCols.size());
            }
        }

        //reading nullable fields
        auto nullable = std::map<std::string, bool>();
        {
            auto iterator = 0;
            do {
                if (input == "1") {
                    nullable[col_names[iterator]] = true;
                }
                else {
                    nullable[col_names[iterator]] = false;
                }

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
        auto newTable = Table(table_name, col_names, columns, fks, nullable, dataType, references);

        for (int i = 0; i < lineCount - (6 + references.size()); i++) {
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

auto lexicalAnalysis(std::string& str) {
    auto output = std::vector<std::string>();
    auto word = std::string();
    for (auto symbol : str) {
        if (symbol == ' ') {
            if (!word.empty()) output.push_back(word);
            word = "";
        } else if (symbol == ',' or symbol == ';' or symbol == '(' or symbol == ')' or symbol == '\'') {
            if (!word.empty()) output.push_back(word);
            if (symbol == ',') output.emplace_back(",");
            else if (symbol == ';') output.emplace_back(";");
            else if (symbol == '(') output.emplace_back("(");
            else if (symbol == '\'');
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
        command = lexicalAnalysis(query);
        if (std::regex_match(command[0], Patterns::exit)) {
            fmt::println("The database was saved and the program was successfully terminated.");
            return 0;
        } else if (std::regex_match(command[0], Patterns::database)) {
            int counter = 0;
            for (auto const& table : Database::database) {
                fmt::println("{}. {}", counter, table.name);
                counter++;
            }
            continue;
        }

        Engine::codeRetrieval(command);
        Engine::completer();
    }
}