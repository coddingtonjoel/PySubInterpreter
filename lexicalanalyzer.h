#ifndef PYSUBINTERPRETER_LEXICALANALYZER_H
#define PYSUBINTERPRETER_LEXICALANALYZER_H

#include <string>
#include <vector>

enum class categoryType {
    KEYWORD,
    IDENTIFIER,
    STRING_LITERAL,
    NUMERIC_LITERAL,
    ASSIGNMENT_OP,
    ARITH_OP,
    LOGICAL_OP,
    RELATIONAL_OP,
    LEFT_PAREN,
    RIGHT_PAREN,
    COLON,
    COMMA,
    COMMENT,
    INDENT,
    UNKNOWN
};

// vector structure of all the lines of code in the file
typedef std::vector<std::vector<std::pair<std::string, categoryType>>> tokenType;

// vector of pairs intended for each line of code
typedef std::vector<std::pair<std::string, categoryType>> tokenLineType;

// for use instead of declaring pairs of strings/categoryType
typedef std::pair<std::string, categoryType> pairType;

class LexicalAnalyzer {
private:
    friend class Interface;
    // main structure
    tokenType tokenInfo;
    // tokenize a full file
    bool tokenize(std::vector<std::string> &);
    void display(tokenType &);
};


#endif //PYSUBINTERPRETER_LEXICALANALYZER_H
