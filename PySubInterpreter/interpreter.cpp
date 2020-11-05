#include <iostream>
#include <vector>
#include <string>
#include "interpreter.h"
#include "expevaluator.h"
#include "lexicalanalyzer.h"

using namespace std;

bool Interpreter::interpretLine(tokenLineType lineTokens) {
	expEvaluator expEvaluation;
	LexicalAnalyzer lexAnalysis;

	for (int i = 0; i < lineTokens.size(); i++) {
		// comments
		if (lineTokens[i].second == categoryType::COMMENT) {
			return true;
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
				subLines.push_back(subLine);

				for (auto line : subLines) {
					cout << line[0].first << endl;
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
						cout << temp << endl;
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
