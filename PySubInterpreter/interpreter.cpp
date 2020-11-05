#include <iostream>
#include <vector>
#include <string>
#include "interpreter.h"
#include "expevaluator.h"
#include "lexicalanalyzer.h"

using namespace std;

bool Interpreter::interpretLine(tokenLineType lineTokens) {
	static expEvaluator expEvaluation;
	LexicalAnalyzer lexAnalysis;

	for (int i = 0; i < lineTokens.size(); i++) {
		// comments
		if (lineTokens[i].second == categoryType::COMMENT) {
			return true;
		}
		// assignment
		else if (lineTokens[i].second == categoryType::ASSIGNMENT_OP) {
			if (i != 0) {
				tokenLineType subLine;
				int counter = 1;
				while (i + counter <= lineTokens.size() - 1) {
					subLine.push_back(lineTokens[i + counter]);
					counter++;
				}
				string rValue = expEvaluation.evaluate(subLine);
				expEvaluation.storeSymbol(lineTokens[i - 1].first, rValue);
			}
		}
		// print()
		else if (lineTokens[i].second == categoryType::KEYWORD && lineTokens[i].first == "print") {
			// expects left parenthesis after keyword 'print'
			if (lineTokens.size() > i && lineTokens[i + 1].second == categoryType::LEFT_PAREN) {
				vector<tokenLineType> subLines;
				int counter = 2;
				tokenLineType subLine;

				// gather the different string literals and numbers on the left and right of commas
				while (lineTokens[i + counter].second != categoryType::RIGHT_PAREN && lineTokens.size() >= i + counter) {
					if (lineTokens[i + counter].second == categoryType::COMMA) {
						subLines.push_back(subLine);
						subLine.clear();
					}
					else {
						subLine.push_back(lineTokens[i + counter]);
					}
					counter++;
				}
				// push back outstanding line on the right of last comma, or the sole expr with no comma in the statement
				subLines.push_back(subLine);

				// evaluate contents
				for (int i = 0; i < subLines.size(); i++) {
					string temp = expEvaluation.evaluate(subLines[i]);
					subLines[i].clear();
					subLines[i].push_back(make_pair(temp, categoryType::STRING_LITERAL));
				}

				// get rid of quotes on the string literals
				for (int i = 0; i < subLines.size(); i++) {
					string firstChar = subLines[i][0].first;
					// single quotes
					if (firstChar[0] == '\'') {
						string temp = "";
						for (char c : subLines[i][0].first) {
							if (c != '\'') {
								temp += c;
							}	
						}
						subLines[i][0].first = temp;
					}
					// double quotes
					if (firstChar[0] == '\"') {
						string temp = "";
						for (char c : subLines[i][0].first) {
							if (c != '\"') {
								temp += c;
							}
						}
						subLines[i][0].first = temp;
					}
				}
				string final = "";
				vector<tokenLineType> temp = subLines;
				
				
				for (int i = 0; i < temp.size(); i++) {
					final = final + temp[i][0].first + " ";
				}

				// make temporary tokenLine that has one string literal of whatever is going to be printed and send it to be evaluated
				tokenLineType returnTemp;
				std::pair<std::string, std::string> pair;
				returnTemp.push_back(make_pair(final, categoryType::STRING_LITERAL));

				cout << expEvaluation.evaluate(returnTemp) << endl;
			}
		}
	}
	return false;
}
