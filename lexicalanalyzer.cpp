#include "lexicalanalyzer.h"
#include <iostream>
#include <cctype>
#include <vector>
#include <string>

using namespace std;

// generate token and category pairs for each line of code
// utilize lineTokens?
void LexicalAnalyzer::tokenize(vector<string> &codeLines) {
    for (const string& line : codeLines) {
        tokenLineType lineTokens;
        string token;
        categoryType type;

        // indent
        if (isspace(line[0])) {
            type = categoryType::INDENT;
            token = "____";
            lineTokens.push_back(make_pair(token, type));
        }

        for (int i = 0; i < line.size(); i++) {
            // clear current token, if any
            token = "";

            // numeric literal
            if (isdigit(line[i])) {
                type = categoryType::NUMERIC_LITERAL;
                token = line[i];

                // check if the next characters are digits too, if so, add them to same token
                int counter = 1;
                while (isdigit(line[i + counter])) {
                    token.push_back(line[i + counter]);
                    counter++;
                }
                i = i + counter;
                lineTokens.push_back(make_pair(token, type));
            }
                // first char is letter cases
            else if (isalpha(line[i])) {
                // PRINT keyword
                if (line[i] == 'p' && line[i + 1] == 'r' && line[i + 2] == 'i' && line[i + 3] == 'n' && line[i +
                                                                                                             4] == 't') {
                    token = "print";
                    type = categoryType::KEYWORD;
                    i = i + 4;
                }
                // IF keyword
                else if (line[i] == 'i' && line[i + 1] == 'f') {
                    token = "if";
                    type = categoryType::KEYWORD;
                    i++;
                }
                // ELIF keyword
                else if (line[i] == 'e' && line[i + 1] == 'l' && line[i + 2] == 'i' && line[i + 3] == 'f') {
                    token = "elif";
                    type = categoryType::KEYWORD;
                    i = i + 3;
                }
                // ELSE keyword
                else if (line[i] == 'e' && line[i + 1] == 'l' && line[i + 2] == 's' && line[i + 3] == 'e') {
                    token = "else";
                    type = categoryType::KEYWORD;
                    i = i + 3;
                }
                // FOR keyword
                else if (line[i] == 'f' && line[i + 1] == 'o' && line[i + 2] == 'r') {
                    token = "for";
                    type = categoryType::KEYWORD;
                    i = i + 2;
                }
                // WHILE keyword
                else if (line[i] == 'w' && line[i + 1] == 'h' && line[i + 2] == 'i' && line[i + 3] == 'l' &&
                         line[i +
                              4] == 'e') {
                    token = "while";
                    type = categoryType::KEYWORD;
                    i = i + 4;
                }
                // AND logical operator
                else if (line[i] == 'a' && line[i + 1] == 'n' && line[i + 2] == 'd') {
                    token = "and";
                    type = categoryType::LOGICAL_OP;
                    i = i + 2;
                }
                // OR logical operator
                else if (line[i] == 'o' && line[i + 1] == 'r') {
                    token = "or";
                    type = categoryType::LOGICAL_OP;
                    i++;
                }
                // NOT logical operator
                else if (line[i] == 'n' && line[i + 1] == 'o' && line[i + 2] == 't') {
                    token = "not";
                    type = categoryType::LOGICAL_OP;
                    i = i + 2;
                }
                // identifier of some sort
                else {
                    token = line[i];
                    type = categoryType::IDENTIFIER;
                    // check if the next characters are part of an identifier
                    int counter = 1;

                    while (isalpha(line[i + counter])) {
                        token.push_back(line[i + counter]);
                        counter++;
                    }
                    i = i + counter - 1;
                }
                lineTokens.push_back(make_pair(token, type));
            }
            // left parenthesis
            else if (line[i] == '(') {
                token = line[i];
                type = categoryType::LEFT_PAREN;
                lineTokens.push_back(make_pair(token, type));
            }
            // right parenthesis
            else if (line[i] == ')') {
                token = line[i];
                type = categoryType::RIGHT_PAREN;
                lineTokens.push_back(make_pair(token, type));
            }
            // string literal
            else if (line[i] == '\"' || line[i] == '\'') {
                type = categoryType::STRING_LITERAL;
                int counter = 1;

                // single quote
                if (line[i] == '\"') {
                    while (line[i + counter] != '\"') {
                        token.push_back(line[i + counter]);
                        counter++;
                    }
                    i = i + counter;
                    token = "\"" + token + "\"";
                }

                    // double quote
                else if (line[i] == '\'') {
                    while (line[i + counter] != '\'') {
                        token.push_back(line[i + counter]);
                        counter++;
                    }
                    i = i + counter;
                    token = "\'" + token + "\'";
                }
                lineTokens.push_back(make_pair(token, type));
            }
            // comment
            else if (line[i] == '#') {
                token = line;
                type = categoryType::COMMENT;
                lineTokens.push_back(make_pair(token, type));
                break;
            }
            // <, >
            else if (line[i] == '>' || line[i] == '<') {
                token = line[i];
                type = categoryType::RELATIONAL_OP;
                lineTokens.push_back(make_pair(token, type));
            }
            // <=, >=
            else if ((line[i] == '>' || line[i] == '<') && line[i + 1] == '=') {
                token = line[i] + line[i + 1];
                type = categoryType::RELATIONAL_OP;
                lineTokens.push_back(make_pair(token, type));
                i++;
            }
            // ==, =
            else if (line[i] == '=') {
                if (line[i + 1] == '=') {
                    token = "==";
                    type = categoryType::RELATIONAL_OP;
                    lineTokens.push_back(make_pair(token, type));
                    i++;
                }
                else {
                    token = "=";
                    type = categoryType::ASSIGNMENT_OP;
                    lineTokens.push_back(make_pair(token, type));
                }
            }
            // !=
            else if (line[i] == '!' && line[i + 1] == '=') {
                token = "!=";
                type = categoryType::RELATIONAL_OP;
                lineTokens.push_back(make_pair(token, type));
                i++;
            }
            // arithmetic operators
            else if (line[i] == '+' || line[i] == '-' || line[i] == '*' || line[i] == '/' || line[i] == '%') {
                token = line[i];
                type = categoryType::ARITH_OP;
                lineTokens.push_back(make_pair(token, type));
            }
            // colon
            else if (line[i] == ':') {
                token = line[i];
                type = categoryType::COLON;
                lineTokens.push_back(make_pair(token, type));
            }
            // comma
            else if (line[i] == ',') {
                token = line[i];
                type = categoryType::COMMA;
                lineTokens.push_back(make_pair(token, type));
            }
            // white space
            else if (isspace(line[i])) {
                // basically just do nothing here
                token = line[i];
            }
            // unknown
            else {
                token = line[i];
                type = categoryType::UNKNOWN;
                lineTokens.push_back(make_pair(token, type));
            }
        }
        tokenInfo.push_back(lineTokens);
        lineTokens.clear();
    }
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
