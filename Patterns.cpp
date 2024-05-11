#include "Patterns.h"

//everything below was the advice of ChatGPT after giving him this prompt:
// Static data member of type 'const std::regex' (aka 'const basic_regex<char>') must be initialized out of line
//*I gave him the error message
const std::regex Patterns::select = std::regex("select", std::regex_constants::icase);
const std::regex Patterns::create = std::regex("create", std::regex_constants::icase);
const std::regex Patterns::drop = std::regex("drop", std::regex_constants::icase);
const std::regex Patterns::alter = std::regex("alter", std::regex_constants::icase);
const std::regex Patterns::truncate = std::regex("truncate", std::regex_constants::icase);
const std::regex Patterns::insert = std::regex("insert", std::regex_constants::icase);
const std::regex Patterns::update = std::regex("update", std::regex_constants::icase);
const std::regex Patterns::del = std::regex("delete", std::regex_constants::icase);
const std::regex Patterns::from = std::regex("from", std::regex_constants::icase);
const std::regex Patterns::into = std::regex("into", std::regex_constants::icase);
const std::regex Patterns::where = std::regex("where", std::regex_constants::icase);