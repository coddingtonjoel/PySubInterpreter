#include <iostream>
#include <vector>
#include <string>
#include "interpreter.h"
#include "expevaluator.h"
#include "lexicalanalyzer.h"

using namespace std;

void Interpreter::interpretLine(tokenLineType lineTokens) {
	static expEvaluator expEvaluation;
	LexicalAnalyzer lexAnalysis;

	for (int i = 0; i < lineTokens.size(); i++) {
		// comments
		if (lineTokens[i].second == categoryType::COMMENT) {
			break;
		}
		// int conversion
		else if (lineTokens[i].second == categoryType::KEYWORD && lineTokens[i].first == "int") {
			if (lineTokens.size() > i && lineTokens[i + 1].second == categoryType::LEFT_PAREN) {
				vector<tokenLineType> subLines;
				int counter = 2;
				tokenLineType subLine;

				while (lineTokens[i + counter].second != categoryType::RIGHT_PAREN && lineTokens.size() >= i + counter) {
					subLine.push_back(lineTokens[i + counter]);
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
		else if (lineTokens[i].second == categoryType::ASSIGNMENT_OP) {
			if (i != 0) {
				// make sure variables aren't dedicated keywords
				if (lineTokens[i - 1].first == "print" || lineTokens[i - 1].first == "input" || lineTokens[i - 1].first == "if" || lineTokens[i - 1].first == "elif" || 
					lineTokens[i - 1].first == "else" || lineTokens[i - 1].first == "for" || lineTokens[i - 1].first == "while" || lineTokens[i - 1].first == "and" || lineTokens[i = 1].first == "int" || 
					lineTokens[i - 1].first == "or" || lineTokens[i - 1].first == "not") {
					cout << "\n**ERROR: Cannot store variable names that match protected Python keywords.**\n" << endl;
					break;
				}
				tokenLineType subLine;
				int counter = 1;
				while (i + counter <= lineTokens.size() - 1) {
					subLine.push_back(lineTokens[i + counter]);
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
										expEvaluation.storeSymbol(lineTokens[0].first, rValue);
										return;
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
					// otherwise just push it back
					else {
						string nestedIntConversion;
						// but if there's a nested int function, evaluate what's inside
						if (lineTokens[i + counter].second == categoryType::KEYWORD && lineTokens[i + counter].first == "int") {
							tokenLineType intSubLine;
							int nestedCounter = 2;
							while (lineTokens[i + counter + nestedCounter].second != categoryType::RIGHT_PAREN) {
								intSubLine.push_back(lineTokens[i + counter + nestedCounter]);
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
							subLine.push_back(lineTokens[i + counter]);
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
}
