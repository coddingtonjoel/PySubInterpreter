#ifndef PYSUBINTERPRETER_EXPEVALUATOR_H
#define PYSUBINTERPRETER_EXPEVALUATOR_H

#include <map>
#include <string>
#include "lexicalanalyzer.h"


class expEvaluator {
private:
    friend class Interface;
    friend class Interpreter;

    typedef std::map<std::string, std::string> symbolTableType;
    static symbolTableType symbolTable;

    std::string infixToPostfix(tokenLineType);
    double postfixEval(tokenLineType);
    void storeSymbol(const std::string&, std::string);
    std::string getSymbol(const std::string&);
    void clearAllSymbols();
    int getPrecedence(const std::string&);
    std::string checkForErrors(tokenLineType);
    std::string evaluate(tokenLineType);
    void displaySymbols();
};


#endif //PYSUBINTERPRETER_EXPEVALUATOR_H
