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
			tokenLineType subLine;
			// expects left parenthesis after keyword 'print'
			cout << "1" << endl;
			if (lineTokens.size() > i && lineTokens[i + 1].second == categoryType::LEFT_PAREN) {
				int counter = 2;
				cout << "2" << endl;
				while (lineTokens[i + counter].second != categoryType::RIGHT_PAREN && lineTokens.size() >= i + counter + 1) {
					subLine.push_back(lineTokens[i + counter]);
					counter++;
				}
				cout << "3" << endl;
				for (auto pair : subLine) {
					cout << pair.first << endl;
				}
				cout << expEvaluation.evaluate(subLine) << endl;
				cout << "6" << endl;
				// clear token info because it was an inline expression
				// but DON'T clear symbol table here!
				lexAnalysis.tokenInfo.clear();
			}
		}
	}
	return false;
}
