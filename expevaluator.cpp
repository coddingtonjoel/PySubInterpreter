#include "expevaluator.h"
#include <iostream>
#include <stack>
#include "lexicalanalyzer.h"
#include <string>
#include <cmath>

using namespace std;

std::string expEvaluator::infixToPostfix(tokenType allTokens) {
    stack<string> infixStack;
    string postfixExpr;

    for (const auto& tokenPair : allTokens[0]) {
        stack<string> temp = infixStack;

        // numbers
        if (tokenPair.second == categoryType::NUMERIC_LITERAL) {
            postfixExpr.append(tokenPair.first + " ");
        }
        // (
        else if (tokenPair.second == categoryType::LEFT_PAREN) {
            infixStack.push("(");
        }
        // )
        else if (tokenPair.second == categoryType::RIGHT_PAREN) {
            // pop and append each stack top to postfixExpr
            while (infixStack.top() != "(") {
                postfixExpr.append(infixStack.top() + " ");
                infixStack.pop();
            }
            // pop the final '(' without appending it to postfixExpr
            infixStack.pop();
        }
        // all operators but assignment
        else if (tokenPair.second == categoryType::ARITH_OP || tokenPair.second == categoryType::LOGICAL_OP ||
        tokenPair.second == categoryType::RELATIONAL_OP) {
            while (!infixStack.empty() && infixStack.top() != "(" && getPrecedence(tokenPair.first) <= getPrecedence
            (infixStack.top())) {
                postfixExpr.append(infixStack.top() + " ");
                infixStack.pop();
            }
            infixStack.push(tokenPair.first);
        }
    }

    // push and pop until infixStack is empty, and then postfixExpr is finished
    while(!infixStack.empty()) {
        postfixExpr.append(infixStack.top() + " ");
        infixStack.pop();
    }

    return postfixExpr;
}

double expEvaluator::postfixEval(tokenType allTokens) {
    stack<string> postfixStack;

    // mini-tokenization of postfixExpr
    for (const auto& tokenPair : allTokens[0]) {
        // push numbers into stack
        if (tokenPair.second == categoryType::NUMERIC_LITERAL) {
            postfixStack.push(tokenPair.first);
        }
        // all operators but "not" because it's unary
        else if ((tokenPair.second == categoryType::ARITH_OP || tokenPair.second == categoryType::RELATIONAL_OP ||
                (tokenPair.first == "and" && tokenPair.second == categoryType::LOGICAL_OP) || (tokenPair.first ==
                "or" && tokenPair.second == categoryType::LOGICAL_OP)) &&
        postfixStack.size() >= 2) {
            double operand2 = stod(postfixStack.top());
            postfixStack.pop();
            double operand1 = stod(postfixStack.top());
            double value;

            if (tokenPair.first == "+") {
                value = operand1 + operand2;
            }
            else if (tokenPair.first == "-") {
                value = operand1 - operand2;
            }
            else if (tokenPair.first == "*") {
                value = operand1 * operand2;
            }
            else if (tokenPair.first == "/") {
                value = operand1 / operand2;
            }
            // uses fmod() from cmath to allow for usage with doubles
            else if (tokenPair.first == "%") {
                value = fmod(operand1, operand2);
            }
            else if (tokenPair.first == "<") {
                value = operand1 < operand2;
            }
            else if (tokenPair.first == "<=") {
                value = operand1 <= operand2;
            }
            else if (tokenPair.first == ">") {
                value = operand1 > operand2;
            }
            else if (tokenPair.first == ">=") {
                value = operand1 >= operand2;
            }
            else if (tokenPair.first == "!=") {
                value = operand1 != operand2;
            }
            else if (tokenPair.first == "==") {
                value = operand1 == operand2;
            }
            else if (tokenPair.first == "and") {
                value = operand1 && operand2;
            }
            else if (tokenPair.first == "or") {
                value = operand1 || operand2;
            }
            else {
                value = -1;
            }
            postfixStack.pop();
            postfixStack.push(to_string(value));
        }
        // logical operator "not"
        else if ((tokenPair.first == "not" && tokenPair.second == categoryType::LOGICAL_OP) && !postfixStack.empty()) {
            double operand = stod(postfixStack.top());
            double value = !operand;
            postfixStack.push(to_string(value));
        }
    }

    return stod(postfixStack.top());
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