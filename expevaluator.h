#ifndef PYSUBINTERPRETER_EXPEVALUATOR_H
#define PYSUBINTERPRETER_EXPEVALUATOR_H

#include <map>
#include <string>
#include "lexicalanalyzer.h"


class expEvaluator {
private:
    friend class Interface;

    typedef std::map<std::string, std::string> symbolTableType;
    symbolTableType symbolTable;

    std::string infixToPostfix(tokenType);
    double postfixEval(tokenType);
    void storeSymbol(const std::string&, std::string);
    std::string getSymbol(const std::string&);
    void clearAllSymbols();
    int getPrecedence(const std::string&);
};


#endif //PYSUBINTERPRETER_EXPEVALUATOR_H
