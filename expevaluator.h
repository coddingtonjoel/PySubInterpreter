#ifndef PYSUBINTERPRETER_EXPEVALUATOR_H
#define PYSUBINTERPRETER_EXPEVALUATOR_H

#include <map>
#include <string>


class expEvaluator {
private:
    friend class Interface;

    typedef std::map<std::string, std::string> symbolTableType;
    symbolTableType symbolTable;

    std::string infixToPostfix(std::string&);
    double postfixEval(std::string&);
    void storeSymbol(const std::string&, std::string);
    std::string getSymbol(const std::string&);
    void clearAllSymbols();
    int getPrecedence(const std::string&);
};


#endif //PYSUBINTERPRETER_EXPEVALUATOR_H
