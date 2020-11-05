#include "expevaluator.h"
#include <iostream>
#include <stack>
#include "lexicalanalyzer.h"
#include <string>
#include <cmath>
#include <iomanip>
#include <sstream>

using namespace std;

// infix to postfix
std::string expEvaluator::infixToPostfix(tokenLineType lineTokens) {
    stack<string> infixStack;
    string postfixExpr;

    for (const auto& tokenPair : lineTokens) {
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

// postfix evaluation
double expEvaluator::postfixEval(tokenLineType lineTokens) {
    stack<string> postfixStack;

    // mini-tokenization of postfixExpr
    for (const auto& tokenPair : lineTokens) {
        // push numbers into stack
        if (tokenPair.second == categoryType::NUMERIC_LITERAL) {
            postfixStack.push(tokenPair.first);
        }
        // all operators but "not" (because it's unary)
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
        else {
            cout << "\n**ERROR: Invalid syntax.**\n" << endl;
            return -1;
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

// check for syntax errors and missing operands before postfix conversion
std::string expEvaluator::checkForErrors(tokenLineType lineTokens) {
    // example 2D table

    // row        pair(s)
    // -----------------------
    // 3 + 10     [3: NUMERIC_LITERAL], [+: ARITHMETIC_OP], [10: NUMERIC_LITERAL]
    // ......     ............
    for (int row = 0; row < lineTokens.size(); row++) {
            // handle identifiers FOR NOW
            if (lineTokens[row].second == categoryType::IDENTIFIER) {
                cout << "\n**ERROR: Invalid syntax.**\n" << endl;
                return "err";
            }
            // if an operator is the very first or very last token within a line
            else if (lineTokens[row].second == categoryType::ARITH_OP || lineTokens[row].second ==
                categoryType::ASSIGNMENT_OP || lineTokens[row].second == categoryType::RELATIONAL_OP ||
                lineTokens[row].second == categoryType::LOGICAL_OP) {
                if (row == 0) {
                    cout << "\n**ERROR: Invalid syntax.**\n" << endl;
                    return "err";
                }
            }
            // if any unknown character exists
            else if (lineTokens[row].second == categoryType::UNKNOWN) {
                cout << "\n**ERROR: Invalid syntax.**\n" << endl;
                return "err";
            }
            // checks that require the size of the token structure to be greater than one 
            else if (lineTokens.size() > 1 && row + 1 < lineTokens.size()) {
                // two numeric literals next to each other
                if (lineTokens[row].second == categoryType::NUMERIC_LITERAL && lineTokens[row + 1].second ==
                    categoryType::NUMERIC_LITERAL) {
                    cout << "\n**ERROR: Missing operand.**\n" << endl;
                    return "err";
                }
                // two arithmetic operators next to each other
                else if (lineTokens[row].second == categoryType::ARITH_OP && lineTokens[row + 1].second ==
                    categoryType::ARITH_OP) {
                    cout << "\n**ERROR: Invalid syntax.**\n" << endl;
                    return "err";
                }
                // two relational operators next to each other
                else if (lineTokens[row].second == categoryType::RELATIONAL_OP && lineTokens[row + 1].second ==
                    categoryType::RELATIONAL_OP) {
                    cout << "\n**ERROR: Invalid syntax.**\n" << endl;
                    return "err";
                }
                // two assignment operators next to each other
                else if (lineTokens[row].second == categoryType::ASSIGNMENT_OP && lineTokens[row + 1].second ==
                    categoryType::ASSIGNMENT_OP) {
                    cout << "\n**ERROR: Invalid syntax.**\n" << endl;
                    return "err";
                }
                // two logical operators next to each other
                else if (lineTokens[row].second == categoryType::LOGICAL_OP && lineTokens[row + 1].second ==
                    categoryType::LOGICAL_OP) {
                    cout << "\n**ERROR: Invalid syntax.**\n" << endl;
                    return "err";
                }
                // two parentheses operators next to each other with nothing inside
                else if (lineTokens[row].second == categoryType::LEFT_PAREN && lineTokens[row + 1].second ==
                    categoryType::RIGHT_PAREN) {
                    cout << "\n**ERROR: Invalid syntax.**\n" << endl;
                    return "err";
                }
                // if only one token exists and it's not a numeric literal
                else if (lineTokens.size() == 1 && lineTokens[0].second != categoryType::NUMERIC_LITERAL) {
                    cout << "\n**ERROR: Invalid syntax.**\n" << endl;
                    return "err";
                }
                // if a numeric literal is directly on the right of a right parentheses -- ex. (4 + 5)4
                else if (lineTokens[row].second == categoryType::RIGHT_PAREN && lineTokens[row + 1].second ==
                    categoryType::NUMERIC_LITERAL) {
                    if (row != lineTokens.size() - 1) {
                        cout << "\n**ERROR: Invalid syntax.**\n" << endl;
                        return "err";
                    }
                }
                
            }
            // if a numeric literal is directly on the left of a left parentheses -- ex. 4(4 + 5)
            else if (row > 0) {
                if (lineTokens[row].second == categoryType::LEFT_PAREN && lineTokens[row - 1].second ==
                categoryType::NUMERIC_LITERAL) {
                    cout << "\n**ERROR: Invalid syntax.**\n" << endl;
                    return "err";
                }
            }
            
        }
    return "noErr";
}

std::string expEvaluator::evaluate(tokenLineType line) {
    LexicalAnalyzer lexAnalysis;
    vector<string> temp;

    // proofread code for errors before running it through infix/postfix conversions
    string result = checkForErrors(line);
    if (result != "err") {
        tokenLineType subLine;

        string postfix = infixToPostfix(line);
        lexAnalysis.tokenInfo.clear();
        temp.push_back(postfix);
        lexAnalysis.tokenize(temp);
        // convert to nice formatting using a string stream
        std::ostringstream oss;
        oss << std::setprecision(8) << std::noshowpoint << postfixEval(lexAnalysis.tokenInfo[0]);
        return oss.str();
    }
    else return "";
}