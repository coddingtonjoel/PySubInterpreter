#include "lexicalanalyzer.h"
#include <iostream>
#include <cctype>
#include <vector>
#include <string>

using namespace std;

// generate token and category pairs for each line of code
// utilize lineTokens?
bool LexicalAnalyzer::tokenize(vector<string> &codeLines) {
    for (int i = 0; i < codeLines.size(); i++) {
        tokenLineType lineTokens;
        string token;
        categoryType type;

        // indent
        if (isspace(codeLines[i][0])) {
            type = categoryType::INDENT;
            token = "____";
            lineTokens.push_back(make_pair(token, type));
        }

        for (int j = 0; j < codeLines[i].size(); j++) {
            // clear current token, if any
            token = "";

            // numeric literal
            if (isdigit(codeLines[i][j])) {
                type = categoryType::NUMERIC_LITERAL;
                token = codeLines[i][j];
                // check if the next characters are digits too, if so, add them to same token
                int counter = 1;
                while (isdigit(codeLines[i][j + counter]) || codeLines[i][j + counter] == '.') {
                    token.push_back(codeLines[i][j + counter]);
                    counter++;
                }
                j = j + counter - 1;
                lineTokens.push_back(make_pair(token, type));
            }
                // first char is letter cases
            else if (isalpha(codeLines[i][j])) {
                // PRINT keyword
                if (codeLines[i][j] == 'p' && codeLines[i][j + 1] == 'r' && codeLines[i][j + 2] == 'i' && codeLines[i][j + 3] == 'n' && codeLines[i][j +
                                                                                              4] == 't' &&
                                                                                              !isalpha(codeLines[i][j
                                                                                              + 5])) {
                    token = "print";
                    type = categoryType::KEYWORD;
                    j = j + 4;
                }
                // IF keyword
                else if (codeLines[i][j] == 'i' && codeLines[i][j + 1] == 'f' &&
                                                   !isalpha(codeLines[i][j
                                                                         + 2])) {
                    token = "if";
                    type = categoryType::KEYWORD;
                    j++;
                }
                // ELIF keyword
                else if (codeLines[i][j] == 'e' && codeLines[i][j + 1] == 'l' && codeLines[i][j + 2] == 'i' && codeLines[i][j + 3] == 'f' &&
                                                                                                               !isalpha(codeLines[i][j
                                                                                                                                     + 4])) {
                    token = "elif";
                    type = categoryType::KEYWORD;
                    j = j + 3;
                }
                // ELSE keyword
                else if (codeLines[i][j] == 'e' && codeLines[i][j + 1] == 'l' && codeLines[i][j + 2] == 's' && codeLines[i][j + 3] == 'e' &&
                                                                                                               !isalpha(codeLines[i][j
                                                                                                                                     + 4])) {
                    token = "else";
                    type = categoryType::KEYWORD;
                    j = j + 3;
                }
                // FOR keyword
                else if (codeLines[i][j] == 'f' && codeLines[i][j + 1] == 'o' && codeLines[i][j + 2] == 'r') {
                    token = "for";
                    type = categoryType::KEYWORD;
                    j = j + 2;
                }
                // WHILE keyword
                else if (codeLines[i][j] == 'w' && codeLines[i][j + 1] == 'h' && codeLines[i][j + 2] == 'i' && codeLines[i][j + 3] == 'l' &&
                        codeLines[i][j +
                           4] == 'e' &&
                        !isalpha(codeLines[i][j
                                              + 5])) {
                    token = "while";
                    type = categoryType::KEYWORD;
                    j = j + 4;
                }
                // AND logical operator
                else if (codeLines[i][j] == 'a' && codeLines[i][j + 1] == 'n' && codeLines[i][j + 2] == 'd' &&
                                                                                 !isalpha(codeLines[i][j
                                                                                                       + 3])) {
                    token = "and";
                    type = categoryType::LOGICAL_OP;
                    j = j + 2;
                }
                // OR logical operator
                else if (codeLines[i][j] == 'o' && codeLines[i][j + 1] == 'r' &&
                                                   !isalpha(codeLines[i][j
                                                                         + 2])) {
                    token = "or";
                    type = categoryType::LOGICAL_OP;
                    j++;
                }
                // NOT logical operator
                else if (codeLines[i][j] == 'n' && codeLines[i][j + 1] == 'o' && codeLines[i][j + 2] == 't' &&
                                                                                 !isalpha(codeLines[i][j
                                                                                                       + 3])) {
                    token = "not";
                    type = categoryType::LOGICAL_OP;
                    j = j + 2;
                }
                // identifier of some sort
                else {
                    token = codeLines[i][j];
                    type = categoryType::IDENTIFIER;
                    // check if the next characters are part of an identifier
                    int counter = 1;

                    while (isalpha(codeLines[i][j + counter])) {
                        token.push_back(codeLines[i][j + counter]);
                        counter++;
                    }
                    j = j + counter - 1;
                }
                lineTokens.push_back(make_pair(token, type));
            }
            // left parenthesis
            else if (codeLines[i][j] == '(') {
                token = codeLines[i][j];
                type = categoryType::LEFT_PAREN;
                lineTokens.push_back(make_pair(token, type));
                int counter = 1;

                // catch the case where codeLines[i][j + counter] doesn't exist
                if (j + counter > codeLines[i].length()) {
                    cout << "\n**ERROR: Unbalanced parentheses.**\n" << endl;
                    cout << "Line " << i + 1 << ":" << j + 1 << " : " << codeLines[i] << endl << endl;
                    return false;
                }

                // catch missing right parentheses
                while (codeLines[i][j + counter] != ')') {
                    if (counter == codeLines[i].length()) {
                        cout << "\n**ERROR: Unbalanced parentheses.**\n" << endl;
                        cout << "Line " << i + 1 << ":" << j + 1 << " : " << codeLines[i] << endl << endl;
                        return false;
                    }
                    counter++;
                }
            }
            // right parenthesis
            else if (codeLines[i][j] == ')') {
                token = codeLines[i][j];
                type = categoryType::RIGHT_PAREN;
                lineTokens.push_back(make_pair(token, type));



                // catch missing left parentheses
                int counter = 0;
                if (codeLines[i][0] == ')') {
                    cout << "\n**ERROR: Unbalanced parentheses.**\n" << endl;
                    cout << "Line " << i + 1 << ":" << j + 1 << " : " << codeLines[i] << endl << endl;
                    return false;
                }
                while (codeLines[i][counter] != '(') {
                    if (counter > 0) {
                        cout << "\n**ERROR: Unbalanced parentheses.**\n" << endl;
                        cout << "Line " << i + 1 << " : " << codeLines[i] << endl << endl;
                        return false;
                    }
                    counter--;
                }
            }
            // string literal + catch unterminated string literal
            else if (codeLines[i][j] == '\"' || codeLines[i][j] == '\'') {
                type = categoryType::STRING_LITERAL;
                int counter = 1;

                // single quote
                if (codeLines[i][j] == '\"') {
                    while (codeLines[i][j + counter] != '\"') {
                        if (codeLines[i][j + counter] == '\'') {
                            cout << "\n**ERROR: Incorrect string literal usage. Expected \" and received \'.**\n" <<
                            endl;
                            cout << "Line " << i + 1 << ":" << j + 1 << " : " << codeLines[i] << endl << endl;
                            return false;
                        }
                        token.push_back(codeLines[i][j + counter]);
                        counter++;
                        // unterminated string literal; missing matching quotes
                        if (counter > codeLines[i].length()) {
                            cout << "\n**ERROR: Unterminated string literal.**\n" << endl;
                            cout << "Line " << i + 1 << ":" << j + 1 << " : " << codeLines[i] << endl << endl;
                            return false;
                        }
                    }
                    j = j + counter;
                    token = "\"" + token + "\"";
                }

                    // double quote
                else if (codeLines[i][j] == '\'') {
                    while (codeLines[i][j + counter] != '\'') {
                        if (codeLines[i][j + counter] == '\"') {
                            cout << "\n**ERROR: Incorrect string literal usage. Expected \' and received \".**\n" <<
                            endl;
                            cout << "Line " << i + 1 << ":" << j + 1 << " : " << codeLines[i] << endl << endl;
                            return false;
                        }
                        token.push_back(codeLines[i][j + counter]);
                        counter++;
                        // unterminated string literal; missing matching quotes
                        if (counter > codeLines[i].length()) {
                            cout << "\n**ERROR: Unterminated string literal.**\n" << endl;
                            cout << "Line " << i + 1 << ":" << j + 1 << " : " << codeLines[i] << endl << endl;
                            return false;
                        }
                    }
                    j = j + counter;
                    token = "\'" + token + "\'";
                }
                lineTokens.push_back(make_pair(token, type));
            }
            // comment
            else if (codeLines[i][j] == '#') {
                token = codeLines[i];
                type = categoryType::COMMENT;
                lineTokens.push_back(make_pair(token, type));
                break;
            }
            // <=
            else if (codeLines[i][j] == '<' && codeLines[i][j + 1] == '=') {
                token = "<=";
                type = categoryType::RELATIONAL_OP;
                lineTokens.push_back(make_pair(token, type));
                j++;
            }
            // >=
            else if (codeLines[i][j] == '>' && codeLines[i][j + 1] == '=') {
                token = ">=";
                type = categoryType::RELATIONAL_OP;
                lineTokens.push_back(make_pair(token, type));
                j++;
            }
            // <, >
            else if (codeLines[i][j] == '>') {
                token = ">";
                type = categoryType::RELATIONAL_OP;
                lineTokens.push_back(make_pair(token, type));
            }
                // <, >
            else if (codeLines[i][j] == '<') {
                token = "<";
                type = categoryType::RELATIONAL_OP;
                lineTokens.push_back(make_pair(token, type));
            }
            // ==, =
            else if (codeLines[i][j] == '=') {
                if (codeLines[i][j + 1] == '=') {
                    token = "==";
                    type = categoryType::RELATIONAL_OP;
                    lineTokens.push_back(make_pair(token, type));
                    j++;
                }
                else {
                    token = "=";
                    type = categoryType::ASSIGNMENT_OP;
                    lineTokens.push_back(make_pair(token, type));
                }
            }
            // !=
            else if (codeLines[i][j] == '!' && codeLines[i][j + 1] == '=') {
                token = "!=";
                type = categoryType::RELATIONAL_OP;
                lineTokens.push_back(make_pair(token, type));
                j++;
            }
            // arithmetic operators
            else if (codeLines[i][j] == '+' || codeLines[i][j] == '-' || codeLines[i][j] == '*' || codeLines[i][j] == '/' || codeLines[i][j] == '%') {
                token = codeLines[i][j];
                type = categoryType::ARITH_OP;
                lineTokens.push_back(make_pair(token, type));
            }
            // colon
            else if (codeLines[i][j] == ':') {
                token = codeLines[i][j];
                type = categoryType::COLON;
                lineTokens.push_back(make_pair(token, type));
            }
            // comma
            else if (codeLines[i][j] == ',') {
                token = codeLines[i][j];
                type = categoryType::COMMA;
                lineTokens.push_back(make_pair(token, type));
            }
            // white space
            else if (isspace(codeLines[i][j])) {
                // basically just do nothing here
                token = codeLines[i][j];
            }
            // unknown
            else {
                token = codeLines[i][j];
                type = categoryType::UNKNOWN;
                lineTokens.push_back(make_pair(token, type));
            }
        }
        tokenInfo.push_back(lineTokens);
        lineTokens.clear();
    }
    return true;
}

// display Lexical Analysis data
void LexicalAnalyzer::display(tokenType &tokens) {
    // for all lines in tokenType
    for (int i = 0; i < tokens.size(); i++) {
        cout << "Line #" << i + 1 << endl;

        // for all token pairs in a given line
        for (int j = 0; j < tokens[i].size(); j++) {
            cout << "TOKEN[" << j + 1 << "]    " << tokens[i][j].first << " -> ";
            // print out correct categoryType for given token
            switch(tokens[i][j].second) {
                case categoryType::KEYWORD: cout << "KEYWORD"; break;
                case categoryType::IDENTIFIER: cout << "IDENTIFIER"; break;
                case categoryType::STRING_LITERAL: cout << "STRING_LITERAL"; break;
                case categoryType::NUMERIC_LITERAL: cout << "NUMERIC_LITERAL"; break;
                case categoryType::ASSIGNMENT_OP: cout << "ASSIGNMENT_OP"; break;
                case categoryType::ARITH_OP: cout << "ARITH_OP"; break;
                case categoryType::LOGICAL_OP: cout << "LOGICAL_OP"; break;
                case categoryType::RELATIONAL_OP: cout << "RELATIONAL_OP"; break;
                case categoryType::LEFT_PAREN: cout << "LEFT_PAREN"; break;
                case categoryType::RIGHT_PAREN: cout << "RIGHT_PAREN"; break;
                case categoryType::COLON: cout << "COLON"; break;
                case categoryType::COMMA: cout << "COMMA"; break;
                case categoryType::COMMENT: cout << "COMMENT"; break;
                case categoryType::INDENT: cout << "INDENT"; break;
                case categoryType::UNKNOWN: cout << "UNKNOWN"; break;
            }
            cout << endl;
        }

        // print line divider
        for (int j = 0; j < 40; j++) {
            cout << "-";
        }
        cout << endl;
    }
}
