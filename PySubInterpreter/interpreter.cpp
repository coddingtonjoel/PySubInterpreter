#include <iostream>
#include <vector>
#include <string>
#include "interpreter.h"
#include "expevaluator.h"
#include "lexicalanalyzer.h"

using namespace std;

int Interpreter::interpretLine(tokenType lineTokens, int n = 0) {
	// n short for lineNum (current line as of passed parameter)
	// returns lines to skip (for block purposes)
	static expEvaluator expEvaluation;
	LexicalAnalyzer lexAnalysis;
	for (int i = 0; i < lineTokens[n].size(); i++) {
		// comments
		if (lineTokens[n][i].second == categoryType::COMMENT) {
			break;
		}
		// if statement
		else if (lineTokens[n][i].second == categoryType::KEYWORD && (lineTokens[n][i].first == "if" || lineTokens[n][i].first == "elif")) {
			tokenLineType subLine;
			int counter = 1;

			// get the exp in a subLine and evaluate it with expEvaluation

			while (lineTokens[n][i + counter].second != categoryType::COLON && lineTokens[n].size() >= i + counter) {
				subLine.push_back(make_pair(lineTokens[n][i + counter].first, lineTokens[n][i + counter].second));
				counter++;
			}

			string eval;

			// check for errors within expression
			if (expEvaluation.checkForErrors(subLine) == "noErr") {
				// check if expression is true or false
				eval = expEvaluation.evaluate(subLine);
			}
			
			for (auto n : subLine) {
				cout << n.first << " ";
			}
			cout << "\n= " << eval << endl << endl;
			
			// if original "if" expression is true
			if (eval == "1") {
				int j = 1;
				// store each proceeding line with ** lineTokens[n][0].second == categoryType::INDENT ** in the stack
				while (n + j < lineTokens.size() && lineTokens[n + j][0].second == categoryType::INDENT) {
					block.push_back(lineTokens[n + j]);
					j++;
				}
				// interpret each statement in the block
				while (!block.empty()) {
					interpretLine(block);
					block.erase(block.begin());
				}
				// skip j of the next lines while interpreting the file
				return n + j;
			}
			// if original "if" expression is false
			else {
				int j = 0;
				int k = 1;
				block.clear();
				// else
				while (n + j < lineTokens.size()) {
					if (lineTokens[n + j][0].second == categoryType::KEYWORD && lineTokens[n + j][0].first == "else") {
						// block within else statement
						while (n + j + k < lineTokens.size() && lineTokens[n + j + k][0].second == categoryType::INDENT) {
							block.push_back(lineTokens[n + j + k]);
							k++;
						}
					}
					j++;
				}
				// interpret each statement in the block
				while (!block.empty()) {
					interpretLine(block);
					block.erase(block.begin());
				}
				return j + k;
			}

		}
		// while loop
		else if (lineTokens[n][i].second == categoryType::KEYWORD && lineTokens[n][i].first == "while") {
			tokenLineType subLine;
			int counter = 1;

			// get the exp in a subLine and evaluate it with expEvaluation

			while (lineTokens[n][i + counter].second != categoryType::COLON && lineTokens[n].size() >= i + counter) {
				subLine.push_back(make_pair(lineTokens[n][i + counter].first, lineTokens[n][i + counter].second));
				counter++;
			}

			string eval;

			// check for errors within expression
			if (expEvaluation.checkForErrors(subLine) == "noErr") {
				// check if expression is true or false
				eval = expEvaluation.evaluate(subLine);
			}

			// loop contitional is fufilled
			if (eval == "1") {
				int j = 1;
				while (eval != "0") {
					while (n + j < lineTokens.size() && lineTokens[n + j][0].second == categoryType::INDENT) {
						block.push_back(lineTokens[n + j]);
						j++;
					}
					// interpret each statement in the block
					while (!block.empty()) {
						interpretLine(block);
						block.erase(block.begin());
					}
				}
				return n + j;
			}
			// skip over remaining statement
			else {
				int j = 1;
				while (n + j < lineTokens.size() && lineTokens[n + j][0].second == categoryType::INDENT) {
					block.push_back(lineTokens[n + j]);
					j++;
				}
				return n + j;
			}
		}
		// int conversion
		else if (lineTokens[n][i].second == categoryType::KEYWORD && lineTokens[n][i].first == "int") {
			if (lineTokens[n].size() > i && lineTokens[n][i + 1].second == categoryType::LEFT_PAREN) {
				vector<tokenLineType> subLines;
				int counter = 2;
				tokenLineType subLine;

				while (lineTokens[n][i + counter].second != categoryType::RIGHT_PAREN && lineTokens[n].size() >= i + counter) {
					subLine.push_back(lineTokens[n][i + counter]);
					counter++;
				}
				// push back outstanding line on the right of last comma, or the sole expr with no comma in the statement
				subLines.push_back(subLine);

				// handle identifiers within int()
				for (int i = 0; i < subLines.size(); i++) {
					for (int j = 0; j < subLines[i].size(); j++) {
						// substitute identifiers for their symbol table values
						if (subLines[i][j].second == categoryType::IDENTIFIER) {
							string value = expEvaluation.getSymbol(subLines[i][j].first);
							subLines[i][j].first = value;
							// int converts them to numbers instead of strings
							subLines[i][j].second = categoryType::NUMERIC_LITERAL;
						}
					}
				}
				
				// evaluate final contents
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
				pair<string, string> pair;
				returnTemp.push_back(make_pair(final, categoryType::STRING_LITERAL));

				cout << expEvaluation.evaluate(returnTemp) << endl;
			}
		}
		// assignment
		else if (lineTokens[n][i].second == categoryType::ASSIGNMENT_OP) {
			if (i != 0) {
				// make sure variables aren't dedicated keywords
				if (lineTokens[n][i - 1].first == "print" || lineTokens[n][i - 1].first == "input" || lineTokens[n][i - 1].first == "if" || lineTokens[n][i - 1].first == "elif" || 
					lineTokens[n][i - 1].first == "else" || lineTokens[n][i - 1].first == "for" || lineTokens[n][i - 1].first == "while" || lineTokens[n][i - 1].first == "and" || lineTokens[n][i = 1].first == "int" || 
					lineTokens[n][i - 1].first == "or" || lineTokens[n][i - 1].first == "not") {
					cout << "\n**ERROR: Cannot store variable names that match protected Python keywords.**\n" << endl;
					break;
				}
				tokenLineType subLine;
				int counter = 1;
				while (i + counter <= lineTokens[n].size() - 1) {
					subLine.push_back(lineTokens[n][i + counter]);
					counter++;
				}

				string rValue;

				// handle case in which right side of the operator is an int function
				if (subLine[0].second == categoryType::KEYWORD && subLine[0].first == "int") {
					for (int i = 0; i < subLine.size(); i++) {
						if (subLine.size() > 1 && subLine[i + 1].second == categoryType::LEFT_PAREN) {
							vector<tokenLineType> intSubLines;
							tokenLineType intSubLine;
							int counter = 2;

							// get tokens between int(___)
							while (subLine[i + counter].second != categoryType::RIGHT_PAREN && subLine.size() >= i + counter) {
								intSubLine.push_back(subLine[i + counter]);
								if (subLine[i + counter + 1].second == categoryType::RIGHT_PAREN) {
									intSubLine.push_back(make_pair(")", categoryType::RIGHT_PAREN));
								}
								counter++;
							}
							// push back outstanding line on the right of last comma, or the sole expr with no comma in the statement
							intSubLines.push_back(intSubLine);

							for (int i = 0; i < intSubLines.size(); i++) {
								for (int j = 0; i < intSubLines[i].size(); j++) {
									if (intSubLines[i].size() > 1 && intSubLines[i][j + 1].second == categoryType::LEFT_PAREN) {
										vector<tokenLineType> inputSubLines;
										tokenLineType inputSubLine;
										int counter = 2;

										// gather the different string literals and numbers on the left and right of commas
										while (intSubLines[i][j + counter].second != categoryType::RIGHT_PAREN && intSubLines[i].size() >= j + counter) {
											inputSubLine.push_back(intSubLines[i][j + counter]);
											counter++;
										}
										// push back outstanding line on the right of last comma, or the sole expr with no comma in the statement
										inputSubLines.push_back(inputSubLine);

										// get rid of quotes on the string literals
										for (int i = 0; i < inputSubLines.size(); i++) {
											string firstChar = inputSubLines[i][0].first;
											// single quotes
											if (firstChar[0] == '\'') {
												string temp = "";
												for (char c : inputSubLines[i][0].first) {
													if (c != '\'') {
														temp += c;
													}
												}
												inputSubLines[i][0].first = temp;
											}
											// double quotes
											if (firstChar[0] == '\"') {
												string temp = "";
												for (char c : inputSubLines[i][0].first) {
													if (c != '\"') {
														temp += c;
													}
												}
												inputSubLines[i][0].first = temp;
											}
										}
										string final = "";
										vector<tokenLineType> temp = inputSubLines;

										for (int i = 0; i < temp.size(); i++) {
											final = final + temp[i][0].first + " ";
										}

										// make temporary tokenLine that has one string literal of whatever is going to be printed and send it to be evaluated
										tokenLineType returnTemp;
										pair<string, string> pair;
										returnTemp.push_back(make_pair(final, categoryType::STRING_LITERAL));

										string response;
										cout << final;
										cin >> response;
										rValue = response;
										if (lineTokens[n][0].second == categoryType::INDENT) {
											expEvaluation.storeSymbol(lineTokens[n][1].first, rValue);
										}
										else {
											expEvaluation.storeSymbol(lineTokens[n][0].first, rValue);
										}
										return 0;
									}
								}
							}

							// get rid of quotes on the string literals
							for (int i = 0; i < intSubLines.size(); i++) {
								string firstChar = intSubLines[i][0].first;
								// single quotes
								if (firstChar[0] == '\'') {
									string temp = "";
									for (char c : intSubLines[i][0].first) {
										if (c != '\'') {
											temp += c;
										}
									}
									intSubLines[i][0].first = temp;
								}
								// double quotes
								if (firstChar[0] == '\"') {
									string temp = "";
									for (char c : intSubLines[i][0].first) {
										if (c != '\"') {
											temp += c;
										}
									}
									intSubLines[i][0].first = temp;
								}
							}
							string final = "";
							vector<tokenLineType> temp = intSubLines;

							for (int i = 0; i < temp.size(); i++) {
								final = final + temp[i][0].first + " ";
							}

							// make temporary tokenLine that has one string literal of whatever is going to be printed and send it to be evaluated
							tokenLineType returnTemp;
							pair<string, string> pair;
							returnTemp.push_back(make_pair(final, categoryType::STRING_LITERAL));

							rValue = stod(final);
							break;
						}
					}
				}

				// handle case in which right side of operator is an input function
				else if (subLine[0].second == categoryType::KEYWORD && subLine[0].first == "input") {
					for (int i = 0; i < subLine.size(); i++) {
						if (subLine.size() > 1 && subLine[i + 1].second == categoryType::LEFT_PAREN) {
							vector<tokenLineType> inputSubLines;
							tokenLineType inputSubLine;
							int counter = 2;

							// gather the different string literals and numbers on the left and right of commas
							while (subLine[i + counter].second != categoryType::RIGHT_PAREN && subLine.size() >= i + counter) {
								inputSubLine.push_back(subLine[i + counter]);
								counter++;
							}
							// push back outstanding line on the right of last comma, or the sole expr with no comma in the statement
							inputSubLines.push_back(inputSubLine);

							// get rid of quotes on the string literals
							for (int i = 0; i < inputSubLines.size(); i++) {
								string firstChar = inputSubLines[i][0].first;
								// single quotes
								if (firstChar[0] == '\'') {
									string temp = "";
									for (char c : inputSubLines[i][0].first) {
										if (c != '\'') {
											temp += c;
										}
									}
									inputSubLines[i][0].first = temp;
								}
								// double quotes
								if (firstChar[0] == '\"') {
									string temp = "";
									for (char c : inputSubLines[i][0].first) {
										if (c != '\"') {
											temp += c;
										}
									}
									inputSubLines[i][0].first = temp;
								}
							}
							string final = "";
							vector<tokenLineType> temp = inputSubLines;

							for (int i = 0; i < temp.size(); i++) {
								final = final + temp[i][0].first + " ";
							}

							// make temporary tokenLine that has one string literal of whatever is going to be printed and send it to be evaluated
							tokenLineType returnTemp;
							pair<string, string> pair;
							returnTemp.push_back(make_pair(final, categoryType::STRING_LITERAL));

							string response;
							cout << final;
							cin >> response;
							rValue = response;
							break;
						}
					}
				}
				else {
					rValue = expEvaluation.evaluate(subLine);
				}

				if (lineTokens[n][i - 1].second == categoryType::INDENT) {
					expEvaluation.storeSymbol(lineTokens[n][i].first, rValue);
				}
				else {
					expEvaluation.storeSymbol(lineTokens[n][i - 1].first, rValue);
				}
			}
		}
		
		// print()
		else if (lineTokens[n][i].second == categoryType::KEYWORD && lineTokens[n][i].first == "print") {
			// expects left parenthesis after keyword 'print'
			if (lineTokens[n].size() > i && lineTokens[n][i + 1].second == categoryType::LEFT_PAREN) {
				vector<tokenLineType> subLines;
				int counter = 2;
				tokenLineType subLine;

				// gather the different string literals and numbers on the left and right of commas
				while (lineTokens[n][i + counter].second != categoryType::RIGHT_PAREN && lineTokens[n].size() >= i + counter) {
					if (lineTokens[n][i + counter].second == categoryType::COMMA) {
						subLines.push_back(subLine);
						subLine.clear();
					}
					// otherwise just push it back
					else {
						string nestedIntConversion;
						// but if there's a nested int function, evaluate what's inside
						if (lineTokens[n][i + counter].second == categoryType::KEYWORD && lineTokens[n][i + counter].first == "int") {
							tokenLineType intSubLine;
							int nestedCounter = 2;
							while (lineTokens[n][i + counter + nestedCounter].second != categoryType::RIGHT_PAREN) {
								intSubLine.push_back(lineTokens[n][i + counter + nestedCounter]);
								nestedCounter++;
							}

							for (int i = 0; i < intSubLine.size(); i++) {
								for (int j = 0; j < intSubLine.size(); j++) {
									// substitute identifiers for their symbol table values
									if (intSubLine[i].second == categoryType::IDENTIFIER) {
										string value = expEvaluation.getSymbol(intSubLine[i].first);
										intSubLine[i].first = value;
										// int converts them to numbers instead of strings
										intSubLine[i].second = categoryType::NUMERIC_LITERAL;
									}
								}
							}
							nestedIntConversion = expEvaluation.evaluate(intSubLine);
							subLine.push_back(make_pair(nestedIntConversion, categoryType::NUMERIC_LITERAL));
							i += nestedCounter;
						}
						else {
							subLine.push_back(lineTokens[n][i + counter]);
						}
						
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
				pair<string, string> pair;
				returnTemp.push_back(make_pair(final, categoryType::STRING_LITERAL));

				cout << expEvaluation.evaluate(returnTemp) << endl;
			}
		}
	}
	return 0;
}

void Interpreter::clearStack() {
	while (!block.empty()) {
		block.pop_back();
	}
}

bool Interpreter::isStackEmpty() {
	if (block.size() == 0) {
		return true;
	}
	else return false;
}
