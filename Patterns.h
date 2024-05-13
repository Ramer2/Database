#include <bits/stdc++.h>

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
    static const std::regex update;
    static const std::regex del;
    static const std::regex from;
    static const std::regex into;
    static const std::regex where;
    static const std::regex or_;
    static const std::regex and_;
    static const std::regex not_;
    //TODO: write regexes for or, and, not and some more
};

#endif //DATABASE_PATTERNS_H
