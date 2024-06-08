#include <regex>

#ifndef DATABASE_PATTERNS_H
#define DATABASE_PATTERNS_H

//this class is used for storing patterns (I tried to use enum but was very disappointed)
class Patterns {
public:
    static const std::regex select;
    static const std::regex create;
    static const std::regex drop;
    static const std::regex alter;
    static const std::regex truncate;
    static const std::regex insert;
    static const std::regex values;
    static const std::regex update;
    static const std::regex del;
    static const std::regex from;
    static const std::regex into;
    static const std::regex where;
    static const std::regex or_;
    static const std::regex and_;
    static const std::regex not_;
    static const std::regex table;
    static const std::regex integer;
    static const std::regex varchar;
    static const std::regex number;
    static const std::regex primary;
    static const std::regex key;
    static const std::regex null;
    static const std::regex date;
    static const std::regex foreign;
    static const std::regex references;
    static const std::regex join;
    static const std::regex on;
};

#endif //DATABASE_PATTERNS_H
