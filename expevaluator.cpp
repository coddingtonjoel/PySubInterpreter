#include "expevaluator.h"
#include <iostream>
#include <stack>

using namespace std;


std::string expEvaluator::infixToPostfix(string &infixExpr) {
    return std::string();
}

double expEvaluator::postfixEval(string &postfixExpr) {
    return 0;
}

void expEvaluator::storeSymbol(const string& symbol, string symbolVal) {
    symbolTable[symbol] = symbolVal;
}

std::string expEvaluator::getSymbol(const string& symbol) {
    if (symbolTable.find(symbol) == symbolTable.end()) {
        cout << "***Unable to find symbol" << endl;
        return "ERR";
    }
    else {
        return symbolTable[symbol];
    }
}

void expEvaluator::clearAllSymbols() {
    symbolTable.clear();
}

// return operator precedence
int expEvaluator::getPrecedence(const string& op) {
    if (op == "*" || op == "/" || op == "%") {
        return 5;
    }
    else if (op == "+" || op == "-") {
        return 4;
    }
    else if (op == "<" || op == "<=" || op == ">" || op == ">=" || op == "!=" || op == "==") {
        return 3;
    }
    else if (op == "not") {
        return 2;
    }
    else if (op == "and") {
        return 1;
    }
    else if (op == "or") {
        return 0;
    }
    else return -1;
}