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
const std::regex Patterns::values = std::regex("values", std::regex_constants::icase);
const std::regex Patterns::update = std::regex("update", std::regex_constants::icase);
const std::regex Patterns::del = std::regex("delete", std::regex_constants::icase);
const std::regex Patterns::from = std::regex("from", std::regex_constants::icase);
const std::regex Patterns::into = std::regex("into", std::regex_constants::icase);
const std::regex Patterns::where = std::regex("where", std::regex_constants::icase);
const std::regex Patterns::and_ = std::regex("and", std::regex_constants::icase);
const std::regex Patterns::or_ = std::regex("or", std::regex_constants::icase);
const std::regex Patterns::not_ = std::regex("not", std::regex_constants::icase);
const std::regex Patterns::table = std::regex("table", std::regex_constants::icase);
const std::regex Patterns::integer = std::regex("integer", std::regex_constants::icase);
const std::regex Patterns::varchar = std::regex("varchar", std::regex_constants::icase);
const std::regex Patterns::number = std::regex("number", std::regex_constants::icase);
const std::regex Patterns::primary = std::regex("primary", std::regex_constants::icase);
const std::regex Patterns::key = std::regex("key", std::regex_constants::icase);
const std::regex Patterns::null = std::regex("null", std::regex_constants::icase);
const std::regex Patterns::date = std::regex("date", std::regex_constants::icase);
const std::regex Patterns::foreign = std::regex("foreign", std::regex_constants::icase);
const std::regex Patterns::references = std::regex("references", std::regex_constants::icase);
const std::regex Patterns::join = std::regex("join", std::regex_constants::icase);
const std::regex Patterns::on = std::regex("on", std::regex_constants::icase);
const std::regex Patterns::order = std::regex("order", std::regex_constants::icase);
const std::regex Patterns::by = std::regex("by", std::regex_constants::icase);
const std::regex Patterns::asc = std::regex("asc", std::regex_constants::icase);
const std::regex Patterns::desc = std::regex("desc", std::regex_constants::icase);
const std::regex Patterns::add = std::regex("add", std::regex_constants::icase);
const std::regex Patterns::column = std::regex("column", std::regex_constants::icase);